#ifndef BINARY_OP_HPP
#define BINARY_OP_HPP

#include <variant>

#include "types.hpp"

Value operator+(const Value& lhs, const Value& rhs);
Value operator-(const Value& lhs, const Value& rhs);
Value operator*(const Value& lhs, const Value& rhs);
Value operator/(const Value& lhs, const Value& rhs);
Value operator%(const Value& lhs, const Value& rhs);
Value operator==(const Value& lhs, const Value& rhs);
Value operator!=(const Value& lhs, const Value& rhs);
Value operator<(const Value& lhs, const Value& rhs);
Value operator>(const Value& lhs, const Value& rhs);
Value operator<=(const Value& lhs, const Value& rhs);
Value operator>=(const Value& lhs, const Value& rhs);
Value operator&&(const Value& lhs, const Value& rhs);
Value operator||(const Value& lhs, const Value& rhs);

#endif // !BINARY_OP_HPP
