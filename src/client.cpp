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

#define H1 "FTX-KEY: "
#define H2 "FTX-TS: "
#define H3 "FTX-SIGN: "

using namespace std;
using namespace rapidjson;

// To prevent parse errors due to partial responses, maintain a in-memory structure that
// changes in size. Parsing to json will be done after the full response has been received.
// Reference 1: https://curl.se/libcurl/c/getinmemory.html
// Reference 2: https://stackoverflow.com/questions/12903587/handling-http-responses-from-libcurl-over-multiple-callbacks
size_t Client::write_response(void * data, size_t size, size_t nmemb, void * userp)
{
    size_t realsize = size * nmemb;
    mem_struct *mem = (mem_struct *)userp;
    
    char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        /* out of memory! */ 
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), data, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
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
    
    string data = time_in_string + "GET" + path;
    string hmacced = encoding::hmac(this->secret, data, 32);
    string sign = encoding::string_to_hex((unsigned char*)hmacced.c_str(), 32);

    slist = curl_slist_append(slist, (H1 + this->key).c_str());
    slist = curl_slist_append(slist, (H2 + time_in_string).c_str());
    slist = curl_slist_append(slist, (H3 + sign).c_str());

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

// Get single market data. FTX does not require authentication
Document Client::get_single_market(string market){

    string path = "/api/markets/" + market;
    string url = endpoint_append(path);

    struct curl_slist *slist = NULL;
    
    Document result = send_request(url, slist);
    
    return result;
}

// Get single market orderbook. FTX does not require authentication
Document Client::get_single_orderbook(string market, string depth){

    string path = "/api/markets/" + market + "/orderbook?depth=" + depth;
    string url = endpoint_append(path);

    struct curl_slist *slist = NULL;
    
    Document result = send_request(url, slist);
    
    return result;
}

Document Client::get_trades(string market, string limit){

    string path = "/api/markets/" + market + "/trades?limit=" + limit;
    string url = endpoint_append(path);

    struct curl_slist *slist = NULL;
    slist = set_header(slist, path);
    
    Document result = send_request(url, slist);
    
    return result;
}

Document Client::get_hist_specified(string market, string resolution, string limit, string start_time, string end_time){

    string path = "/api/markets/" + market + "/candles?resolution=" + resolution;

    if(limit != ""){
        path = path + "&limit=" + limit;
    }

    if(start_time != ""){
        path = path + "&start_time=" + start_time;
    }

    if(end_time != ""){
        path = path + "&end_time=" + end_time;
    }

    string url = endpoint_append(path);

    struct curl_slist *slist = NULL;
    
    Document result = send_request(url, slist);
    
    return result;
}

Document Client::get_hist_recent(string market, string resolution, string limit){

    string path = "/api/markets/" + market + "/candles?resolution=" + resolution + "&limit=" + limit;
    string url = endpoint_append(path);
    cout << url << endl;

    struct curl_slist *slist = NULL;
    slist = set_header(slist, path);
    
    Document result = send_request(url, slist);
    
    return result;
}

Document Client::send_request(string url, struct curl_slist * slist){

    CURL *curl;
    CURLcode res;

    mem_struct mem;
    mem.memory = (char *)malloc(1);     /* will be grown as needed by the realloc above */ 
    mem.size = 0;                       /* no data at this point */ 

    curl = curl_easy_init();
    if(curl) {

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

        Document doc;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&mem);
    
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);

        if((&doc)->Parse((char *)mem.memory).HasParseError()){
            cout << "Parse Error" << endl;
            return NULL;
        };

        /* Check for errors */ 
        if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    
        /* always cleanup */ 
        curl_easy_cleanup(curl);
        curl_slist_free_all(slist);
        free(mem.memory);

        return doc;
    }
    return NULL;
}
