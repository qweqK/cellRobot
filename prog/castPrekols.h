#pragma once

#include "baseNodes.h"



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


class TypeCaster {
    VarType t;
    std::unique_ptr<VlueTypeNode> r;

public:
    TypeCaster(VarType tt, std::unique_ptr<VlueTypeNode> expr) :t(tt), r(std::move(expr)){}

    std::unique_ptr<VlueTypeNode> cast() {
        if (t == VarType::CELL && r->getValType() == VarType::SIGNED) {
            r = std::make_unique<SignedToCell>(std::move(r));
        }
        else if (t == VarType::CELL && r->getValType() == VarType::UNSIGNED) {
            r = std::make_unique<UnsignedToCell>(std::move(r));
        }
        else if (t == VarType::SIGNED && r->getValType() == VarType::UNSIGNED) {
            r = std::make_unique<UnsignToSigned>(std::move(r));
        }
        else if (t == VarType::UNSIGNED && r->getValType() == VarType::SIGNED) {
            r = std::make_unique<SignedToUnsign>(std::move(r));
        }
        else if (t == VarType::SIGNED && r->getValType() == VarType::CELL) {
            r = std::make_unique<CellToSigned>(std::move(r));
        }
        else if (t == VarType::UNSIGNED && r->getValType() == VarType::CELL) {
            r = std::make_unique<CellToUnsigned>(std::move(r));
        }
        return std::move(r);
    };

};





