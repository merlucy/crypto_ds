#ifndef PARSER_H
#define PARSER_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class Parser{

    public:
        // Take json as input, and populate db
        int parse_orderbook(rapidjson::Document *doc);
        int parse_historical(rapidjson::Document *doc);
};

#endif /* PARSER_H */