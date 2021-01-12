#include <iostream>
#include <stdio.h>
#include <ctime>
#include <string>
#include "client.h"
#include "parser.h"
#include "query.h"
#include "db.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <mongoc/mongoc.h>

using namespace std;
using namespace rapidjson;


int main(){
    
    Database db = Database();
    db.check_db();

    Client c = Client("https://ftx.com");

    Query q = Query(&c, &db);

    q.populate_db("BTC/USD", "2020-01-01");
    q.populate_db("ETH/USD", "2020-01-01");

    return 0;
}