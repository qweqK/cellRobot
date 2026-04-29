#pragma once
#include "castPrekols.h"


 class InitNode : public Node {
        std::unordered_map<std::string, SIT> &varst;
        VarType t;
        std::unique_ptr<VlueTypeNode> right;
        std::string name;
        bool isConst;

    public:
        InitNode(std::string var, std::unique_ptr<VlueTypeNode> rightn,  std::unordered_map<std::string, SIT> &vars, VarType tt,bool isConst=false) : varst(vars), t(tt), name(std::move(var)), isConst(isConst) {
            if (varst.contains(name)) throw std::invalid_argument("Variable name already exists");
            if (rightn->getValType() == VarType::MATRIX || rightn->getValType() == VarType::CELL) {throw std::invalid_argument("Matrix type not supported");}
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
        std::unique_ptr<VlueTypeNode> left;
        std::unique_ptr<VlueTypeNode> right;
        std::unordered_map<std::string, SIT> &varst;
        std::string name;
    public:
        InitMatrixNode(std::string s, std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<VlueTypeNode> rightn, VarType tt,std::unordered_map<std::string, SIT> &varstr ) : t(tt), varst(varstr), name(std::move(s)) {
            if (t == VarType::MATRIX) {throw std::invalid_argument("Matrix type not supported");}
            if (varst.contains(name)) throw std::invalid_argument("Variable name already exists");
            if (leftn->getValType() != VarType::UNSIGNED) {TypeCaster tk(VarType::UNSIGNED,std::move(leftn)); leftn = std::move(tk.cast());}
            if (rightn->getValType() != VarType::UNSIGNED) {TypeCaster tk(VarType::UNSIGNED,std::move(rightn)); rightn = std::move(tk.cast());}
            left = std::move(leftn);
            right = std::move(rightn);
            varst[name] = {Matrix(t, 0, 0), false};
        }
        Value proc() override {
            varst[name] = {Matrix(t, std::get<unsigned int>(left->proc().s), std::get<unsigned int>(right->proc().s))};
            return 0;
        }
    };

