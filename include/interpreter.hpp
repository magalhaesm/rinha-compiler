#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "types.hpp"

Document parse(const std::string& filePath);
Value eval(const Node& node, Context& ctx);
Value evalBinary(const Node& node, Context& ctx);
Value evalCall(const Node& node, Context& ctx);
Value print(const Value& val);

Kind match(const Node& node);
BinaryOp matchOp(const Node& node);
bool hasNext(const Node& node);

#endif // !INTERPRETER_H
