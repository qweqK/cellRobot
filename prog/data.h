#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include  <map>
#include <memory>
#include <stack>
#include <variant>
#include <vector>
#include "parser.hpp"
#include <unordered_map>
#include "diffValues.h"


class Node {
public:
    virtual void print() {std::cout << "wait the minute" << std::endl;};
    virtual ~Node() = default;
    virtual Value proc() {Value v = 0; return v;};

};



class TableSym {
public:
    using TSC = std::unordered_map<std::string, SIT>;
    TSC varstCur;
    TableSym * prev;
    TableSym() : prev(nullptr) {}
    TableSym(TableSym * prev) : prev(prev) {}
    SIT getValue(std::string varName) {
        for (TableSym * e = this; e->prev != nullptr; e = e->prev) {
            if (e->varstCur.contains(varName)) {
                return varstCur[varName];
            }
        }
        throw std::invalid_argument("Variable name not found");
    }
};

class Function {
    public:
    TSC params;
    std::unique_ptr<Node> root;
    Function(std::unique_ptr<Node> r, TSC p) : params(std::move(p)), root(std::move(r)) {}
    Function() = default;
    Function(Function &&) = default;
    Function &operator=(Function &&) = default;

};


class Frame {
    public:
    Node * root;
    TSC &params;
    std::vector<TSC> ts;
    TSC * global;
    SIT getValue(const std::string &varName) {
        for (auto it = ts.rbegin(); it != ts.rend(); ++it) {
            if (it->contains(varName)) {return (*it)[varName];}
        }
         if (global->contains(varName)) {return (*global)[varName];}
         throw std::runtime_error("Variable name not found kakogo?");
    }
    void newPut(const std::string &name, SIT s) {
        (*ts.rbegin())[name] = s;
    }
    void oldPut(const std::string &name, SIT s) {
        for (auto it = ts.rbegin(); it != ts.rend(); ++it) {
            if (it->contains(name)) {
                (*it)[name] = s;
            }
        }
        if (global->contains(name)) {
            (*global)[name] = s;
        }
    }
    Frame(Node *r, TSC &par, TSC *global) : root(r), params(par) ,global(global) {
        ts.emplace_back(params.begin(), params.end());

    }

};


    class Data {
    public:
        std::unique_ptr<TSC> varst;
        std::vector<TSC> buildStorage;
        std::stack<Frame> callStack;
        std::unordered_map<std::string, Function> fStore;
        SIT getBuildValue(std::string varName) {
            for (auto it = buildStorage.rbegin(); it != buildStorage.rend(); ++it) {
                if (it->contains(varName)) {return (*it)[varName];}
            }
            throw std::invalid_argument("Variable name not found");
        }
        void interpritation() {
            if (!fStore.contains("start")) throw std::invalid_argument("start not found");
            callStack.push({fStore["start"].root.get(),fStore["start"].params,  varst.get()});
            callStack.top().root->proc();
        }
        Data() = default;
    };


