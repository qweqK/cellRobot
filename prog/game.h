#pragma once
#include "parser.hpp"
#include <string>
#include <map>
#include "data.h"
class Game {
    Data data;
    yy::parser parser;
    std::string filename;

    public:
    //void printTable();
    Game(std::string str) : data(), parser(yy::parser(data)), filename(std::move(str)) {}
    void run();
};


