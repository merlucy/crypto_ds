#include <websocketpp/config/asio_client.hpp>
//#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include <iostream>
#include <map>
#include <string>

typedef websocketpp::client<websocketpp::config::asio_client> ws_client;

class connection_metadata {
    public:
        typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;
    
        connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri)
        : m_id(id)
        , m_hdl(hdl)
        , m_status("Connecting")
        , m_uri(uri)
        , m_server("N/A")
        {}
    
        void on_open(ws_client * c, websocketpp::connection_hdl hdl) {
            m_status = "Open";
    
            ws_client::connection_ptr con = c->get_con_from_hdl(hdl);
            m_server = con->get_response_header("Server");
        }
    
        void on_fail(ws_client * c, websocketpp::connection_hdl hdl) {
            m_status = "Failed";
    
            ws_client::connection_ptr con = c->get_con_from_hdl(hdl);
            m_server = con->get_response_header("Server");
            m_error_reason = con->get_ec().message();
        }

        void on_close(ws_client * c, websocketpp::connection_hdl hdl){
            m_status = "Closed";
            ws_client::connection_ptr con = c->get_con_from_hdl(hdl);
            std::stringstream s;
            s << "close code: " << con->get_remote_close_code() << " (" 
            << websocketpp::close::status::get_string(con->get_remote_close_code()) 
            << "), close reason: " << con->get_remote_close_reason();
            m_error_reason = s.str();
        }

        websocketpp::connection_hdl get_hdl(){
            return m_hdl;
        }

        std::string get_status(){
            return m_status;
        }

        int get_id(){
            return m_id;
        }
    
        friend std::ostream & operator<< (std::ostream & out, connection_metadata const & data);

    private:
        int m_id;
        websocketpp::connection_hdl m_hdl;
        std::string m_status;
        std::string m_uri;
        std::string m_server;
        std::string m_error_reason;
};
 
std::ostream & operator<< (std::ostream & out, connection_metadata const & data) {
    out << "> URI: " << data.m_uri << "\n"
        << "> Status: " << data.m_status << "\n"
        << "> Remote Server: " << (data.m_server.empty() ? "None Specified" : data.m_server) << "\n"
        << "> Error/close reason: " << (data.m_error_reason.empty() ? "N/A" : data.m_error_reason);
 
    return out;
}

class Websocket_client{

    public:
        Websocket_client();
        ~Websocket_client();

        int connect(std::string const & uri);
        void close(int id, websocketpp::close::status::value code);

    private:

        ws_client client;
        typedef std::map<int,connection_metadata::ptr> con_list;
        int m_next_id;
        con_list m_connection_list;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

        connection_metadata::ptr get_metadata(int id) const;
};

Websocket_client::Websocket_client():m_next_id(0){

    client.clear_access_channels(websocketpp::log::alevel::all);
    client.clear_error_channels(websocketpp::log::elevel::all);

    client.init_asio();
    client.start_perpetual();

    m_thread.reset(new websocketpp::lib::thread(&ws_client::run, &client));
}

int Websocket_client::connect(std::string const & uri){

    websocketpp::lib::error_code ec;
    ws_client::connection_ptr con = client.get_connection(uri, ec);

    if (ec) {
        std::cout << "> Connect initialization error: " << ec.message() << std::endl;
        return -1;
    }

    int new_id = m_next_id++;
    connection_metadata::ptr metadata_ptr(new connection_metadata(new_id, con->get_handle(), uri));
    m_connection_list[new_id] = metadata_ptr;

    con->set_open_handler(websocketpp::lib::bind(
        &connection_metadata::on_open,
        metadata_ptr,
        &client,
        websocketpp::lib::placeholders::_1
    ));

    con->set_fail_handler(websocketpp::lib::bind(
        &connection_metadata::on_fail,
        metadata_ptr,
        &client,
        websocketpp::lib::placeholders::_1
    ));

    client.connect(con);

    return new_id;
}

connection_metadata::ptr Websocket_client::get_metadata(int id) const {
    con_list::const_iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        return connection_metadata::ptr();
    } else {
        return metadata_it->second;
    }
}

void Websocket_client::close(int id, websocketpp::close::status::value code) {
    websocketpp::lib::error_code ec;
    
    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }
    
    client.close(metadata_it->second->get_hdl(), code, "", ec);
    if (ec) {
        std::cout << "> Error initiating close: " << ec.message() << std::endl;
    }
}

Websocket_client::~Websocket_client() {
    client.stop_perpetual();
    
    for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it) {
        if (it->second->get_status() != "Open") {
            // Only close open connections
            continue;
        }
        
        std::cout << "> Closing connection " << it->second->get_id() << std::endl;
        
        websocketpp::lib::error_code ec;
        client.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", ec);
        if (ec) {
            std::cout << "> Error closing connection " << it->second->get_id() << ": "  
                      << ec.message() << std::endl;
        }
    }
    
    m_thread->join();
}