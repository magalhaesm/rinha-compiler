#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "types.hpp"

Document parse(const std::string& filePath);
Value eval(const Node& node);
inline Kind match(const Node& node);
BinaryOp matchOp(const Node& node);
Value evalBinary(const Node& node);

Value print(const Value& val);

#endif // !INTERPRETER_H
