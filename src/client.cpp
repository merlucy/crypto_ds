#include <iostream>
#include <stdio.h>
#include <ctime>
#include <string>
#include "util/encoding.h"
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <curl/curl.h>
#include <chrono>
#include "client.h"

using namespace std;
using namespace rapidjson;

size_t Client::to_json(void * data, size_t size, size_t nmemb, void * doc)
{
    size_t realsize = size*nmemb;
    Document *d = (Document *)doc;

    d->Parse((char *)data);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d->Accept(writer);
    cout << buffer.GetString() << endl;

    return realsize;
}

string Client::endpoint_append(string path){
    return this->endpoint + path;
}

struct curl_slist* Client::set_header(struct curl_slist * slist, string path){
    
    // Get Unix Epoch
    const auto ts = chrono::system_clock::now();
    long p1 = chrono::duration_cast<chrono::milliseconds>(ts.time_since_epoch()).count();
    string time_in_string = to_string(p1);
    
    string h1 = "FTX-KEY: ";
    string h2 = "FTX-TS: ";
    string h3 = "FTX-SIGN: ";

    string data = time_in_string + "GET" + path;
    string hmacced = encoding::hmac(this->secret, data, 32);
    string sign = encoding::string_to_hex((unsigned char*)hmacced.c_str(), 32);

    slist = curl_slist_append(slist, (h1 + this->key).c_str());
    slist = curl_slist_append(slist, (h2 + time_in_string).c_str());
    slist = curl_slist_append(slist, (h3 + sign).c_str());

    return slist;
}

Document Client::get_account(){

    string path = "/api/account";
    string url = endpoint_append(path);

    struct curl_slist *slist = NULL;
    slist = set_header(slist, path);
    
    Document result = send_request(url, slist);
    
    return result;
}

Document Client::send_request(string url, struct curl_slist * slist){

    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) {

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

        Document json;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, to_json);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&json);
    
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);

        /* Check for errors */ 
        if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    
        /* always cleanup */ 
        curl_easy_cleanup(curl);
        curl_slist_free_all(slist);

        return json;
    }

    return NULL;
}
