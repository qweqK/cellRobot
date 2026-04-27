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

enum class VarType {SIGNED=1, UNSIGNED=2, CELL=3, MATRIX=4};
enum class NodeType {VAR_NODE, OPER_NODE, CONST_NODE};
enum class DIRECT {UP=1, DOWN=2, LEFT=2, RIGHT=3};

class Robot {

};

class GameMap {

};


class Cell {
public:
    std::vector<bool> data;
    Cell operator +(Cell&r) {
        return {data[0] || r.data[0], data[1] || r.data[1], data[2] || r.data[2], data[3] || r.data[3]};
    }
    Cell operator -(Cell&r) {
        return {data[0]  != r.data[0], data[1] != r.data[1], data[2] != r.data[2], data[3] != r.data[3]};
    }
    Cell operator %(Cell&r) {
        return {data[0]  != r.data[0], data[1] != r.data[1], data[2] != r.data[2], data[3] != r.data[3]};
    }
    Cell operator /(Cell&r) {
        return {data[0]  != r.data[0], data[1] != r.data[1], data[2] != r.data[2], data[3] != r.data[3]};
    }
    Cell operator *(Cell &r) {
        return {data[0] && r.data[0], data[1] && r.data[1], data[2] && r.data[2], data[3] && r.data[3]};
    }

    Cell(bool f, bool s, bool t, bool fo) {
        data = {f, s, t, fo};
    }
    Cell() {
        data = {false, false, false, false};
    }
    friend std::ostream& operator << (std::ostream& out,  Cell &rhs) {
        if (rhs.data[0]) out <<" ___"<< std::endl;
        if (rhs.data[2]) out << "|";
        out <<  "   ";
        if (rhs.data[3]) out << "|" <<std::endl;
        if (rhs.data[1]) out << " ‾‾‾";
        return out;
    };
};


class Matrix {
public:
    VarType t;
    unsigned int rows;
    unsigned int cols;
    using val = std::variant<int, unsigned int, Cell>;
    std::vector<val> mat;
    Matrix(VarType t, unsigned int h, unsigned int v) : t(t), rows(h), cols(v) {
        if (t == VarType::SIGNED) {
            mat = std::vector<val>(rows * cols, 0);
        }
        else if (t == VarType::UNSIGNED) {
            mat = std::vector<val>(rows * cols, 0u);
        }
        else if (t == VarType::CELL) {
            mat = std::vector<val>(rows * cols, Cell());
        }

    };

    val& operator () (unsigned int r, unsigned int c) {
        return mat[r * cols + c];
    }

    Matrix operator +(Matrix&r) {
        return r;
    }
};


class Value {
    public:
    std::variant<std::monostate, int, unsigned int,Cell, Matrix> s;
    VarType type;
    Value(int i) : s(i), type(VarType::SIGNED){}
    Value(unsigned int i) : s(i), type(VarType::UNSIGNED){}
    Value(Cell c) : s(c), type(VarType::CELL) {}
    Value(Matrix m) : s(m), type(VarType::MATRIX){}
};

class SIT {
public:
    Value value=0;
    bool isConst = false;
};

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
            // case VarType::CELL: std::cout << std::get<Cell>(value.s); break;
            //case VarType::MATRIX: std::cout << std::get<Matrix>(value.s); break;
        }
    }
    LiterNode(Value  value, VarType tt) : value(std::move(value)), t(tt)  {}
    Value proc() override{return value;}
};




class SignedToUnsign : public VlueTypeNode {
    public:
    VarType t;
    VarType getValType() override{return t;}
    void setValType(VarType tt) override{ t = tt ;}
    std::unique_ptr<VlueTypeNode> next;
    SignedToUnsign(std::unique_ptr<VlueTypeNode> vn) {
        next = std::move(vn);
        t = VarType::UNSIGNED;
    };
    void print() override{ std::cout << "STU("; next->print(); std::cout << ")";}
    Value proc() override {
        auto r = std::get<int>(next->proc().s);
        if (r<0)/*osibka*/ return 0;
        unsigned int nr = static_cast<unsigned int>(r);
        return nr;
    }

};

