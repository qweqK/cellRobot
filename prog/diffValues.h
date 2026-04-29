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
    std::variant< int, unsigned int,Cell, Matrix> s;
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