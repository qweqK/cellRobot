#pragma once
#include "parser.hpp"
#include <string>
#include <map>
#include "data.h"
#include "robot.h"
#include "visualiser.h"


class Game {
    Data data;
    GameMap gameMap;
    yy::parser parser;
    std::string filename;
    std::unique_ptr<Visualiser> visualiser= nullptr;
    std::string pathGaphic ="";
    bool isGraphic;
    public:
    //void printTable();
    Game(std::string str, std::string mappath= "") :  gameMap(mappath), data(gameMap), parser(data), filename(std::move(str)), isGraphic(false) {}
    Game(std::string str, std::string mappath, std::string pathGraphic) :  gameMap(mappath), data(gameMap), parser(data), filename(std::move(str)), isGraphic(true), pathGaphic(std::move(pathGraphic))  {}

    void run();
};