class UnsignToSigned : public VlueTypeNode {
    public:
    VarType t;
    VarType getValType() override{return t;}
    void setValType(VarType tt) override{ t = tt ;}
    std::unique_ptr<VlueTypeNode> next;
    UnsignToSigned(std::unique_ptr<VlueTypeNode> vn) {
        next = std::move(vn);
        t = VarType::SIGNED;
    };
    void print() override{ std::cout << "UTS("; next->print(); std::cout << ")";}
    Value proc() override {
        auto r = std::get<unsigned int>(next->proc().s);
        // проверка, что многа
        int nr = static_cast<int>(r);
        return nr;
    }

};

class CellToSigned : public VlueTypeNode {
    public:
    VarType t;
    VarType getValType() override{return t;}
    void setValType(VarType tt) override{ t = tt ;}
    std::unique_ptr<VlueTypeNode> next;
    CellToSigned(std::unique_ptr<VlueTypeNode> vn) {
        next = std::move(vn);
        t = VarType::SIGNED;
    }
    void print() override{ std::cout << "CTS("; next->print(); std::cout << ")";}
    Value proc() override {
        auto r = std::get<Cell>(next->proc().s);
        int nr = r.data[0] && r.data[1] && r.data[2] && r.data[3];
        return nr;
    }

};

class CellToUnsigned : public VlueTypeNode {
public:
    VarType t;
    VarType getValType() override{return t;}
    void setValType(VarType tt) override{ t = tt ;}
    std::unique_ptr<VlueTypeNode> next;
    CellToUnsigned(std::unique_ptr<VlueTypeNode> vn) {
        next = std::move(vn);
        t = VarType::UNSIGNED;
    }

    void print() override{ std::cout << "CTU("; next->print(); std::cout << ")";}

    Value proc() override {
        auto r = std::get<Cell>(next->proc().s);
        unsigned int nr = r.data[0] && r.data[1] && r.data[2] && r.data[3];
        return nr;
    }

};



 class SignedToCell : public VlueTypeNode {
    public:
     VarType t;
     VarType getValType() override{return t;}
     void setValType(VarType tt)override{ t = tt ;}
    std::unique_ptr<VlueTypeNode> next;
    SignedToCell(std::unique_ptr<VlueTypeNode> vn) {
        t = VarType::CELL;
        next = std::move(vn);
    }
     void print() override{ std::cout << "STC("; next->print(); std::cout << ")";}
     Value proc() override {
        auto r = std::get<int>(next->proc().s);
        Cell nr;
        if (r != 0) {nr.data[0] = true; nr.data[1] = true; nr.data[2] = true; nr.data[3] = true;}
        return nr;
    }

};

class UnsignedToCell : public VlueTypeNode {
public:
    VarType t;
    VarType getValType() override{return t;}
    void setValType(VarType tt)override{ t = tt ;}
    std::unique_ptr<VlueTypeNode> next;
    UnsignedToCell(std::unique_ptr<VlueTypeNode> vn) {
        t = VarType::CELL;
        next = std::move(vn);
    }
    Value proc() override {
        auto r = std::get<unsigned int>(next->proc().s);
        Cell nr;
        if (r != 0) {nr.data[0] = true; nr.data[1] = true; nr.data[2] = true; nr.data[3] = true;}
        return nr;
    }
    void print() override{ std::cout << "UTC("; next->print(); std::cout << ")";}
};




class ArithNode : public VlueTypeNode {
    public:
    VarType t;
    VarType getValType() override{return t;}
    void setValType(VarType tt)override{ t = tt ;}
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    ArithNode( std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn ) {
        if (leftn->getValType() != rightn->getValType()) {
            if (leftn->getValType()== VarType::MATRIX || rightn->getValType()== VarType::MATRIX) {
                /*osibka*/
            }
            else if (leftn->getValType() == VarType::CELL && rightn->getValType() == VarType::SIGNED) {
                rightn = std::make_unique<SignedToCell>(std::move(rightn));
            }
            else if (leftn->getValType() == VarType::CELL && rightn->getValType() == VarType::UNSIGNED) {
                rightn = std::make_unique<UnsignedToCell>(std::move(rightn));
            }
            else if (leftn->getValType() == VarType::SIGNED && rightn->getValType() == VarType::UNSIGNED) {
                rightn = std::make_unique<UnsignToSigned>(std::move(rightn));
            }
            else if (leftn->getValType() == VarType::UNSIGNED && rightn->getValType() == VarType::SIGNED) {
                rightn = std::make_unique<SignedToUnsign>(std::move(rightn));
            }
            else if (leftn->getValType() == VarType::SIGNED && rightn->getValType() == VarType::CELL) {
                rightn = std::make_unique<CellToSigned>(std::move(rightn));
            }
            else if (leftn->getValType() == VarType::UNSIGNED && rightn->getValType() == VarType::CELL) {
                rightn = std::make_unique<CellToUnsigned>(std::move(rightn));
            }
        }
        t= leftn->getValType();
        left = std::move(leftn);
        right = std::move(rightn);
    }
};



