#pragma once
#include  "diffValues.h"
#include  <fstream>
#include "nlohmann/json.hpp"

class GameMap {
public:
    int rX=0;
    int rY=0;
    int rows=0;
    int cols=0;
    int exitX=1;
    int exitY =1;
    Matrix grid;
    GameMap(std::string path = "") : grid(VarType::CELL, 0u, 0u) {
        if (path != "") {
            std::ifstream file(path);
            if (!file.is_open()) throw std::runtime_error("Can't open file " + path);
            nlohmann::json j;
            file >> j;
            rows = j["rows"].get<int>();
            cols = j["cols"].get<int>();
            exitX  = j["exitX"].get<int>();
            exitY  = j["exitY"].get<int>();
            rX = j["rXpos"].get<int>();
            rY = j["rYpos"].get<int>();
            auto gridd = j["grid"].get<std::vector<std::string>>();
            grid = Matrix(VarType::CELL, rows, cols);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    auto c = gridd[i * cols + j];
                    bool br[4] = {false, false, false, false};
                    for (auto s : c) {
                        if (s == 't') br[0] = true;
                        else if (s == 'd') br[1] = true;
                        else if (s == 'l') br[2] = true;
                        else if (s == 'r') br[3] = true;
                    }
                    grid(i, j).emplace<Cell>(br[0], br[1], br[2], br[3]);
                }
            }
        }
    }
};
