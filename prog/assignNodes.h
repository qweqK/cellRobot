#pragma once
#include "castPrekols.h"


class AssignNode : public Node {
    std::unique_ptr<VlueTypeNode> expr;
    Data &varst;
    std::string var;
    VarType t;

public:
    AssignNode(std::string varr, std::unique_ptr<VlueTypeNode> exprs, Data &d) : var(std::move(varr)), varst(d){
        auto a = varst.getBuildValue(var);
        t =a.value.type;
            if (!a.isConst) {
                if (a.value.type != exprs->getValType()) {
                    if (a.value.type == VarType::DEFAULT || exprs->getValType() == VarType::DEFAULT) {

                    }
                    else {
                        if (a.value.type == VarType::MATRIX || exprs->getValType() == VarType::MATRIX) { throw std::invalid_argument("Matrix cast type not supported"); }
                        TypeCaster tk(a.value.type, std::move(exprs));
                        exprs = std::move(tk.cast());
                    }
                }
            } else {throw std::runtime_error("it const");}
        expr = std::move(exprs);
    }

    Value proc() override {
        if (expr->getValType() == VarType::DEFAULT) {
            auto val = expr->proc();
            val.castSelf(t);
            varst.callStack.top().oldPut(var, val);
            return std::monostate();
        }
        else {
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
public:

    MatrixAccesNode(std::string m, std::unique_ptr<VlueTypeNode> exprr, Data &d, std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : name(std::move(m)), varst(d), expr(std::move(exprr)) {
        if (d.getBuildValue(name).value.type != VarType::MATRIX) throw std::invalid_argument("this var is not matrix");
        if (expr->getValType() == VarType::MATRIX) throw std::invalid_argument("Matrix type not supported");
        if (leftn->getValType() != VarType::UNSIGNED) {TypeCaster tk(VarType::UNSIGNED,std::move(leftn)); leftn = std::move(tk.cast());}
        if (rightn->getValType() != VarType::UNSIGNED) {TypeCaster tk(VarType::UNSIGNED,std::move(rightn)); rightn = std::move(tk.cast());}
        left = std::move(leftn);
        right = std::move(rightn);
        std::cout <<"fill prekol" <<std::endl;
    }
    void print() override {
        std::cout << "matrix" << name << "("; left->print(); std::cout << ", "; right->print(); std::cout << ")" << "<-"; expr->print();
    }
    Value proc() override {
        auto & a = std::get<Matrix>(varst.callStack.top().getValue(name).value.s);
        const auto row = std::get<unsigned int>(left->proc().s);
        const auto col = std::get<unsigned int>(right->proc().s);
        auto val = expr->proc();
        if (a.cols < row || a.rows < col) {throw std::runtime_error("range_out");}
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
    public:
    MatrixAssignAccesNode(std::string m, Data &d, std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : name(std::move(m)), varst(d){
        if (d.getBuildValue(name).value.type != VarType::MATRIX) throw std::invalid_argument("this var is not matrix");
        auto a = varst.getBuildValue(name);
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
        if (a.cols < row || a.rows < col) throw std::runtime_error("bolse tem na");
        switch (t) {
            case VarType::SIGNED: return std::get<int>(a(row,col));
            case VarType::UNSIGNED: return std::get<unsigned int>(a(row,col));
            case VarType::MATRIX: return std::get<Cell>(a(row, col));
                default: std::monostate();
        }
        return std::monostate();
    }
};
