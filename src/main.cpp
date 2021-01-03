#include <iostream>
#include <stdio.h>
#include <ctime>
#include <string>
#include "client.h"
#include "parser.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;


int main(){

    Client c = Client("https://ftx.com");

    //c.get_account();
    //c.get_single_market("BTC/USD");
    Document* doc = c.get_single_orderbook("BTC/USD", "20");

    Parser p;
    p.parse_orderbook(doc);

    //c.get_trades("BTC/USD", "20");
    c.get_hist_recent("BTC/USD", "300", "20");

    return 0;
}