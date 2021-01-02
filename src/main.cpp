#include <iostream>
#include <stdio.h>
#include <ctime>
#include <string>
#include "client.h"

using namespace std;

int main(){

    Client c = Client("https://ftx.com");

    //c.get_account();
    //c.get_single_market("BTC/USD");
    //c.get_single_orderbook("BTC/USD", "20");
    //c.get_trades("BTC/USD", "20");
    c.get_hist_recent("BTC/USD", "300", "20");

    return 0;
}