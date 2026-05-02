#pragma once
#include "diffValues.h"
#include  "memory"
#include <unordered_map>
#include "data.h"

class VlueTypeNode : public Node {
public:
    virtual  VarType getValType() = 0;
    virtual  void setValType(VarType t) = 0;
};


class VarNode : public VlueTypeNode {
public:
    std::string id;
    Data &varst;
    VarType t;
    VarNode(std::string & str, Data &varstr ) : id(std::move(str)), varst(varstr) {
            auto a = varst.getBuildValue(id);
            t = a.value.type;
    }

    VarType getValType() override{return t;}
    void setValType(VarType tt) override{ t = tt ;}
    Value proc()override{return varst.callStack.top().getValue(id).value;};
    void print() override{  std::cout << id;}
};



class LiterNode : public VlueTypeNode {
public:
    VarType t;
    VarType getValType() override{return t;}
    void setValType(VarType tt) override{ t = tt ;}
    Value value;
    void print() override {
        switch (t) {
            case VarType::SIGNED:  std::cout<< std::get<int>(value.s); break;
            case VarType::UNSIGNED : std::cout << std::get<unsigned int>(value.s); break;
            case VarType::CELL: std::cout << std::get<Cell>(value.s); break;
                //case VarType::MATRIX: std::cout << std::get<Matrix>(value.s); break;
        }
    }
    LiterNode(Value  value, VarType tt) : value(std::move(value)), t(tt)  {}
    Value proc() override{return value;}
};

class EndSentNode : public Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
public:
    EndSentNode(std::unique_ptr<Node>  leftn, std::unique_ptr<Node> rightn ) : left(std::move(leftn)), right(std::move(rightn)) {}
    Value proc() override {
        left->proc(); return right->proc();
    }
    void print() override{left->print();std::cout << ";" << std::endl; right->print();}

};


class EmptyNode : public Node {
    Value proc() override {return 0;}
    void print() override {std::cout << "null" << std::endl;}
};

class BracketNode : public Node {
    std::unique_ptr<Node> next;
    Data &data;
public:
    BracketNode(std::unique_ptr<Node>  next, Data &data) : next(std::move(next)), data(data) {
        data.buildStorage.emplace_back();
    }
    Value proc() override {
        data.callStack.top().ts.emplace_back();
        next->proc();
        data.callStack.top().ts.pop_back();
        return std::monostate();
    }
    void  print() override {
        std::cout << "{"; next->print(); std::cout << "}" << std::endl;
    }
};

class PrintNode : public Node {
    std::string var;
    Data &data;
    public:
    PrintNode(std::string v, Data &data) : var(std::move(v)), data(data) {}
    Value proc() override {
        std::cout << data.callStack.top().ts.size() << " "  <<var << ": " <<data.callStack.top().getValue(var).value << std::endl;
        return std::monostate();
    }
    void print() override {
        std::cout << "print: " << var;
    }
};


