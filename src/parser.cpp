#include "parser.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/pointer.h"

using namespace rapidjson;
using namespace std;

int Parser::parse_orderbook(Document * doc){

    Value * result = Pointer("/result").Get(*doc);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    result->Accept(writer);
    std::cout << buffer.GetString() << std::endl;

    return 1;
}

int Parser::parse_historical(Document * doc){

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
