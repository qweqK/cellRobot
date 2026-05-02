#pragma once
#include "parser.hpp"
#include <string>
#include <map>
#include "data.h"
#include "robot.h"
class Game {
    Data data;
    Robot robot;
    GameMap gameMap;
    yy::parser parser;
    std::string filename;
    public:
    //void printTable();
    Game(std::string str, std::string mappath) : robot(), gameMap(mappath, robot), data(), parser(data), filename(std::move(str)) {}
    void run();
};


