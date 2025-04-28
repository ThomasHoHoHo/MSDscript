#include "env.h"
#include "val.h"

PTR(Env) Env::empty = NEW(EmptyEnv)();

EmptyEnv::EmptyEnv() {}
PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("Free variable: " + find_name);
}

ExtendedEnv::ExtendedEnv(std::string var, PTR(Val) val, PTR(Env) rest)
    : var(var), val(val), rest(rest) {}

PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (find_name == var) {
        return val;
    } else {
        return rest->lookup(find_name);
    }
}
