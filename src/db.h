#ifndef DB_H
#define DB_H

#include <mongoc/mongoc.h>
#include <string>
#include <vector>
#include "d_struct.h"

class Database{

    public:
        Database();
        ~Database();
        bool check_db();
        bool insert_hist(std::string market, std::vector<hist_p> *v);

    private:
        mongoc_uri_t *uri;
        mongoc_client_t *client;
        mongoc_database_t *database;
        const char *db_name = "db_name";
        const char *uri_string = "mongodb://localhost:27017";
};

#endif