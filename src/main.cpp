#include <iostream>
#include <stdio.h>
#include <ctime>
#include <string>
#include "client.h"

using namespace std;

int main(){

    Client c;

    // std::cout << "hello world" << std::endl;

    // // 1. Parse a JSON string into DOM.
    // const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    // Document d;
    // d.Parse(json);
 
    // // 2. Modify it by DOM.
    // Value& s = d["stars"];
    // s.SetInt(s.GetInt() + 1);
 
    // // 3. Stringify the DOM
    // StringBuffer buffer;
    // Writer<StringBuffer> writer(buffer);
    // d.Accept(writer);
 
    // // Output {"project":"rapidjson","stars":11}
    // cout << buffer.GetString() << endl;

    c.send_request();

    return 0;
}