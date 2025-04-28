#ifndef VAL_H
#define VAL_H

#include "pointer.h"
#include <string>

class Expr;
class Env;

CLASS(Val) {
public:
    virtual ~Val() = default;
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
    virtual PTR(Val) mult_with(PTR(Val) other_val) = 0;
    virtual bool equals(PTR(Val) other_val) = 0;
    virtual PTR(Expr) to_expr() = 0;
    virtual std::string to_string() = 0;
    virtual bool is_true();
};

class NumVal : public Val {
public:
    int64_t val;
    NumVal(int64_t val);
    PTR(Val) add_to(PTR(Val) other_val) override;
    PTR(Val) mult_with(PTR(Val) other_val) override;
    bool equals(PTR(Val) other_val) override;
    PTR(Expr) to_expr() override;
    std::string to_string() override;
    bool is_true() override;
};

class BoolVal : public Val {
public:
    bool val;
    BoolVal(bool val);
    PTR(Val) add_to(PTR(Val) other_val) override;
    PTR(Val) mult_with(PTR(Val) other_val) override;
    bool equals(PTR(Val) other_val) override;
    PTR(Expr) to_expr() override;
    std::string to_string() override;
    bool is_true() override;
};

class FunVal : public Val {
public:
    std::string var;
    PTR(Expr) body;
    PTR(Env) env;
    FunVal(std::string var, PTR(Expr) body, PTR(Env) env);
    PTR(Val) add_to(PTR(Val) other_val) override;
    PTR(Val) mult_with(PTR(Val) other_val) override;
    bool equals(PTR(Val) other_val) override;
    PTR(Expr) to_expr() override;
    std::string to_string() override;
};

#endif
