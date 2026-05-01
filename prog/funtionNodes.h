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
     CallFuncNode(std::string s, std::vector<std::string> argss, Data &dd) : name(std::move(s)), args(std::move(argss)), d(dd), t(VarType::DEFAULT) {}
    CallFuncNode(std::string s, Data &dd) : name(std::move(s)), args(), d(dd), t(VarType::DEFAULT) {}

     Value proc() override {
          if (!d.fStore.contains(name)) throw std::runtime_error("Func:: name not found");
          PARAMS p;
          for (auto s : args) {
              p.emplace_back( s,d.callStack.top().getValue(s));
          }
          Frame f(d.fStore[name].root.get(), d.fStore[name].params, d.varst.get());
          bool start = f.prepareToStart(p);
          if (!start) {throw std::runtime_error("Func:: prepareToStart failed");}
          d.callStack.emplace(f);
          auto fRes=  d.callStack.top().proc();
          d.callStack.pop();
          return fRes;
     };
    VarType getValType() override {return t;};
    void setValType(VarType tt)override { t = tt;}
};




