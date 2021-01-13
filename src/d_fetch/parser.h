#ifndef PARSER_H
#define PARSER_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "../util/d_struct.h"

#include <vector>

class Parser{

    public:
        // Take json as input, and populate db
        int write_orderbook(rapidjson::Document *doc);
        int write_historical(rapidjson::Document *doc);
        int ser_orderbook(rapidjson::Document *doc, std::vector<hist_p> *v);
        int ser_historical(rapidjson::Document *doc, std::vector<hist_p> *v);
};

#endif /* PARSER_H */