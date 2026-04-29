#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include  <map>
#include <memory>
#include <variant>
#include <vector>
#include "parser.hpp"
#include <unordered_map>
#include  "arithNodes.h"




class Function {
    public:
        std::unique_ptr<std::unordered_map<std::string, SIT>> varst;
        std::unique_ptr<Node> root;
        Function() = default;
        Function(std::unique_ptr<std::unordered_map<std::string, SIT>> vars, std::unique_ptr<Node> r) : varst(std::move(vars)), root(std::move(r)) {}
};

    class Data {
    public:

        std::unordered_map<std::string, Function> functions;
        std::unique_ptr<std::unordered_map<std::string, SIT>> varstCur;
        //std::stack<Function> callStack;
        Data() :  varstCur(std::make_unique<std::unordered_map<std::string, SIT>>()) {}

    };

