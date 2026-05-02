#pragma once
#include <castPrekols.h>
#include <baseNodes.h>

class ArithNode : public VlueTypeNode {
public:
    VarType t;
    VarType getValType() override{return t;}
    void setValType(VarType tt)override{ t = tt ;}
    std::unique_ptr<VlueTypeNode> left;
    std::unique_ptr<VlueTypeNode> right;
    ArithNode( std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn )  {
        if (leftn->getValType() == VarType::DEFAULT || rightn->getValType() == VarType::DEFAULT) {
            t= VarType::DEFAULT;
            left = std::move(leftn);
            right = std::move(rightn);
        }
        else {
            if (leftn->getValType() != rightn->getValType()) {
                if (leftn->getValType()== VarType::MATRIX || rightn->getValType()== VarType::MATRIX) { /* osibka */ }
                TypeCaster tk(leftn->getValType(), std::move(rightn));
                rightn= std::move(tk.cast());

            }
            t= leftn->getValType();
            left = std::move(leftn);
            right = std::move(rightn);
        }
    }
};

class MatrUnaryOper : public VlueTypeNode {
    std::unique_ptr<VlueTypeNode> expr;
    VarType t;
public:
    void setValType(VarType tt) override { t= tt;}
    VarType getValType() override {return t;};

    MatrUnaryOper(std::unique_ptr<VlueTypeNode> exprs): expr(std::move(exprs)), t(VarType::MATRIX) {
        if (expr->getValType() != VarType::MATRIX) {
            throw std::runtime_error("MatrUnaryOper: not a matrix");
        }
    }
    Value proc() override {
        return std::get<Matrix>(expr->proc().s).opersharp();
    };
};

class PlusNode : public ArithNode {
    public:
    PlusNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    Value proc() override {
        auto l = left->proc();
        auto r = right->proc();
        if (l.type != r.type) {l.runtimeCaster(r);}
        if (l.type == VarType::SIGNED) {
            return std::get<int>(l.s) + std::get<int>(r.s);
        }
        else if (l.type == VarType::UNSIGNED) {
            return std::get<unsigned int>(l.s) + std::get<unsigned int>(r.s);
        }
        else if (l.type == VarType::CELL) {
            return std::get<Cell>(l.s) + std::get<Cell>(r.s);
        }
        else if (l.type == VarType::MATRIX) {
            return std::get<Matrix>(l.s) + std::get<Matrix>(r.s);
        }
        return std::monostate();

    }

    void print() override{left->print();std::cout << "+"; right->print();}


};



class MinusNode : public ArithNode {

    public:
    MinusNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    Value proc() override {
        auto l = left->proc();
        auto r = right->proc();
        if (l.type != r.type) {l.runtimeCaster(r);}
        if (l.type == VarType::SIGNED) {
            return std::get<int>(l.s) - std::get<int>(r.s);
        }
        else if (l.type == VarType::UNSIGNED) {
            if (std::get<unsigned int>(l.s) < std::get<unsigned int>(r.s)) { throw std::runtime_error("overflow");}
            return std::get<unsigned int>(l.s) - std::get<unsigned int>(r.s);
        }
        else if (l.type == VarType::CELL) {
            return std::get<Cell>(l.s) - std::get<Cell>(r.s);
        }
        else if (l.type == VarType::MATRIX) {

        }
        return std::monostate();

    }
    void print() override{left->print();std::cout << "-"; right->print();}


};

class UminusNode : public VlueTypeNode {
    VarType t;
    VarType getValType() override {return  t;}
    void setValType(VarType tt) override { t = tt;}
    std::unique_ptr<VlueTypeNode> next;
    public:
    UminusNode(std::unique_ptr<VlueTypeNode>expr ) : t(VarType::SIGNED){
        if (expr->getValType() != VarType::SIGNED) {/*error*/}
        next = std::move(expr);
    }
    Value proc() override {
        return -std::get<int>(next->proc().s);
    }
    void print() override{std::cout<< "Umin("; next->print(); std::cout<< ")";}
};


class MulNode : public ArithNode {
public:

    MulNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    Value proc() override {
        auto l = left->proc();
        auto r = right->proc();
        if (l.type != r.type) {l.runtimeCaster(r);}
        if (l.type == VarType::SIGNED) {
            return std::get<int>(l.s) * std::get<int>(r.s);
        }
        else if (l.type == VarType::UNSIGNED) {
            return std::get<unsigned int>(l.s) * std::get<unsigned int>(r.s);
        }
        else if (l.type == VarType::CELL) {
            return std::get<Cell> (l.s) * std::get<Cell>(r.s);
        }
        else if (l.type == VarType::MATRIX) {

        }
        return std::monostate();
    }
    void print() override{left->print();std::cout << "*"; right->print();}

};

