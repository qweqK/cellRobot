#pragma once
#include  "diffValues.h"
#include  <fstream>
#include "nlohmann/json.hpp"

class GameMap {
    int rX;
    int rY;
    int rows;
    int cols;
    int exit;
    Matrix map;

    public:
    GameMap(std::string path) {
            std::ifstream file(path);
            if (!file.is_open()) throw std::runtime_error("Can't open file " + path);
            nlohmann::json j;
            file >> j;
            auto rowsl = j["rows"].get<int>();
            auto colsl = j["cols"].get<int>();
            auto exit = j["exit"].get<int>();
            auto rXpos = j["rSpos"].get<int>();
            auto rYpos = j["rDpos"].get<int>();
            auto grid = j["grid"].get<std::vector<std::vector<bool>>>();
            Matrix maploc(VarType::CELL, rowsl, colsl);
            for (int i = 0; i < rowsl; i++) {
                for (int j = 0; j < colsl; j++) {
                        auto c = grid[i*colsl + j];
                        maploc(i, j).emplace<Cell>(c[0], c[1], c[2], c[3]);
                }
            }
            rows = rowsl;
            cols = colsl;
            map = std::move(maploc);
    }
};
