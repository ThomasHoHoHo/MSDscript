#ifndef PARSE_HPP
#define PARSE_HPP

#include "expr.h"
#include <stdio.h>
#include <string>
#include <istream>

/**
 * @file parse.hpp
 * @brief Header file for parsing expressions.
 *
 * This file declares functions for parsing expressions from strings and input streams.
 */
class Expr;

/**
 * @brief Parses an expression from a string.
 * @param s The input string to parse.
 * @return A pointer to the parsed expression.
 * @throws std::runtime_error If the input string is invalid.
 */
PTR(Expr) parse_str(const std::string& s);

/**
 * @brief Parses an expression from an input stream.
 * @param in The input stream to parse.
 * @return A pointer to the parsed expression.
 * @throws std::runtime_error If the input stream contains invalid data.
 */
PTR(Expr) parse(std::istream &in);

/**
 * @brief Parses a general expression from an input stream.
 * @param in The input stream to parse.
 * @return A pointer to the parsed expression.
 * @throws std::runtime_error If the input is not a valid expression.
 */
PTR(Expr) parse_expr(std::istream &in);

/**
 * @brief Parses a numeric expression from an input stream.
 * @param in The input stream to parse.
 * @return A pointer to the parsed Num expression.
 * @throws std::runtime_error If the input is not a valid number.
 */
PTR(Expr) parse_num(std::istream &in);

/**
 * @brief Parses a variable expression from an input stream.
 * @param in The input stream to parse.
 * @return A pointer to the parsed VarExpr expression.
 * @throws std::runtime_error If the input is not a valid variable name.
 */
PTR(Expr) parse_var(std::istream &in);

/**
 * @brief Parses a let expression from an input stream.
 * @param in The input stream to parse.
 * @return A pointer to the parsed LetExpr expression.
 * @throws std::runtime_error If the input is not a valid let expression.
 */
PTR(Expr) parse_let(std::istream &in);

/**
 * @brief Parses an additive expression (e.g., x + y) from an input stream.
 * @param in The input stream to parse.
 * @return A pointer to the parsed Add expression.
 */
PTR(Expr) parse_addened(std::istream &in);

/**
 * @brief Parses a multiplicative expression (e.g., x * y) from an input stream.
 * @param in The input stream to parse.
 * @return A pointer to the parsed Mult expression.
 */
PTR(Expr) parse_multened(std::istream &in);

/**
 * @brief Skips all whitespace characters in the input stream.
 * @param in The input stream to skip whitespace in.
 */
void skip_whitespace(std::istream &in);

/**
 * @brief Consumes a specific character from the input stream.
 * @param in The input stream.
 * @param expect The expected character to consume.
 * @throws std::runtime_error If the next character does not match the expected character.
 */
void consume(std::istream &in, int expect);

PTR(Expr) parse_keyword(std::istream &in);
PTR(Expr) parse_if(std::istream &in);
PTR(Expr) parse_comparg(std::istream &in);

PTR(Expr) parse_fun(std::istream &in);
PTR(Expr) parse_call(std::istream &in);

#endif // PARSE_HPP
