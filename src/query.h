#ifndef QUERY_H
#define QUERY_H

#include "client.h"
#include "parser.h"
#include "db.h"
#include <vector>

class Query{

    public:
        Query(Client *c, Database *db){
            this->client = c;
            this->db = db;
        }
        
        int insert_db(std::string col_name, std::vector<hist_p> *v);
        int populate_db(std::string market, std::string start_time);
        std::vector<hist_p> get_year_hist(std::string market);
        std::vector<hist_p> get_hist_from(std::string market, std::string start_time);
        std::vector<hist_p> get_hist_custom(std::string market, std::string start_time, std::string end_time, std::string resolution, std::string limit);
        std::vector<hist_p> get_from_db(std::string market, std::string start_time);

    private:
        Client *client;
        Database *db;
        long get_next_epoch(std::string start_time, std::string market, long interval);
};

#endif