#include "val.h"
#include "expr.h"
#include "env.h"
#include <sstream>
#include <string>
#include <stdexcept>

// ==================== NumExpr ====================
NumExpr::NumExpr(int64_t val) : val(val) {}

bool NumExpr::equals(PTR(Expr) e) {
    PTR(NumExpr) num = CAST(NumExpr)(e);
    return num && this->val == num->val;
}

PTR(Val) NumExpr::interp(PTR(Env) env) {
    return NEW(NumVal)(val);
}

void NumExpr::printExp(std::ostream &os) {
    os << val;
}

void NumExpr::pretty_print(std::ostream &os, precedence_t, std::streampos&) {
    os << val;
}

// ==================== AddExpr ====================
AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) : lhs(lhs), rhs(rhs) {}

bool AddExpr::equals(PTR(Expr) e) {
    PTR(AddExpr) add = CAST(AddExpr)(e);
    return add && lhs->equals(add->lhs) && rhs->equals(add->rhs);
}

PTR(Val) AddExpr::interp(PTR(Env) env) {
    return lhs->interp(env)->add_to(rhs->interp(env));
}

void AddExpr::printExp(std::ostream &os) {
    os << "(" << lhs->to_string() << "+" << rhs->to_string() << ")";
}

void AddExpr::pretty_print(std::ostream &os, precedence_t prec, std::streampos &lastIndent) {
    bool needs_paren = prec >= prec_add;
    if (needs_paren) os << "(";
    lhs->pretty_print(os, prec_add, lastIndent);
    os << " + ";
    rhs->pretty_print(os, prec_none, lastIndent);
    if (needs_paren) os << ")";
}

// ==================== MultExpr ====================
MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) : lhs(lhs), rhs(rhs) {}

bool MultExpr::equals(PTR(Expr) e) {
    PTR(MultExpr) mult = CAST(MultExpr)(e);
    return mult && lhs->equals(mult->lhs) && rhs->equals(mult->rhs);
}

PTR(Val) MultExpr::interp(PTR(Env) env) {
    return lhs->interp(env)->mult_with(rhs->interp(env));
}

void MultExpr::printExp(std::ostream &os) {
    os << "(" << lhs->to_string() << "*" << rhs->to_string() << ")";
}

void MultExpr::pretty_print(std::ostream &os, precedence_t prec, std::streampos &lastIndent) {
    bool needs_paren = prec >= prec_mult;
    if (needs_paren) os << "(";
    lhs->pretty_print(os, prec_mult, lastIndent);
    os << " * ";
    rhs->pretty_print(os, prec_mult, lastIndent);
    if (needs_paren) os << ")";
}

// ==================== VarExpr ====================
VarExpr::VarExpr(const std::string &name) : name(name) {}

bool VarExpr::equals(PTR(Expr) e) {
    PTR(VarExpr) var = CAST(VarExpr)(e);
    return var && name == var->name;
}

PTR(Val) VarExpr::interp(PTR(Env) env) {
    return env->lookup(name);
}

void VarExpr::printExp(std::ostream &os) {
    os << name;
}

void VarExpr::pretty_print(std::ostream &os, precedence_t, std::streampos&) {
    os << name;
}

// ==================== LetExpr ====================
LetExpr::LetExpr(const std::string &var, PTR(Expr) rhs, PTR(Expr) body)
    : var(var), rhs(rhs), body(body) {}

bool LetExpr::equals(PTR(Expr) e) {
    PTR(LetExpr) let = CAST(LetExpr)(e);
    return let && var == let->var &&
           rhs->equals(let->rhs) &&
           body->equals(let->body);
}

PTR(Val) LetExpr::interp(PTR(Env) env) {
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(var, rhs_val, env);
    return body->interp(new_env);
}

void LetExpr::printExp(std::ostream &os) {
    os << "(_let " << var << "=" << rhs->to_string()
    << " _in " << body->to_string() << ")";
}

void LetExpr::pretty_print(std::ostream &os, precedence_t prec, std::streampos &lastIndent) {
    bool needs_paren = prec != prec_none;
    if (needs_paren) os << "(";

    std::streampos let_start = os.tellp();
    os << "_let " << var << " = ";
    rhs->pretty_print(os, prec_none, lastIndent);

    os << "\n";
    size_t indent = let_start - lastIndent;
    os << std::string(indent, ' ') << "_in ";

    std::streampos in_start = os.tellp();
    body->pretty_print(os, prec_none, in_start);

    if (needs_paren) os << ")";
}

// ==================== BoolExpr ====================
BoolExpr::BoolExpr(bool val) : val(val) {}

bool BoolExpr::equals(PTR(Expr) e) {
    PTR(BoolExpr) b = CAST(BoolExpr)(e);
    return b && val == b->val;
}

PTR(Val) BoolExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(val);
}

void BoolExpr::printExp(std::ostream &os) {
    os << (val ? "_true" : "_false");
}

void BoolExpr::pretty_print(std::ostream &os, precedence_t, std::streampos&) {
    os << (val ? "_true" : "_false");
}

// ==================== EqualExpr ====================
EqualExpr::EqualExpr(PTR(Expr) lhs, PTR(Expr) rhs)
    : lhs(lhs), rhs(rhs) {}

