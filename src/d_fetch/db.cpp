#include "db.h"
#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <iostream>
#include <vector>
#include "rapidjson/document.h"

using namespace std;

Database::Database(){

    bson_error_t error;
    mongoc_init();

    uri = mongoc_uri_new_with_error (this->uri_string, &error);
    if (!uri) {
        fprintf (stderr,
                "failed to parse URI: %s\n"
                "error message:       %s\n",
                uri_string,
                error.message);
    }

    this->client = mongoc_client_new_from_uri (uri);
    if (!this->client) {
    }
    mongoc_client_set_appname (this->client, "connect-example");

    this->database = mongoc_client_get_database (this->client, this->db_name);
}

Database::~Database(){
    mongoc_database_destroy (this->database);
    mongoc_client_destroy (this->client);
    mongoc_cleanup ();

    /*
        * Release our handles and clean up libmongoc
        */
    mongoc_uri_destroy (uri);
}

// Check db connection, where mongod listening to port localhost:27017
bool Database::check_db(){

    bson_t *command, reply;
    bson_error_t error;
    char *str;
    bool retval;

    command = BCON_NEW ("ping", BCON_INT32 (1));

    retval = mongoc_client_command_simple (
      this->client, "admin", command, NULL, &reply, &error);

    if (!retval) {
        fprintf (stderr, "%s\n", error.message);
    }

    str = bson_as_json (&reply, NULL);
    printf ("%s\n", str);

    bson_destroy (&reply);
    bson_destroy (command);
    bson_free (str);

    return true;
}

bool Database::insert_hist(string market, vector<hist_p> *v){
    mongoc_collection_t *collection;
    collection = mongoc_client_get_collection(this->client, this->db_name, market.c_str());

    bson_t *document;
    bson_error_t error;

    for(vector<hist_p>::iterator itr = v->begin(); itr != v->end(); itr++){

        document = bson_new();

        bson_append_date_time(document, "time", -1, int64_t(itr->epoch));
        bson_append_double(document, "close", -1, itr->close);
        bson_append_double(document, "high", -1, itr->high);
        bson_append_double(document, "low", -1, itr->low);
        bson_append_double(document, "open", -1, itr->open);
        bson_append_double(document, "epoch", -1, itr->epoch);
        bson_append_double(document, "volume", -1, itr->volume);

        if (!mongoc_collection_insert_one (
            collection, document, NULL, NULL, &error)) {
            fprintf (stderr, "%s\n", error.message);
        }

        bson_destroy(document);
    }

    mongoc_collection_destroy(collection);

    return true;
}

// Check the latest time stamp of the final data point inside the collection
long Database::get_latest(string market){
    mongoc_collection_t *collection;
    collection = mongoc_client_get_collection(this->client, this->db_name, market.c_str());

    long result = 0;
    bson_t *query;
    bson_t *filter;
    const bson_t *doc;
    char *str;
    mongoc_cursor_t *cursor;

    query = bson_new();
    filter = bson_new();

    bson_append_document_begin(query, "sort", -1, filter);
    bson_append_int32(filter, "epoch", -1, -1);
    bson_append_document_end(query, filter);

    cursor = mongoc_collection_find_with_opts(collection, bson_new(), query, NULL);

    if(mongoc_cursor_next(cursor, &doc)){
        str = bson_as_relaxed_extended_json(doc, NULL);

        rapidjson::Document temp;
        temp.Parse(str);
        result = long(temp["epoch"].GetDouble());

        bson_free (str);
    }

    bson_destroy(filter);
    bson_destroy(query);
    mongoc_collection_destroy(collection);

    return result;
}

// Get data points from the collection since the given UNIX epoch
vector<hist_p> Database::get_all_db_from(string market, long epoch){
    
    mongoc_collection_t *collection;
    collection = mongoc_client_get_collection(this->client, this->db_name, market.c_str());

    bson_t *query;
    bson_t *filter;
    const bson_t *doc;
    char *str;
    mongoc_cursor_t *cursor;
    vector<hist_p> result;

    query = bson_new();
    filter = bson_new();

    bson_append_document_begin(query, "time", -1, filter);
    bson_append_date_time(filter, "$gte", -1, epoch);
    bson_append_document_end(query, filter);

    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);


    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_relaxed_extended_json (doc, NULL);

        rapidjson::Document temp;
        hist_p p;
        temp.Parse(str);

        p.time = temp["time"]["$date"].GetString();
        p.epoch = temp["epoch"].GetDouble();
        p.close = temp["close"].GetDouble();
        p.high = temp["high"].GetDouble();
        p.low = temp["low"].GetDouble();
        p.open = temp["open"].GetDouble();
        p.volume = temp["volume"].GetDouble();

        result.push_back(p);
        bson_free (str);
    }

    bson_destroy(filter);
    bson_destroy(query);
    mongoc_collection_destroy(collection);

    return result;
}