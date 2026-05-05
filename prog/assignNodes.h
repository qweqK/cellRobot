#pragma once
#include "castPrekols.h"


class AssignNode : public Node {
    std::unique_ptr<VlueTypeNode> expr;
    Data &varst;
    std::string var;
    VarType t;
    yy::location loc;

public:
    AssignNode(std::string varr, std::unique_ptr<VlueTypeNode> exprs, Data &d, yy::location l) : var(std::move(varr)), varst(d), loc(l){
        auto a = varst.getBuildValue(var, loc);
        t =a.value.type;
            if (!a.isConst) {
                if (a.value.type != exprs->getValType()) {
                    if (a.value.type == VarType::DEFAULT || exprs->getValType() == VarType::DEFAULT) {

                    }
                    else {
                        if (a.value.type == VarType::MATRIX || exprs->getValType() == VarType::MATRIX) { d.errorStatic(loc, "matrix typ not support");}
                        TypeCaster tk(a.value.type, std::move(exprs));
                        exprs = std::move(tk.cast());
                    }
                }
            } else {d.errorStatic(loc,"variable " + var + " const" );}
        expr = std::move(exprs);
    }

    Value proc() override {
        if (expr->getValType() == VarType::DEFAULT) {
            auto val = expr->proc();
            if (t != VarType::MATRIX) val.castSelf(t);
            else  val.castSelf(VarType::MATRIX, std::get<Matrix>(varst.callStack.top().getValue(var).value.s).t);
            varst.callStack.top().oldPut(var, val);
            return std::monostate();
        }
        else if (expr->getValType() == VarType::MATRIX) {
            auto val = expr->proc();
            if (std::get<Matrix>(varst.callStack.top().getValue(var).value.s).t != std::get<Matrix>(val.s).t) {
                std::cout << "qq";
                throw std::runtime_error("Matrix type dont match" + std::to_string(loc.begin.line));
            }
            varst.callStack.top().oldPut(var, expr->proc()); return  std::monostate();;
        }
        else  {
            varst.callStack.top().oldPut(var, expr->proc()); return  std::monostate();;
        }
    }
    void print() override {std::cout << var<< " <- "; expr->print();}
};





class MatrixAccesNode : public Node {
    std::string name;
    Data &varst;
    std::unique_ptr<VlueTypeNode> expr;
    std::unique_ptr<VlueTypeNode> left;
    std::unique_ptr<VlueTypeNode> right;
    yy::location loc;
public:

    MatrixAccesNode(std::string m, std::unique_ptr<VlueTypeNode> exprr, Data &d, std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn, yy::location l) : name(std::move(m)), varst(d), expr(std::move(exprr)), loc(l) {
        if (d.getBuildValue(name, loc).value.type != VarType::MATRIX) d.errorStatic(loc,"this var is not matrix");
        if (expr->getValType() == VarType::MATRIX) d.errorStatic(loc,"Matrix type not supported");
        if (leftn->getValType() != VarType::UNSIGNED) {TypeCaster tk(VarType::UNSIGNED,std::move(leftn)); leftn = std::move(tk.cast());}
        if (rightn->getValType() != VarType::UNSIGNED) {TypeCaster tk(VarType::UNSIGNED,std::move(rightn)); rightn = std::move(tk.cast());}
        left = std::move(leftn);
        right = std::move(rightn);
    }
    void print() override {
        std::cout << "matrix" << name << "("; left->print(); std::cout << ", "; right->print(); std::cout << ")" << "<-"; expr->print();
    }
    Value proc() override {
        auto & a = std::get<Matrix>(varst.callStack.top().getValue(name).value.s);
        const auto row = std::get<unsigned int>(left->proc().s);
        const auto col = std::get<unsigned int>(right->proc().s);
        auto val = expr->proc();
        if (a.rows <= row || a.cols <= col) {throw std::runtime_error("range_out: " + std::to_string(loc.begin.line));}
        val.castSelf(a.t);
        switch (a.t) {
            case VarType::CELL: a(row, col) =  get<Cell>(val.s); break;
            case VarType::SIGNED: a(row, col) =get<int>(val.s);break;
            case VarType::UNSIGNED: a(row, col) =  get<unsigned int>(val.s); break;
                default: std::monostate();;
        }
       return  std::monostate();;
    }

};

class MatrixAssignAccesNode : public VlueTypeNode {
    VarType t;
    std::string name;
    Data &varst;
    std::unique_ptr<VlueTypeNode> left;
    std::unique_ptr<VlueTypeNode> right;
    yy::location loc;
    public:
    MatrixAssignAccesNode(std::string m, Data &d, std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn, yy::location l) : name(std::move(m)), varst(d), loc(l){
        if (d.getBuildValue(name, loc).value.type != VarType::MATRIX) d.errorStatic(loc,"this var is not matrix");
        auto a = varst.getBuildValue(name, loc);
        t = std::get<Matrix>(a.value.s).t;
        if (leftn->getValType() != VarType::UNSIGNED) {TypeCaster tk(VarType::UNSIGNED,std::move(leftn)); leftn = std::move(tk.cast());}
        if (rightn->getValType() != VarType::UNSIGNED) {TypeCaster tk(VarType::UNSIGNED,std::move(rightn)); rightn = std::move(tk.cast());}
        left = std::move(leftn);
        right = std::move(rightn);
    }

    void print() override {
        std::cout << "matrix" << name << "("; left->print(); std::cout << ", "; right->print(); std::cout << ")";
    }


    VarType getValType() override{return  t;}
    void setValType(VarType tt) override{t = tt;}
    Value proc() override {
        auto & a = std::get<Matrix>(varst.callStack.top().getValue(name).value.s);
        const auto row = std::get<unsigned int>(left->proc().s);
        const auto col = std::get<unsigned int>(right->proc().s);
        if (a.cols <= row || a.rows <= col) throw std::runtime_error("mnogovata budet: "  + std::to_string(loc.begin.line));
        switch (t) {
            case VarType::SIGNED: return std::get<int>(a(row,col));
            case VarType::UNSIGNED: return std::get<unsigned int>(a(row,col));
            case VarType::MATRIX: return std::get<Cell>(a(row, col));
                default: std::monostate();
        }
        return std::monostate();
    }
};
