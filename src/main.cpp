#include <iostream>
#include <stdio.h>
#include <ctime>
#include <string>
#include "client.h"

using namespace std;

int main(){

    Client c = Client("https://ftx.com");

    c.get_account();

    return 0;
}