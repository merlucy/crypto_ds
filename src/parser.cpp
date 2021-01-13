#include "parser.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/pointer.h"

using namespace rapidjson;
using namespace std;

int Parser::write_orderbook(Document * doc){

    Value * result = Pointer("/result").Get(*doc);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    result->Accept(writer);
    std::cout << buffer.GetString() << std::endl;

    return 1;
}

int Parser::write_historical(Document * doc){

    const Value& result = (*doc)["result"];
    assert(result.IsArray());

    ofstream file;
    file.open("temp.txt", ofstream::app);
    file << setprecision(13);

    for(SizeType i = 0; i < result.Size(); i++){
        const Value& element = result[i];
        
        file << element["close"].GetDouble() << ' ';
        file << element["high"].GetDouble() << ' ';
        file << element["low"].GetDouble() << ' ';
        file << element["open"].GetDouble() << ' ';
        file << element["startTime"].GetString() << ' ';
        file << element["time"].GetDouble() << ' ';
        file << element["volume"].GetDouble() << '\n';
    }

    file.close();

    return 1;
}

// int Parser::ser_orderbook(Document * doc, vector<hist_p> *v){
//     return 1;
// }

/*  Serialize historical data points into hist structs
*
*/
int Parser::ser_historical(Document * doc, vector<hist_p> *v){

    const Value& result = (*doc)["result"];
    assert(result.IsArray());

    for(SizeType i = 0; i < result.Size(); i++){
        const Value& element = result[i];
        hist_p elem;

        elem.close = element["close"].GetDouble();
        elem.high = element["high"].GetDouble();
        elem.low = element["low"].GetDouble();
        elem.open = element["open"].GetDouble();
        elem.time = element["startTime"].GetString();
        elem.epoch = element["time"].GetDouble();
        elem.volume = element["volume"].GetDouble();
        v->push_back(elem);
    }

    for(vector<hist_p>::iterator itr = v->begin(); itr != v->end(); itr++){

        cout << itr->close << " ";
        cout << itr->high << " ";
        cout << itr->low << " ";
        cout << itr->open << " ";
        cout << itr->time << " ";
        cout << itr->epoch << " ";
        cout << itr->volume << endl;
    }

    return 1;
}

