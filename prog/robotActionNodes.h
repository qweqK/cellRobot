#pragma once
#include "baseNodes.h"
#include "robot.h"
class XrayNode : public VlueTypeNode {
    GameMap &map;
    VarType t;
public:
    VarType getValType() override { return t; }
    void setValType(VarType tt) override { t =tt; }
    XrayNode(GameMap &m) : VlueTypeNode(), map(m), t(VarType::MATRIX) {}
    Value proc() override {

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
                if (map.rX - 1 > 0) {
                    if (std::get<Cell>(map.grid(map.rY, map.rX )).data[2] == false && std::get<Cell>(map.grid(map.rY, map.rX - 1)).data[3] == false) map.rX--;
                    else throw std::runtime_error("robot crushed in move left action");
                }
                break;
            }
            case DIRECT::RIGHT: {
                if (map.rX + 1  < map.cols -1) {
                    if (std::get<Cell>(map.grid(map.rY, map.rX)).data[3] == false && std::get<Cell>(map.grid(map.rY, map.rX + 1)).data[2] == false) map.rX++;
                    else throw std::runtime_error("robot crushed in move right action");
                }
            }break;

            case DIRECT::UP: {
                if (map.rY - 1  >  0) {
                    if (std::get<Cell>(map.grid(map.rY , map.rX)).data[0] == false && std::get<Cell>(map.grid(map.rY -1, map.rX)).data[1] == false) map.rX++;
                    else throw std::runtime_error("robot crushed in move up action");
                }
            }break;

            case DIRECT::DOWN: {if (map.rY + 1  < map.rows -1){
                if (std::get<Cell>(map.grid(map.rY, map.rX)).data[1] == false && std::get<Cell>(map.grid(map.rY, map.rX + 1)).data[0] == false ) map.rX++;
                else throw std::runtime_error("robot crushed in move down action");
            }
            }break;
        }
        if ()
        return std::monostate();
    }
};



