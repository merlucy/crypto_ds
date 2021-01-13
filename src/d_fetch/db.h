#ifndef DB_H
#define DB_H

#include <mongoc/mongoc.h>
#include <string>
#include <vector>
#include "../util/d_struct.h"

class Database{

    public:
        Database();
        ~Database();
        bool check_db();
        bool insert_hist(std::string market, std::vector<hist_p> *v);

        std::string check_latest(std::string market);
        std::vector<hist_p> get_all_db_from(std::string market, long epoch);
        long get_latest(std::string market);
    private:
        mongoc_uri_t *uri;
        mongoc_client_t *client;
        mongoc_database_t *database;
        const char *db_name = "db_name";
        const char *uri_string = "mongodb://localhost:27017";
};

#endif