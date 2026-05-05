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
            auto gridd = j["grid"].get<std::vector<std::vector<bool>>>();
            grid = Matrix(VarType::CELL, rows, cols);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    auto c = gridd[i * cols + j];
                    grid(i, j).emplace<Cell>(c[0], c[1], c[2], c[3]);
                }
            }
        }
    }
};
