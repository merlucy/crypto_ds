#include <iostream>
#include <stdio.h>
#include <ctime>
#include <string>
#include "client.h"
#include "parser.h"
#include "query.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;


int main(){

    Client c = Client("https://ftx.com");
    Query q = Query(&c);

    //q.get_year_hist("BTC/USD");
    q.get_hist_from("BTC/USD", "2020-01-01");

    return 0;
}