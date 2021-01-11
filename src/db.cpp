#include "db.h"
#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <iostream>

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

        bson_append_date_time(document, "time", -1, int64_t(itr->time));
        bson_append_double(document, "close", -1, itr->close);
        bson_append_double(document, "high", -1, itr->high);
        bson_append_double(document, "low", -1, itr->low);
        bson_append_double(document, "open", -1, itr->open);
        bson_append_double(document, "time", -1, itr->time);
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