bool EqualExpr::equals(PTR(Expr) e) {
    PTR(EqualExpr) eq = CAST(EqualExpr)(e);
    return eq && lhs->equals(eq->lhs) && rhs->equals(eq->rhs);
}

PTR(Val) EqualExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(lhs->interp(env)->equals(rhs->interp(env)));
}

void EqualExpr::printExp(std::ostream &os) {
    os << "(" << lhs->to_string() << "==" << rhs->to_string() << ")";
}

void EqualExpr::pretty_print(std::ostream &os, precedence_t prec, std::streampos &lastIndent) {
    bool needs_paren = prec > prec_none;
    if (needs_paren) os << "(";
    lhs->pretty_print(os, prec_add, lastIndent);
    os << " == ";
    rhs->pretty_print(os, prec_add, lastIndent);
    if (needs_paren) os << ")";
}

// ==================== IfExpr ====================
IfExpr::IfExpr(PTR(Expr) condition, PTR(Expr) then_branch, PTR(Expr) else_branch)
    : condition(condition), then_branch(then_branch), else_branch(else_branch) {}

bool IfExpr::equals(PTR(Expr) e) {
    PTR(IfExpr) i = CAST(IfExpr)(e);
    return i && condition->equals(i->condition) &&
           then_branch->equals(i->then_branch) &&
           else_branch->equals(i->else_branch);
}

PTR(Val) IfExpr::interp(PTR(Env) env) {
    PTR(Val) cond_val = condition->interp(env);
    PTR(BoolVal) bool_cond = CAST(BoolVal)(cond_val);
    if (!bool_cond) throw std::runtime_error("Condition must be boolean");
    return bool_cond->val ? then_branch->interp(env) : else_branch->interp(env);
}

void IfExpr::printExp(std::ostream &os) {
    os << "(_if " << condition->to_string()
    << " _then " << then_branch->to_string()
    << " _else " << else_branch->to_string() << ")";
}

void IfExpr::pretty_print(std::ostream &os, precedence_t prec, std::streampos &lastIndent) {
    bool needs_paren = prec != prec_none;
    if (needs_paren) os << "(";

    std::streampos if_start = os.tellp();
    os << "_if ";
    condition->pretty_print(os, prec_none, lastIndent);

    os << "\n";
    size_t indent = if_start - lastIndent + 2;
    std::streampos new_indent_pos = if_start + std::streamoff(indent);

    os << std::string(indent, ' ') << "_then ";
    then_branch->pretty_print(os, prec_none, new_indent_pos);

    os << "\n" << std::string(indent, ' ') << "_else ";
    else_branch->pretty_print(os, prec_none, new_indent_pos);

    if (needs_paren) os << ")";
}

// ==================== FunExpr ====================
FunExpr::FunExpr(const std::string &var, PTR(Expr) body)
    : var(var), body(body) {}

bool FunExpr::equals(PTR(Expr) e) {
    PTR(FunExpr) f = CAST(FunExpr)(e);
    return f && var == f->var && body->equals(f->body);
}

PTR(Val) FunExpr::interp(PTR(Env) env) {
    return NEW(FunVal)(var, body, env);
}

void FunExpr::printExp(std::ostream &os) {
    os << "(_fun (" << var << ") " << body->to_string() << ")";
}

void FunExpr::pretty_print(std::ostream &os, precedence_t prec, std::streampos &lastIndent) {
    bool needs_paren = prec != prec_none;
    if (needs_paren) os << "(";

    os << "_fun (" << var << ")";
    if (body->is_simple()) {
        os << " ";
        body->pretty_print(os, prec_none, lastIndent);
    } else {
        os << "\n  ";
        std::streampos body_start = os.tellp();
        body->pretty_print(os, prec_none, body_start);
    }

    if (needs_paren) os << ")";
}

// ==================== CallExpr ====================
CallExpr::CallExpr(PTR(Expr) func, PTR(Expr) arg)
    : func(func), arg(arg) {}

bool CallExpr::equals(PTR(Expr) e) {
    PTR(CallExpr) c = CAST(CallExpr)(e);
    return c && func->equals(c->func) && arg->equals(c->arg);
}

PTR(Val) CallExpr::interp(PTR(Env) env) {
    PTR(Val) func_val = func->interp(env);
    PTR(FunVal) fun = CAST(FunVal)(func_val);
    if (!fun) throw std::runtime_error("Cannot call non-function value");

    PTR(Val) arg_val = arg->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(fun->var, arg_val, fun->env);
    return fun->body->interp(new_env);
}

void CallExpr::printExp(std::ostream &os) {
    os << func->to_string() << "(" << arg->to_string() << ")";
}

void CallExpr::pretty_print(std::ostream &os, precedence_t, std::streampos &lastIndent) {
    func->pretty_print(os, prec_none, lastIndent);
    os << "(";
    arg->pretty_print(os, prec_none, lastIndent);
    os << ")";
}

// ==================== Base Methods ====================
std::string Expr::to_string() {
    std::stringstream ss;
    this->printExp(ss);
    return ss.str();
}

std::string Expr::to_pretty_string() {
    std::stringstream ss;
    std::streampos initial_pos = ss.tellp();
    this->pretty_print(ss, prec_none, initial_pos);
    return ss.str();
}
