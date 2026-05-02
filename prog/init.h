#pragma once
#include "castPrekols.h"


 class InitNode : public Node {
      Data &varst;
        VarType t;
        std::unique_ptr<VlueTypeNode> right;
        std::string name;
        bool isConst;


    public:
        InitNode(std::string var, std::unique_ptr<VlueTypeNode> rightn,  Data &d, VarType tt,bool isConst=false) : varst(d), t(tt), name(std::move(var)), isConst(isConst) {
            if (varst.buildStorage.back().contains(name)) throw std::invalid_argument("Variable name already exists");
            if (rightn->getValType()==VarType::MATRIX) throw std::invalid_argument("Matrix type not supported");
            if (t != rightn->getValType()) {
                TypeCaster tk(t,std::move(rightn));
                rightn = std::move(tk.cast());
            }
            right = std::move(rightn);
            if (t == VarType::UNSIGNED) {
                d.buildStorage.back()[name] = {0u, isConst};
            }
            else if (t==VarType::SIGNED) {
                d.buildStorage.back()[name] = {0, isConst};
            }
        }
        Value proc() override {
            auto r =  right->proc();
            if (right->getValType()==VarType::DEFAULT) {
                r.castSelf(t);
            }
            varst.callStack.top().newPut(name, {r,isConst});
            return std::monostate();
        };
        void print() override{std::cout << name << "<-"; right->print();}

    };




    class InitCellNode : public Node {
        std::string name;
        Data & varst;
        VarType t;
        std::vector<bool> walls;
        bool isConst;



    public:

        InitCellNode(std::string s, std::vector<std::pair<bool, bool>> v, Data &d,bool isConst=false ) : name(std::move(s)), varst(d), t(VarType::CELL), isConst(isConst) {
            if (varst.buildStorage.back().contains(name)) throw std::invalid_argument("Variable name already exists");
            walls = {v[0].first, v[1].first, v[2].first, v[3].first};
            (*varst.buildStorage.rbegin())[name] = {Cell(walls[0], walls[1], walls[2], walls[3]), isConst};
        }
        Value proc() override {
            varst.callStack.top().newPut(name, {{walls[0], walls[1], walls[2], walls[3]}, isConst});
            return std::monostate();
        }
        void print() override {
            std::cout << name << "<-";
            std::cout<< walls[0] <<"|"<<walls[1]<< "|"<<walls[2] << "|" << walls[3];
        }


    };

    class InitMatrixNode : public Node {
        VarType t;
        std::unique_ptr<VlueTypeNode> left;
        std::unique_ptr<VlueTypeNode> right;
        Data & varst;
        std::string name;
    public:
        InitMatrixNode(std::string s, std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn, VarType tt,Data &d ) : t(tt), varst(d), name(std::move(s)) {
            if (t == VarType::MATRIX) {throw std::invalid_argument("Matrix type not supported");}
            if (varst.buildStorage.back().contains(name)) throw std::invalid_argument("Variable name already exists");
            if (leftn->getValType() != VarType::UNSIGNED) {TypeCaster tk(VarType::UNSIGNED,std::move(leftn)); leftn = std::move(tk.cast());}
            if (rightn->getValType() != VarType::UNSIGNED) {TypeCaster tk(VarType::UNSIGNED,std::move(rightn)); rightn = std::move(tk.cast());}
            left = std::move(leftn);
            right = std::move(rightn);
            (*varst.buildStorage.rbegin())[name] = {Matrix(t, 0u, 0u), false};
        }
        Value proc() override {
            varst.callStack.top().newPut(name, {Matrix(t, std::get<unsigned int>(left->proc().s), std::get<unsigned int>(right->proc().s)), false});
            return std::monostate();
        }
        void print() override {
            std::cout << name << "<-" << "matrix" << std::endl;
        }
    };

class InitEmptyNode : public Node {
    Data & varst;
    VarType t;
    std::string name;
    bool isConst;
    public:
    InitEmptyNode(std::string s, VarType tt, Data &d, bool c= false) : t(tt), varst(d), name(std::move(s)), isConst(c) {
        if (t == VarType::MATRIX) throw std::invalid_argument("Matrix type not supported");
        if (d.buildStorage.back().contains(name)) throw std::invalid_argument("Variable name already exists");
        if (t == VarType::UNSIGNED) {
            d.buildStorage.back()[name] = {0u, isConst};
        }
        else if (t==VarType::SIGNED) {
            d.buildStorage.back()[name] = {0, isConst};
        }
        else if (t  == VarType::CELL) {
            d.buildStorage.back()[name] = {Cell(), isConst};
        }
    }
    Value proc() override {

        varst.callStack.top().newPut(name, {Value(0u),isConst});
        switch (t) {
            case VarType::UNSIGNED:  varst.callStack.top().newPut(name, {Value(0u),isConst}); break;
            case VarType::CELL: varst.callStack.top().newPut(name, {Cell(),isConst}); break;
            case VarType::SIGNED: varst.callStack.top().newPut(name, {Value(0),isConst}); break;
                default: throw std::invalid_argument("Unsupported type");
        }
        return std::monostate();
    }

};


class InitEmptyMatrixNode : public Node {
    VarType t;
    std::string name;
    Data & varst;
public:
    InitEmptyMatrixNode(std::string n, VarType tt, Data &d) : t(tt), varst(d), name(std::move(n)) {
        if (t == VarType::MATRIX) {throw std::invalid_argument("Matrix type not supported");}
        if (varst.buildStorage.back().contains(name)) {throw std::invalid_argument("Variable name already exists");}
        (*varst.buildStorage.rbegin())[name] = {Matrix(t, 0u, 0u), false};
    }
    Value proc() override {
        varst.callStack.top().newPut(name, {Matrix(t, 0u, 0u), false});
        return std::monostate();
    }



};

