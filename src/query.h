#ifndef QUERY_H
#define QUERY_H

#include "client.h"

class Query{

    public:
        Query(Client *c){
            this->client = c;
        }

        int get_year_hist(std::string market);
        int get_hist_from(std::string market, std::string start_time);
        int get_hist_custom(std::string market, std::string start_time, std::string end_time, std::string resolution, std::string limit);

    private:
        Client *client;
};

#endif