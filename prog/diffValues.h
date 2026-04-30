#pragma once
#include <vector>
#include <variant>
#include <iostream>

enum class VarType {DEFAULT = 0,SIGNED=1, UNSIGNED=2, CELL=3, MATRIX=4};
enum class NodeType {VAR_NODE, OPER_NODE, CONST_NODE};
enum class DIRECT {UP=1, DOWN=2, LEFT=2, RIGHT=3};


class Robot {

};

class GameMap {

};


class Cell {
public:
    //std::vector<bool> data;
    int data[4];
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
        //data = {f, s, t, fo};
        data[0] = f; data[1] = s; data[2] = t; data[3] = fo;
    }
    Cell() {
       // data = {false, false, false, false};
        data[0] = false; data[1] = false; data[2]= false; data[3] = false;
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

    friend std::ostream& operator << (std::ostream& out,  Matrix &rhs) {
        out << std::endl;
        if (rhs.t == VarType::SIGNED) {
            for (size_t i = 0; i < rhs.rows; i++) {
                for (size_t j = 0; j < rhs.cols; j++) {
                    out << std::get<int>(rhs(i,j));
                }
                out << std::endl;
            }
        }
        else if (rhs.t == VarType::UNSIGNED) {
            for (size_t i = 0; i < rhs.rows; i++) {
                for (size_t j = 0; j < rhs.cols; j++) {
                    out << std::get<unsigned int>(rhs(i,j));
                }
                out << std::endl;
            }
        }
        else if (rhs.t == VarType::CELL) {
            for (size_t i = 0; i < rhs.rows; i++) {
                for (size_t j = 0; j < rhs.cols; j++) {
                    out << std::get<Cell>(rhs(i,j));
                }
                out << std::endl;
            }
        }
        return out;
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
    Value(std::monostate s) : s(s), type(VarType::DEFAULT){}
    void runtimeCaster(Value &r) {
        if (r.type == VarType::MATRIX || type == VarType::MATRIX)  {throw std::runtime_error("runtime caster error");}
        else if (type == VarType::CELL && r.type == VarType::SIGNED) {
            auto rr = std::get<int>(r.s);
            Cell nr;
            if (rr != 0) {nr.data[0] = true; nr.data[1] = true; nr.data[2] = true; nr.data[3] = true;}
            r = std::move(nr);
            r.type = VarType::CELL;
        }
        else if (type == VarType::CELL && r.type == VarType::UNSIGNED) {
            auto rr = std::get<unsigned int>(r.s);
            Cell nr;
            if (rr != 0) {nr.data[0] = true; nr.data[1] = true; nr.data[2] = true; nr.data[3] = true;}
            r = std::move(nr);
            r.type = VarType::CELL;
        }
        else if (type == VarType::SIGNED &&  r.type == VarType::UNSIGNED) {
            auto rr = std::get<unsigned int>(r.s);
            // проверка, что многа
            int nr = static_cast<int>(rr);
            r = std::move(nr);
            r.type = VarType::SIGNED;
        }
        else if (type == VarType::UNSIGNED &&  r.type == VarType::SIGNED) {
            auto rr = std::get<int>(r.s);
            if (rr<0)/*osibka*/ throw std::runtime_error("runtime caster error");
            unsigned int nr = static_cast<unsigned int>(rr);
            r = std::move(nr);
            r.type = VarType::UNSIGNED;
        }
        else if (type == VarType::SIGNED &&  r.type == VarType::CELL) {
            auto rr = std::get<Cell>(r.s);;
            int nr = rr.data[0] && rr.data[1] && rr.data[2] && rr.data[3];
            r = std::move(nr);
            r.type = VarType::SIGNED;
        }
        else if (type == VarType::UNSIGNED && r.type == VarType::CELL) {
            auto rr = std::get<Cell>(r.s);;
            unsigned int nr = rr.data[0] && rr.data[1] && rr.data[2] && rr.data[3];
            r = std::move(nr);
            r.type = VarType::UNSIGNED;
        }
    };
    void castSelf(VarType t) {
        if (t == VarType::MATRIX || type == VarType::MATRIX)  {throw std::runtime_error("runtime caster error");}
        else if (t == VarType::CELL && type == VarType::SIGNED) {
            auto rr = std::get<int>(s);
            Cell nr;
            if (rr != 0) {nr.data[0] = true; nr.data[1] = true; nr.data[2] = true; nr.data[3] = true;}
            s = std::move(nr);
            type = VarType::CELL;
        }
        else if (t == VarType::CELL && type == VarType::UNSIGNED) {
            auto rr = std::get<unsigned int>(s);
            Cell nr;
            if (rr != 0) {nr.data[0] = true; nr.data[1] = true; nr.data[2] = true; nr.data[3] = true;}
            s = std::move(nr);
            type = VarType::CELL;
        }
        else if (t == VarType::SIGNED &&  type == VarType::UNSIGNED) {
            auto rr = std::get<unsigned int>(s);
            // проверка, что многа
            int nr = static_cast<int>(rr);
            s = std::move(nr);
            type = VarType::SIGNED;
        }
        else if (t == VarType::UNSIGNED &&  type == VarType::SIGNED) {
            auto rr = std::get<int>(s);
            if (rr<0)/*osibka*/ throw std::runtime_error("runtime caster error");
            unsigned int nr = static_cast<unsigned int>(rr);
            s = std::move(nr);
            type = VarType::UNSIGNED;
        }
        else if (t == VarType::SIGNED &&  type == VarType::CELL) {
            auto rr = std::get<Cell>(s);;
            int nr = rr.data[0] && rr.data[1] && rr.data[2] && rr.data[3];
            s = std::move(nr);
            type = VarType::SIGNED;
        }
        else if (t == VarType::UNSIGNED && type == VarType::CELL) {
            auto rr = std::get<Cell>(s);;
            unsigned int nr = rr.data[0] && rr.data[1] && rr.data[2] && rr.data[3];
            s = std::move(nr);
            type = VarType::UNSIGNED;
        }
    }
};


class SIT {
public:
    Value value;
    bool isConst;
    SIT(int i, bool isC = false) : value(i), isConst(isC){}
    SIT(unsigned int u, bool isC=false) : value(u), isConst(isC){}
    SIT(Cell c, bool isC=false) : value(c), isConst(isC){}
    SIT(Matrix m, bool isC=false) : value(m), isConst(isC) {}
    SIT() : value(std::monostate()), isConst(false) {}
};

using TSC = std::unordered_map<std::string, SIT>;