class PlusNode : public ArithNode {
    public:
    PlusNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    Value proc() override {
        auto l = left->proc();
        auto r = right->proc();

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
    }

    void print() override{left->print();std::cout << "+"; right->print();}


};



class MinusNode : public ArithNode {

    public:
    MinusNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    Value proc() override {
        auto l = left->proc();
        auto r = right->proc();
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
    }
    void print() override{left->print();std::cout << "-"; right->print();}


};

class UminusNode : public VlueTypeNode {
    VarType t;
    VarType getValType() override {return  t;}
    void setValType(VarType tt) override { t = tt;}
    std::unique_ptr<VlueTypeNode> next;
    public:

    UminusNode(std::unique_ptr<VlueTypeNode>expr ) {
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
        if (l.type == VarType::SIGNED) {
            return std::get<int>(l.s) * std::get<int>(r.s);
        }
        else if (l.type == VarType::UNSIGNED) {
            return std::get<unsigned int>(l.s) * std::get<unsigned int>(r.s);
        }
        else if (l.type == VarType::CELL) {
            return std::get<Cell> (l.s) * std::get<Cell>(r.s);
        }
    }
    void print() override{left->print();std::cout << "*"; right->print();}

};

class DivNode : public ArithNode {
public:

    DivNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    Value proc() override {
        auto l = left->proc();
        auto r = right->proc();
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
    }
    void print() override{left->print();std::cout << "/"; right->print();}

};

class DivModNode : public ArithNode {
public:

    DivModNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    Value proc() override {
        auto l = left->proc();
        auto r = right->proc();
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
    }
    void print() override{left->print();std::cout << "%"; right->print();}

};


class GreaterNode : public ArithNode {
public:
    GreaterNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    void print() override{left->print();std::cout << ">" << std::endl; right->print();}

};

class LessNode : public ArithNode {
public:

    LessNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    void print() override{left->print();std::cout << "<" << std::endl; right->print();}

};

class EqNode : public ArithNode {
public:
    EqNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn) : ArithNode(std::move(leftn), std::move(rightn)) {}
    void print() override{left->print();std::cout << "=" << std::endl; right->print();}

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




class InitNode : public Node {
    std::unordered_map<std::string, SIT> &varst;
    VarType t;
    std::unique_ptr<VlueTypeNode> right;
    std::string name;
    bool isConst;

public:
    InitNode(std::string var, std::unique_ptr<VlueTypeNode> rightn,  std::unordered_map<std::string, SIT> &vars, VarType tt,bool isConst=false) : varst(vars), t(tt), name(std::move(var)), isConst(isConst) {
        if (varst.contains(name)) throw std::invalid_argument("Variable name already exists");
        if (rightn->getValType() == VarType::MATRIX || rightn->getValType() == VarType::CELL) {/*error*/}
        if ( t == VarType::SIGNED && rightn->getValType() != VarType::SIGNED) {
            rightn = std::make_unique<UnsignToSigned>(std::move(rightn));
            varst[name] = {0, isConst};
        }
        else if ( t == VarType::UNSIGNED && rightn -> getValType() != VarType::UNSIGNED) {

            rightn = std::make_unique<SignedToUnsign>(std::move(rightn));
        }

        if (rightn->getValType() == VarType::UNSIGNED) {
            varst[name] = {0u, isConst};
        }
        else if (rightn->getValType() == VarType::SIGNED) {
            varst[name] = {0, isConst};
        }

        right = std::move(rightn);
    }
    Value proc() override {
        auto r =  right->proc();
        varst[name] = {std::move(r), isConst};
        return 0;
    };
    void print() override{std::cout << name << "<-"; right->print();}

};




class InitCellNode : public Node {
    std::string name;
    std::unordered_map<std::string, SIT> &varst;
    VarType t;
    std::vector<bool> walls;
    bool isConst;



