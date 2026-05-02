#pragma once
#include "parser.hpp"
#include <string>
#include <map>
#include "data.h"
#include "robot.h"
class Game {
    Data data;
    GameMap gameMap;
    yy::parser parser;
    std::string filename;
    public:
    //void printTable();
    Game(std::string str, std::string mappath= "") :  gameMap(mappath), data(gameMap), parser(data), filename(std::move(str)) {}
    void run();
};


