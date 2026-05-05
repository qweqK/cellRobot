#pragma once
#include "baseNodes.h"
#include "robot.h"
class XrayNode : public VlueTypeNode {
    GameMap &map;
    VarType t;
public:
    VarType getValType() override { return t; }
    void setValType(VarType tt) override { t =tt; }
    XrayNode(GameMap &m) : map(m), t(VarType::MATRIX) {}
    Value proc() override {
        Matrix res(VarType::CELL, 5u, 5u);
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                int fX = map.rX-2 + j;
                int fY = map.rY-2 + i;
                if (fX <0 || fY <0 || fY>=map.rows || fX>=map.cols)  res(i, j) = Cell(true, true, true, true);
                else res(i, j) = map.grid(fY,fX);
            }
        }
        return res;
    };
};

class MoveNode : public Node {
    GameMap &map;
    DIRECT direct;
    public:
    MoveNode(DIRECT d, GameMap & m) : map(m), direct(d) {}

    Value proc() override {
        switch (direct) {
            case DIRECT::LEFT: {
                if (map.rX - 1 >= 0) {
                    if (std::get<Cell>(map.grid(map.rY, map.rX )).data[2] == false && std::get<Cell>(map.grid(map.rY, map.rX - 1)).data[3] == false) map.rX--;
                    else throw std::runtime_error("robot crushed in move left action");
                }  else throw std::runtime_error("robot range out in move left action");
            }break;
            case DIRECT::RIGHT: {
                if (map.rX + 1  < map.cols) {
                    if (std::get<Cell>(map.grid(map.rY, map.rX)).data[3] == false && std::get<Cell>(map.grid(map.rY, map.rX + 1)).data[2] == false) map.rX++;
                    else throw std::runtime_error("robot crushed in move right action");
                } else throw std::runtime_error("robot range out in move right action");
            }break;

            case DIRECT::UP: {
                if (map.rY - 1  >=  0) {
                    if (std::get<Cell>(map.grid(map.rY , map.rX)).data[0] == false && std::get<Cell>(map.grid(map.rY -1, map.rX)).data[1] == false) map.rY--;
                    else throw std::runtime_error("robot crushed in move up action");
                } else throw std::runtime_error("robot range out in move up action");
            }break;

            case DIRECT::DOWN: {if (map.rY + 1  < map.rows){
                if (std::get<Cell>(map.grid(map.rY, map.rX)).data[1] == false && std::get<Cell>(map.grid(map.rY +1, map.rX)).data[0] == false ) map.rY++;
                else throw std::runtime_error("robot crushed in move down action");
            } else throw std::runtime_error("robot range out in move down action");
            }break;
        }
        return std::monostate();
    }
};

class PrintMapNode : public Node {
    GameMap &map;
    public:
    PrintMapNode(GameMap & m) : map(m) {}
    Value proc() override {
        for (size_t i = 0; i < map.rows; i++) {
            for (size_t j = 0; j < map.cols; j++) {
                if (i == map.rY && j == map.rX) {std::cout<< "r";}
                else if (i == map.exitY && j == map.exitX) {std::cout<< "I";}
                else std::cout << std::get<Cell>(map.grid(i,j));
                std::cout << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        return std::monostate();
    }
};


class IsWinNode : public Node {
    Data &d;
    public:
    IsWinNode(Data & m) : d(m) {}
    Value proc() override {
        if (d.map.rX == d.map.exitX && d.map.rY == d.map.exitY) {
            std::cout<< "------------------win------------------" << std::endl;
            throw std::runtime_error("win");
        }
        return std::monostate();
    }
};


class ScreenNode : public Node {
    Data &d;
public:
    ScreenNode( Data &d) : d(d) {}
    Value proc() override {
        d.screens.push_back(d.map);
        return std::monostate();
    }
};