#pragma once
#include "castPrekols.h"

class DefInitNode : public Node {
public:
   virtual std::string getName() =0;
    virtual SIT getRightValue() =0;
};

 class InitNode : public DefInitNode {
      Data &varst;
        VarType t;
        std::unique_ptr<VlueTypeNode> right;
        std::string name;
        bool isConst;
        yy::location loc;

    public:
        std::string getName() override {return name;}
        InitNode(std::string var, std::unique_ptr<VlueTypeNode> rightn,  Data &d, VarType tt,yy::location l ,bool isConst=false) : varst(d), t(tt), name(std::move(var)), isConst(isConst), loc(l) {
            if (varst.buildStorage.back().contains(name)) d.errorStatic(loc, "var already exist");
            if (rightn->getValType()==VarType::MATRIX) d.errorStatic(loc, "Matrix type not supported");
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
        SIT getRightValue() override {
            auto r =  right->proc();
            if (right->getValType()==VarType::DEFAULT) {
                r.castSelf(t);
            }
            return {r, isConst};
        };
    };




    class InitCellNode : public DefInitNode {
        std::string name;
        Data & varst;
        VarType t;
        std::vector<bool> walls;
        bool isConst;
        yy::location loc;



    public:
        std::string getName() override{return name;}
        InitCellNode(std::string s, std::vector<std::pair<bool, bool>> v, Data &d,yy::location l,bool isConst=false ) : name(std::move(s)), varst(d), t(VarType::CELL), isConst(isConst), loc(l) {
            if (varst.buildStorage.back().contains(name)) d.errorStatic(loc,"Variable name already exists");
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
        SIT getRightValue() override {
            return {{walls[0], walls[1], walls[2], walls[3]}, isConst};
        };

    };

    class InitMatrixNode : public DefInitNode {
        VarType t;
        std::unique_ptr<VlueTypeNode> left;
        std::unique_ptr<VlueTypeNode> right;
        Data & varst;
        std::string name;
        yy::location loc;
    public:
        std::string getName() override {return name;}
        InitMatrixNode(std::string s, std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn, VarType tt,Data &d, yy::location l ) : t(tt), varst(d), name(std::move(s)), loc(l) {
            if (t == VarType::MATRIX) {d.errorStatic(loc,"Matrix type not supported");}
            if (varst.buildStorage.back().contains(name)) d.errorStatic(loc,"Variable name already exists");
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
        SIT getRightValue() override {
            return {Matrix(t, std::get<unsigned int>(left->proc().s), std::get<unsigned int>(right->proc().s)), false};
        };
    };

class InitEmptyNode : public DefInitNode {
    Data & varst;
    VarType t;
    std::string name;
    bool isConst;
    yy::location loc;
    public:
    std::string getName() override{return name;}
    InitEmptyNode(std::string s, VarType tt, Data &d, yy::location l ,bool c= false) : t(tt), varst(d), name(std::move(s)), isConst(c), loc(l) {
        if (t == VarType::MATRIX) d.errorStatic(loc,"Matrix type not supported");
        if (d.buildStorage.back().contains(name)) d.errorStatic(loc,"Variable name already exists");
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
    SIT getRightValue() override {
        switch (t) {
            case VarType::UNSIGNED:  return{Value(0u),isConst}; break;
            case VarType::CELL: return{Cell(),isConst}; break;
            case VarType::SIGNED: return{Value(0),isConst}; break;
            default: throw std::invalid_argument("Unsupported type");
        }
    };

};


class InitEmptyMatrixNode : public DefInitNode {
    VarType t;
    std::string name;
    Data & varst;
    yy::location loc;
public:
    std::string getName() override{return name;}
    InitEmptyMatrixNode(std::string n, VarType tt, Data &d, yy::location l) : t(tt), varst(d), name(std::move(n)), loc(l) {
        if (t == VarType::MATRIX) {d.errorStatic(loc,"Matrix type not supported");}
        if (varst.buildStorage.back().contains(name)) {d.errorStatic(loc,"Variable name already exists");}
        (*varst.buildStorage.rbegin())[name] = {Matrix(t, 0u, 0u), false};
    }
    Value proc() override {
        varst.callStack.top().newPut(name, {Matrix(t, 0u, 0u), false});
        return std::monostate();
    }
    SIT getRightValue() override {
        return {Matrix(t, 0u, 0u), false};
    };



};

