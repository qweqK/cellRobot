#pragma once

#include <map>

#include "baseNodes.h"
#include "data.h"
#include "diffValues.h"

class CallFuncNode : public VlueTypeNode {
     std::string name;
     std::vector<std::string> args;
     Data & d;
     VarType t;


     public:
     CallFuncNode(std::string &s, std::vector<std::string> argss, Data &dd) : name(s), args(argss), d(dd), t(VarType::DEFAULT) {}
     Value proc() override {

     };

};



class GroupSentNode : public Node {
     std::unique_ptr<Node> group;
     GroupSentNode(std::unique_ptr<Node> n) : group(std::move(n)) {}
     Value proc() override {

     }
     S

};

