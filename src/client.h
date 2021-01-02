#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <fstream>
#include <curl/curl.h>
#include "rapidjson/document.h"

class Client{
    public:
        Client(std::string endpoint){
            this->endpoint = endpoint;

            std::ifstream file;
            file.open("ftx_key.txt");

            file >> this->key >> this->secret;
            file.close();
            //Add confirmation output
        };

        rapidjson::Document get_account();
        static size_t to_json(void * data, size_t size, size_t nmemb, void * doc);

    private:
        std::string endpoint;
        std::string key;
        std::string secret;
        std::string endpoint_append(std::string path);
        struct curl_slist* set_header(struct curl_slist * slist, std::string path);
        rapidjson::Document send_request(std::string url, struct curl_slist* slist);
};

#endif /* CLIENT_H */