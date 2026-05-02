#pragma once
#include "baseNodes.h"
#include "robot.h"
class XrayNode : public VlueTypeNode {
    Robot &rb;
    GameMap &map;
    VarType t;
public:
    VarType getValType() override { return t; }
    void setValType(VarType tt) override { t =tt; }
    XrayNode(Robot &r, GameMap &m) : VlueTypeNode(), rb(r), map(m), t(VarType::MATRIX) {}
    Value proc() override {
        return Matrix(VarType::CELL, 3, 33);
    };
};

class MoveNode : public Node {

};