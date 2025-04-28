#ifndef EXPR_H
#define EXPR_H

#include "pointer.h"
#include "val.h"
#include "env.h"
#include <string>
#include <iostream>
#include <memory>

typedef enum {
    prec_none,
    prec_eq,
    prec_add,
    prec_mult
} precedence_t;

CLASS(Expr) {
public:
    virtual ~Expr() = default;
    virtual bool equals(PTR(Expr) e) = 0;
    virtual PTR(Val) interp(PTR(Env) env) = 0;
    virtual void printExp(std::ostream &os) = 0;
    virtual void pretty_print(std::ostream &os, precedence_t prec, std::streampos& lastIndent) = 0;
    virtual bool is_simple() const { return false; }
    virtual std::string to_string();
    std::string to_pretty_string();
};

class NumExpr : public Expr {
public:
    int64_t val;
    NumExpr(int64_t val);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    void printExp(std::ostream &os) override;
    bool is_simple() const override { return true; }
    void pretty_print(std::ostream &os, precedence_t prec, std::streampos& lastIndent) override;
};

class AddExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    AddExpr(PTR(Expr), PTR(Expr));
    bool equals(PTR(Expr)) override;
    PTR(Val) interp(PTR(Env) env) override;
    void printExp(std::ostream&) override;
    bool is_simple() const override { return true; }
    void pretty_print(std::ostream&, precedence_t, std::streampos&) override;
};

class MultExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    MultExpr(PTR(Expr), PTR(Expr));
    bool equals(PTR(Expr)) override;
    PTR(Val) interp(PTR(Env) env) override;
    void printExp(std::ostream&) override;
    bool is_simple() const override { return true; }
    void pretty_print(std::ostream&, precedence_t, std::streampos&) override;
};

class VarExpr : public Expr {
public:
    std::string name;
    VarExpr(const std::string&);
    bool equals(PTR(Expr)) override;
    PTR(Val) interp(PTR(Env) env) override;
    void printExp(std::ostream&) override;
    void pretty_print(std::ostream&, precedence_t, std::streampos&) override;
};

class LetExpr : public Expr {
public:
    std::string var;
    PTR(Expr) rhs;
    PTR(Expr) body;
    LetExpr(const std::string&, PTR(Expr), PTR(Expr));
    bool equals(PTR(Expr)) override;
    PTR(Val) interp(PTR(Env) env) override;
    void printExp(std::ostream&) override;
    void pretty_print(std::ostream&, precedence_t, std::streampos&) override;
};

class BoolExpr : public Expr {
public:
    bool val;
    BoolExpr(bool);
    bool equals(PTR(Expr)) override;
    PTR(Val) interp(PTR(Env) env) override;
    void printExp(std::ostream&) override;
    void pretty_print(std::ostream&, precedence_t, std::streampos&) override;
};

class EqualExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    EqualExpr(PTR(Expr), PTR(Expr));
    bool equals(PTR(Expr)) override;
    PTR(Val) interp(PTR(Env) env) override;
    void printExp(std::ostream&) override;
    void pretty_print(std::ostream&, precedence_t, std::streampos&) override;
};

class IfExpr : public Expr {
public:
    PTR(Expr) condition;
    PTR(Expr) then_branch;
    PTR(Expr) else_branch;
    IfExpr(PTR(Expr), PTR(Expr), PTR(Expr));
    bool equals(PTR(Expr)) override;
    PTR(Val) interp(PTR(Env) env) override;
    void printExp(std::ostream&) override;
    void pretty_print(std::ostream&, precedence_t, std::streampos&) override;
};

class FunExpr : public Expr {
public:
    std::string var;
    PTR(Expr) body;
    FunExpr(const std::string&, PTR(Expr));
    bool equals(PTR(Expr)) override;
    PTR(Val) interp(PTR(Env) env) override;
    void printExp(std::ostream&) override;
    void pretty_print(std::ostream&, precedence_t, std::streampos&) override;
};

class CallExpr : public Expr {
public:
    PTR(Expr) func;
    PTR(Expr) arg;
    CallExpr(PTR(Expr), PTR(Expr));
    bool equals(PTR(Expr)) override;
    PTR(Val) interp(PTR(Env) env) override;
    void printExp(std::ostream&) override;
    bool is_simple() const override { return true; }
    void pretty_print(std::ostream&, precedence_t, std::streampos&) override;
};

#endif
