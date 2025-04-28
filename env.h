//
// Created by Thomas Ho on 4/1/25.
//

#ifndef ENV_H
#define ENV_H
#include "val.h"
#include "expr.h"

class Val;

CLASS(Env) {
public:
    static PTR(Env) empty;
    virtual ~Env() = default;
    virtual PTR(Val) lookup(std::string find_name) = 0;
};

class EmptyEnv : public Env {
public:
    EmptyEnv();
    PTR(Val) lookup(std::string find_name) override;
};

class ExtendedEnv : public Env {
public:
    std::string var;
    PTR(Val) val;
    PTR(Env) rest;

    ExtendedEnv(std::string var, PTR(Val) val, PTR(Env) rest);
    PTR(Val) lookup(std::string find_name) override;
};

#endif //ENV_H
