#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <fstream>

class Client{
    public:
        Client(){
            std::ifstream file;
            file.open("ftx_key.txt");

            file >> this->key >> this->secret;
            file.close();
        };

        static size_t to_json(void * data, size_t size, size_t nmemb, void * doc);
        void send_request();

    private:
        std::string key;
        std::string secret;
};


#endif /* CLIENT_H */