    public:

    InitCellNode(std::string s, std::vector<std::pair<bool, bool>> v, std::unordered_map<std::string, SIT> &vars,bool isConst=false ) : name(std::move(s)), varst(vars), t(VarType::CELL), isConst(isConst) {
        if (varst.contains(name)) throw std::invalid_argument("Variable name already exists");
        walls = {v[0].first, v[1].first, v[2].first, v[3].first};
        varst[name] = {Cell(walls[0], walls[1], walls[2], walls[3]), isConst};
    }
    Value proc() override {return 0;};
    void print() override {
        std::cout << name << "<-";
        std::cout<< walls[0] <<"|"<<walls[1]<< "|"<<walls[2] << "|" << walls[3];
    }


};

class InitMatrixNode : public Node {
    VarType t;
    unsigned int h;
    unsigned int v;
    std::unordered_map<std::string, SIT> &varst;
    std::string name;
public:
    InitMatrixNode(std::string s, unsigned int h,unsigned int v, VarType tt,std::unordered_map<std::string, SIT> &varstr ) : t(tt), h(h), v(v), varst(varstr), name(std::move(s)) {
        if (t == VarType::MATRIX) {throw std::invalid_argument("Matrix type not supported");}
        if (varst.contains(name)) throw std::invalid_argument("Variable name already exists");
        varst[name] = {Matrix(t, h, v)};
    }
    Value proc() override {
        return 0;
    }
};







class XrayNode : public VlueTypeNode {
    Robot &rb;
    GameMap &map;
    VarType t;
    public:
    VarType getValType() override { return t; }
    void setValType(VarType tt) override { t =tt; }
    XrayNode(Robot &r, GameMap &m) : VlueTypeNode(), rb(r), map(m), t(VarType::MATRIX) {}
    Value proc() override {
      return Matrix(VarType::CELL, 3, 33);
    };
};

class MoveNode : public Node {

};

class AssignNode : public Node {
    std::unique_ptr<VlueTypeNode> expr;
    std::unordered_map<std::string, SIT>& varst;
    std::string var;

public:
    AssignNode(std::string varr, std::unique_ptr<VlueTypeNode> exprs, std::unordered_map<std::string, SIT> &vars) : var(std::move(varr)), varst(vars){
        if (varst.contains(var)) {
            if (!varst[var].isConst) {
                expr = std::move(exprs);
            }
        }
        else {throw std::runtime_error("its const");}
    }
    Value proc() override {
        varst[var] = {expr->proc(), false}; return 0;
    }
    void print() override {std::cout << var<< " <- "; expr->print();}
};



class TestRepNode : public Node {
    std::unique_ptr<VlueTypeNode> left;
    std::unique_ptr<Node> right;
    public:
    TestRepNode(std::unique_ptr<VlueTypeNode>  leftn, std::unique_ptr<Node> rightn) : right(std::move(rightn)) {
        if (leftn->getValType() != VarType::UNSIGNED || leftn->getValType() != VarType::SIGNED) {/* error */}
        left =  std::move(leftn);
    }
    Value proc() override {
        while (std::get<int>(left->proc().s)) {right->proc();}
        return 0;
    }
    void print() override {std::cout << "while("; left->print(); std::cout << ") {" << std::endl; right->print(); std::cout <<  std::endl << "}" << std::endl;};

};



class TestOnceNode: public Node {
    std::unique_ptr<VlueTypeNode> left;
    std::unique_ptr<Node> right;
    public:
    TestOnceNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<Node> rightn) : right(std::move(rightn)) {
        if (leftn->getValType() != VarType::UNSIGNED || leftn->getValType() != VarType::SIGNED) {
            left = std::move(leftn);
        }
    }
    Value proc() override {
        if (std::get<int>(left->proc().s)) {right->proc();}
        return 0;
    }
    void print() override {std::cout << "if("; left->print(); std::cout << ") {" << std::endl; right->print(); std::cout << std::endl << "}" << std::endl;};

};

class CallFunctionNode : public Node {

};


class EmptyNode : public Node {
    Value proc() override {return 0;}
    void print() override {std::cout << "null" << std::endl;}
};




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
    Data() :  varstCur(std::make_unique<std::unordered_map<std::string, SIT>>()) {}

};




