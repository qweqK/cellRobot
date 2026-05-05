#pragma once
#include <algorithm>
#include <iostream>
#include <location.hh>
#include <string>
#include  <map>
#include <memory>
#include <stack>
#include <variant>
#include <vector>
#include <unordered_map>
#include "diffValues.h"
#include "robot.h"
#include "position.hh"

class Node {
public:
    virtual void print() {std::cout << "wait the minute" << std::endl;};
    virtual ~Node() = default;
    virtual Value proc() {Value v = 0; return v;};

};


class Function {
    public:
    PARAMS params;
    std::unique_ptr<Node> root;
    int i;
    Function(std::unique_ptr<Node> r, PARAMS p) : params(std::move(p)), root(std::move(r)), i(params.size()) {}
    Function() = default;
    Function(Function &&) = default;
    Function &operator=(Function &&) = default;

};


class Frame {
    public:
    Node * root;
    PARAMS params;
    std::vector<TSC> ts;
    TSC * global;
    int i;

    SIT &getValue(const std::string &varName) {
        for (auto it = ts.rbegin(); it != ts.rend(); ++it) {
            if (it->contains(varName)) {return (*it)[varName];}
        }
         if (global && global->contains(varName)) {

             return (*global)[varName];
         }
         throw std::runtime_error("Variable name not found kakogo? c" + varName);
    }
    void newPut(const std::string &name, SIT s) {
        (*ts.rbegin())[name] = std::move(s);
    }
    void oldPut(const std::string &name, SIT s) {
        for (auto it = ts.rbegin(); it != ts.rend(); ++it) {
            if (it->contains(name)) {
                (*it)[name] = std::move(s);
            }
        }
        if (global && global->contains(name)) {
            (*global)[name] = std::move(s);
        }
    }
    Frame(Node *r,PARAMS par, TSC *global) : root(r), params(par) ,global(global) {
        ts.emplace_back();
    }

    Value proc() {
      auto a = root->proc();
        return a;
    }

    bool prepareToStart(PARAMS par){
        if (par.size() != params.size() ) {return false;}
        for (int i = 0; i < par.size(); i++) {
            if (par[i].second.value.type != params[i].second.value.type) {return false;}
            if (par[i].second.value.type == VarType::MATRIX) {
               if ( par[i].second.value.s.index() != params[i].second.value.s.index()) return false;
            }
        }

        for (int i = 0; i < par.size(); i++) {
            auto bir  = par[i].second;
            ts.back().emplace(params[i].first,bir);
        }

        return true;
    }

};


    class Data {
    public:
        GameMap &map;
        std::unique_ptr<TSC> varst;
        std::vector<TSC> buildStorage;
        std::stack<Frame> callStack;
        std::unordered_map<std::string, Function> fStore;
        std::vector<GameMap> screens;
        int errorCounter = 0;

        Data(GameMap & m) : map(m) {buildStorage.emplace_back(); varst = std::make_unique<TSC>();}
        SIT getBuildValue(const std::string& varName, yy::location loc) {
            for (auto it = buildStorage.rbegin(); it != buildStorage.rend(); ++it) {
                if (it->contains(varName)) {return (*it)[varName];}
            }
            errorStatic(loc,"Variable name not found: " + varName);
            return {std::monostate()};
        }
        void addBuildParams(PARAMS pr) {
            for (auto &a : pr) {
                buildStorage.back().emplace(a);

            }
        }
        bool chekIsExist(std::string varName) {
            for (auto it = buildStorage.rbegin(); it != buildStorage.rend(); ++it) {
                if (it->contains(varName)) {return true;}
            }
            return false;
        }
        void interpritation() {
            if (!fStore.contains("start")) throw std::invalid_argument("start not found");
            callStack.emplace(fStore["start"].root.get(),fStore["start"].params,  varst.get());
            callStack.top().root->proc();
        }
        void errorStatic(const yy::location& l, const std::string &msg) {
            errorCounter += 1;
            std::cerr << msg << " at line: " << l << std::endl;
        }
        Data() = default;



    };


