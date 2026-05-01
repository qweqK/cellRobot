#pragma once

#include <baseNodes.h>


class TestRepNode : public Node {
    std::unique_ptr<VlueTypeNode> left;
    std::unique_ptr<Node> right;
public:
    TestRepNode(std::unique_ptr<VlueTypeNode>  leftn, std::unique_ptr<Node> rightn) : right(std::move(rightn)) {
        if (leftn->getValType() == VarType::DEFAULT) {
            left = std::move(leftn);
        }
        else {
            if (leftn->getValType() == VarType::MATRIX) {/* error */}
            if (leftn->getValType() == VarType::CELL) {
                TypeCaster tk(VarType::SIGNED, std::move(leftn));;
                leftn = std::move(tk.cast());

            }
            left =  std::move(leftn);
        }
    }
    Value proc() override {
        if (left->getValType() == VarType::DEFAULT) {
            auto r = left->proc();
            r.castSelf(VarType::SIGNED);
            auto t = std::get<int>(r.s);
            while (t) {
                right->proc();
                r = left->proc();
                r.castSelf(VarType::SIGNED);
                t = std::get<int>(r.s);
            }
        }
        if(left->getValType() == VarType::SIGNED) {
            while (std::get<int>(left->proc().s)) {right->proc();}
        }
        else while (std::get<unsigned int>(right->proc().s)) {left->proc();}
        return std::monostate();
    }
    void print() override {std::cout << "while("; left->print(); std::cout << ")" << std::endl; right->print();};
};

class TestOnceNode: public Node {
    std::unique_ptr<VlueTypeNode> left;
    std::unique_ptr<Node> right;
public:
    TestOnceNode(std::unique_ptr<VlueTypeNode> leftn, std::unique_ptr<Node> rightn) : right(std::move(rightn)) {
        if (leftn->getValType() == VarType::DEFAULT) {
            left = std::move(leftn);
        }
        else {
            if (leftn->getValType() == VarType::MATRIX) {/* error */}
            if (leftn->getValType() == VarType::CELL) {
                TypeCaster tk(VarType::SIGNED, std::move(leftn));;
                leftn = std::move(tk.cast());
            }
            left =  std::move(leftn);
        }
    }

    Value proc() override {
        if (left->getValType() == VarType::SIGNED) {
            if (std::get<int>(left->proc().s)) {right->proc();}
        }
        else {
            if (std::get<unsigned int>(left->proc().s)) {right->proc();}
        }
        return std::monostate();
    }
    void print() override {std::cout << "if("; left->print(); std::cout << ")" << std::endl; right->print();}

};