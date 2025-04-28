#include "val.h"
#include "expr.h"
#include <limits>
#include <stdexcept>

NumVal::NumVal(int64_t val) : val(val) {}

PTR(Val) NumVal::add_to(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (!other_num) throw std::runtime_error("Add of non-number");

    if ((other_num->val > 0) && (val > (INT64_MAX - other_num->val))) {
        throw std::runtime_error("Addition overflow");
    }
    if ((other_num->val < 0) && (val < (INT64_MIN - other_num->val))) {
        throw std::runtime_error("Addition overflow");
    }

    return NEW(NumVal)(val + other_num->val);
}

PTR(Val) NumVal::mult_with(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (!other_num) throw std::runtime_error("Multiplication of non-number");

    if (val == 0 || other_num->val == 0) return NEW(NumVal)(0);

    if (val > 0) {
        if (other_num->val > 0) {
            if (val > (INT64_MAX / other_num->val)) {
                throw std::runtime_error("Multiplication overflow");
            }
        } else {
            if (other_num->val < (INT64_MIN / val)) {
                throw std::runtime_error("Multiplication overflow");
            }
        }
    } else {
        if (other_num->val > 0) {
            if (val < (INT64_MIN / other_num->val)) {
                throw std::runtime_error("Multiplication overflow");
            }
        } else {
            if (val != 0 && other_num->val < (INT64_MAX / val)) {
                throw std::runtime_error("Multiplication overflow");
            }
        }
    }
    return NEW(NumVal)(val * other_num->val);
}

bool NumVal::equals(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    return other_num && val == other_num->val;
}

PTR(Expr) NumVal::to_expr() {
    return NEW(NumExpr)(val);
}

std::string NumVal::to_string() {
    return std::to_string(val);
}

bool NumVal::is_true() {
    throw std::runtime_error("test of non-boolean");
}

BoolVal::BoolVal(bool val) : val(val) {}

PTR(Val) BoolVal::add_to(PTR(Val)) {
    throw std::runtime_error("Addition of boolean");
}

PTR(Val) BoolVal::mult_with(PTR(Val)) {
    throw std::runtime_error("Multiplication of boolean");
}

bool BoolVal::equals(PTR(Val) other) {
    PTR(BoolVal) b = CAST(BoolVal)(other);
    return b && val == b->val;
}

PTR(Expr) BoolVal::to_expr() {
    return NEW(BoolExpr)(val);
}

std::string BoolVal::to_string() {
    return val ? "_true" : "_false";
}

bool BoolVal::is_true() {
    throw std::runtime_error("Testing of a non-boolean");
}

bool Val::is_true() {
    throw std::runtime_error("test of boolean");
}

FunVal::FunVal(std::string var, PTR(Expr) body, PTR(Env) env)
    : var(var), body(body), env(env) {}

PTR(Val) FunVal::add_to(PTR(Val)) {
    throw std::runtime_error("Cannot add functions");
}

PTR(Val) FunVal::mult_with(PTR(Val)) {
    throw std::runtime_error("Cannot multiply functions");
}

bool FunVal::equals(PTR(Val) other) {
    PTR(FunVal) f = CAST(FunVal)(other);
    return f && var == f->var && body->equals(f->body) && env == f->env;
}

PTR(Expr) FunVal::to_expr() {
    return NEW(FunExpr)(var, body);
}

std::string FunVal::to_string() {
    return "[function]";
}
