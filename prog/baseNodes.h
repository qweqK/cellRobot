#pragma once
#include "diffValues.h"
#include  "memory"
#include <unordered_map>
class Node {
public:
    virtual void print() {std::cout << "wait the minute" << std::endl;};
    virtual ~Node() = default;
    virtual Value proc() {Value v = 0; return v;};

};


class VlueTypeNode : public Node {
public:
    virtual  VarType getValType() = 0;
    virtual  void setValType(VarType t) = 0;
};


class VarNode : public VlueTypeNode {
public:
    std::string id;
    std::unordered_map<std::string, SIT> &varst;
    VarType t;
    VarNode(std::string & str, std::unordered_map<std::string, SIT> &varstr ) : id(std::move(str)), varst(varstr), t(varst[id].value.type) {
        std::cout << varst[id].value.s.index()  << std::endl;
    }
    VarType getValType() override{return t;}
    void setValType(VarType tt) override{ t = tt ;}
    Value proc()override{return varst[id].value;};
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


