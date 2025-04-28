#include "parse.h"
#include "expr.h"
#include "val.h"
#include "pointer.h"
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <cerrno>

using namespace std;

void consume(istream &in, int expect) {
    if (in.get() != expect) {
        throw runtime_error("consume mismatch");
    }
}

void skip_whitespace(istream &in) {
    while (isspace(in.peek())) {
        in.get();
    }
}

PTR(Expr) parse_num(istream &in) {
    skip_whitespace(in);
    string num_str;
    bool negative = false;

    if (in.peek() == '-') {
        negative = true;
        num_str += static_cast<char>(in.get());
    }

    while (isdigit(in.peek())) {
        num_str += static_cast<char>(in.get());
    }

    if (num_str.empty() || (negative && num_str.size() == 1)) {
        throw runtime_error("invalid number format");
    }

    return NEW(NumExpr)(stoi(num_str));
}

PTR(Expr) parse_var(istream &in) {
    skip_whitespace(in);
    string var_name;

    if (!isalpha(in.peek())) throw runtime_error("Invalid variable name");

    while (isalnum(in.peek()) || in.peek() == '_') {
        var_name += static_cast<char>(in.get());
    }

    return NEW(VarExpr)(var_name);
}

PTR(Expr) parse_fun(istream &in) {
    skip_whitespace(in);

    // Expect '(' after _fun
    if (in.peek() != '(') {
        throw runtime_error("Expected '(' after _fun");
    }
    consume(in, '(');
    skip_whitespace(in);

    string var;
    while (isalnum(in.peek()) || in.peek() == '_') {
        var += static_cast<char>(in.get());
    }

    skip_whitespace(in);
    if (in.peek() != ')') {
        throw runtime_error("Expected ')' after parameter");
    }
    consume(in, ')');

    PTR(Expr) body = parse_expr(in);
    return NEW(FunExpr)(var, body);
}

PTR(Expr) parse_keyword(istream &in) {
    consume(in, '_');
    string keyword;
    while (isalpha(in.peek())) keyword += static_cast<char>(in.get());

    if (keyword == "true") return NEW(BoolExpr)(true);
    if (keyword == "false") return NEW(BoolExpr)(false);
    if (keyword == "let") return parse_let(in);
    if (keyword == "if") return parse_if(in);
    if (keyword == "fun") return parse_fun(in);

    throw runtime_error("Unknown keyword: _" + keyword);
}

PTR(Expr) parse_multicand(istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    PTR(Expr) e;

    if (c == '(') {
        consume(in, '(');
        e = parse_expr(in);
        skip_whitespace(in);
        consume(in, ')');
    } else if (isdigit(c) || c == '-') {
        e = parse_num(in);
    } else if (isalpha(c)) {
        e = parse_var(in);
    } else if (c == '_') {
        e = parse_keyword(in);
    } else {
        throw runtime_error("invalid input");
    }

    while (true) {
        skip_whitespace(in);
        if (in.peek() != '(') break;
        consume(in, '(');
        PTR(Expr) actual_arg = (in.peek() == ')') ? NEW(NumExpr)(0) : parse_expr(in);
        consume(in, ')');
        e = NEW(CallExpr)(e, actual_arg);
    }

    return e;
}

PTR(Expr) parse_multend(istream &in) {
    PTR(Expr) e = parse_multicand(in);
    while (true) {
        skip_whitespace(in);
        if (in.peek() == '*') {
            consume(in, '*');
            e = NEW(MultExpr)(e, parse_multicand(in));
        } else {
            break;
        }
    }
    return e;
}

PTR(Expr) parse_addend(istream &in) {
    PTR(Expr) e = parse_multend(in);
    while (true) {
        skip_whitespace(in);
        if (in.peek() == '+') {
            consume(in, '+');
            e = NEW(AddExpr)(e, parse_multend(in));
        } else {
            break;
        }
    }
    return e;
}

PTR(Expr) parse_comparison(istream &in) {
    PTR(Expr) e = parse_addend(in);
    skip_whitespace(in);
    if (in.peek() == '=') {
        consume(in, '=');
        if (in.get() != '=') throw runtime_error("Expected ==");
        return NEW(EqualExpr)(e, parse_comparison(in));
    }
    return e;
}

PTR(Expr) parse_expr(istream &in) {
    PTR(Expr) e = parse_comparison(in);
    skip_whitespace(in);
    return e;
}

PTR(Expr) parse_str(const string &s) {
    istringstream in(s);
    return parse_expr(in);
}

PTR(Expr) parse_let(istream &in) {
    skip_whitespace(in);
    string var;

    // Parse variable name
    while (isalnum(in.peek()) || in.peek() == '_') {
        var += static_cast<char>(in.get());
    }

    skip_whitespace(in);
    consume(in, '='); // Expect '=' after variable

    PTR(Expr) rhs = parse_expr(in);

    skip_whitespace(in);
    consume(in, '_'); // Expect _in
    string in_kw;
    while (isalpha(in.peek())) in_kw += static_cast<char>(in.get());
    if (in_kw != "in") throw runtime_error("Expected _in");

    PTR(Expr) body = parse_expr(in);
    return NEW(LetExpr)(var, rhs, body);
}

PTR(Expr) parse_if(istream &in) {
    PTR(Expr) cond = parse_expr(in);

    skip_whitespace(in);
    consume(in, '_');
    string then_kw;
    while (isalpha(in.peek())) then_kw += static_cast<char>(in.get());
    if (then_kw != "then") throw runtime_error("expected _then"); // Lowercase

    PTR(Expr) then_branch = parse_expr(in);

    skip_whitespace(in);
    consume(in, '_');
    string else_kw;
    while (isalpha(in.peek())) else_kw += static_cast<char>(in.get());
    if (else_kw != "else") throw runtime_error("expected _else"); // Lowercase

    PTR(Expr) else_branch = parse_expr(in);

    return NEW(IfExpr)(cond, then_branch, else_branch);
}