class DivNode : public ArithNode {
public:

    DivNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    Value proc() override {
        auto l = left->proc();
        auto r = right->proc();
        if (l.type != r.type) {l.runtimeCaster(r);}
        if (l.type == VarType::SIGNED) {
            if (std::get<int> (l.s) == 0) {/* error */ }
            else return std::get<int>(l.s) / std::get<int>(r.s);
        }
        else if (l.type == VarType::UNSIGNED) {
            if (std::get<unsigned int> (l.s) == 0) { }
            else return std::get<unsigned int>(l.s) / std::get<unsigned int>(r.s);
        }
        else if (l.type == VarType::CELL) {
            return std::get<Cell>(l.s) / std::get<Cell>(r.s);
        }
        else if (l.type == VarType::MATRIX) {
        }
        return std::monostate();
    }
    void print() override{left->print();std::cout << "/"; right->print();}

};

class DivModNode : public ArithNode {
public:

    DivModNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    Value proc() override {
        auto l = left->proc();
        auto r = right->proc();
        if (l.type != r.type) {l.runtimeCaster(r);}
        if (l.type == VarType::SIGNED) {
            return std::get<int>(l.s) % std::get<int>(r.s);
        }
        else if (l.type == VarType::UNSIGNED) {
            return std::get<unsigned int>(l.s) % std::get<unsigned int>(r.s);
        }
        else if (l.type == VarType::CELL) {
            return std::get<Cell>(l.s) % std::get<Cell>(r.s);
        }
        else if (l.type == VarType::MATRIX) {

        }
        return std::monostate();
    }
    void print() override{left->print();std::cout << "%"; right->print();}

};


class GreaterNode : public ArithNode {
public:
    GreaterNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {
         if (left->getValType() == VarType::MATRIX || left->getValType() == VarType::CELL || right->getValType() == VarType::MATRIX || right->getValType() == VarType::CELL) { throw std::invalid_argument("non right param");}
    }
    Value proc() override{
        auto l = left->proc();
        auto r = right->proc();
        if (l.type != r.type) {l.runtimeCaster(r); if (l.type == VarType::CELL) throw std::invalid_argument("fizhma");}
        if (l.type == VarType::SIGNED) {
            if (std::get<int>(l.s)> std::get<int>(r.s)) return 1;
            else return 0;
        }
        else if (l.type == VarType::UNSIGNED) {
            if (std::get<int>(l.s)> std::get<int>(r.s)) return 1u;
            else return 0u;
        }
        return std::monostate();

    };
    void print() override{left->print();std::cout << ">" << std::endl; right->print();}

    };

class LessNode : public ArithNode {
    public:

        LessNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {
         if (left->getValType() == VarType::MATRIX || left->getValType() == VarType::CELL || right->getValType() == VarType::MATRIX || right->getValType() == VarType::CELL) { throw std::invalid_argument("non right param");}
        }
        Value proc() override{
            auto l = left->proc();
            auto r = right->proc();
            if (l.type != r.type) {l.runtimeCaster(r); if (l.type == VarType::CELL) throw std::invalid_argument("fizhma");}
            if (l.type == VarType::SIGNED) {
                if (std::get<int>(l.s) < std::get<int>(r.s)) return 1;
                else return 0;
            }
            else if (l.type == VarType::UNSIGNED) {
                if (std::get<int>(l.s) < std::get<int>(r.s)) return 1u;
                else return 0u;
            }
            return std::monostate();

        };
        void print() override{left->print();std::cout << "<" << std::endl; right->print();}

    };

    class EqNode : public ArithNode {
    public:
        EqNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {
         if (left->getValType() == VarType::MATRIX || left->getValType() == VarType::CELL || right->getValType() == VarType::MATRIX || right->getValType() == VarType::CELL) { throw std::invalid_argument("non right param");}
        }
        Value proc() override{
            auto l = left->proc();
            auto r = right->proc();
            if (l.type != r.type) {l.runtimeCaster(r); if (l.type == VarType::CELL) throw std::invalid_argument("fizhma");}
            if (l.type == VarType::SIGNED) {
                if (std::get<int>(l.s) == std::get<int>(r.s)) return 1;
                else return 0;
            }
            else if (l.type == VarType::UNSIGNED) {
                if (std::get<int>(l.s) == std::get<int>(r.s)) return 1u;
                else return 0u;
            }
            return std::monostate();
        };

        void print() override {
            left->print();std::cout << "=" << std::endl; right->print();
        }
    };
