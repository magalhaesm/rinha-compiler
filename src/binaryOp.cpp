#include "binaryOp.hpp"

Value operator+(const Value& lhs, const Value& rhs)
{
    if (std::holds_alternative<Int>(lhs) && std::holds_alternative<Int>(rhs))
    {
        return std::get<Int>(lhs) + std::get<Int>(rhs);
    }
    if (std::holds_alternative<Str>(lhs) && std::holds_alternative<Str>(rhs))
    {
        return std::get<Str>(lhs) + std::get<Str>(rhs);
    }
    if (std::holds_alternative<Int>(lhs) && std::holds_alternative<Str>(rhs))
    {
        return std::to_string(std::get<Int>(lhs)) + std::get<Str>(rhs);
    }
    if (std::holds_alternative<Str>(lhs) && std::holds_alternative<Int>(rhs))
    {
        return std::get<Str>(lhs) + std::to_string(std::get<Int>(rhs));
    }
    throw std::runtime_error("Invalid operation");
}

Value operator-(const Value& lhs, const Value& rhs)
{
    return std::get<Int>(lhs) - std::get<Int>(rhs);
}

Value operator*(const Value& lhs, const Value& rhs)
{
    return std::get<Int>(lhs) * std::get<Int>(rhs);
}

Value operator/(const Value& lhs, const Value& rhs)
{
    Int rhsValue = std::get<Int>(rhs);
    if (rhsValue == 0)
    {
        throw std::runtime_error("Division by zero");
    }
    return std::get<Int>(lhs) / rhsValue;
}

Value operator%(const Value& lhs, const Value& rhs)
{
    Int rhsValue = std::get<Int>(rhs);
    if (rhsValue == 0)
    {
        throw std::runtime_error("Division by zero");
    }
    return std::get<Int>(lhs) % rhsValue;
}

Value operator==(const Value& lhs, const Value& rhs)
{
    return std::get<Int>(lhs) == std::get<Int>(rhs);
}

Value operator!=(const Value& lhs, const Value& rhs)
{
    return std::get<Int>(lhs) != std::get<Int>(rhs);
}

Value operator<(const Value& lhs, const Value& rhs)
{
    return std::get<Int>(lhs) < std::get<Int>(rhs);
}

Value operator>(const Value& lhs, const Value& rhs)
{
    return std::get<Int>(lhs) > std::get<Int>(rhs);
}

Value operator<=(const Value& lhs, const Value& rhs)
{
    return std::get<Int>(lhs) <= std::get<Int>(rhs);
}

Value operator>=(const Value& lhs, const Value& rhs)
{
    return std::get<Int>(lhs) >= std::get<Int>(rhs);
}

Value operator&&(const Value& lhs, const Value& rhs)
{
    return std::get<Bool>(lhs) && std::get<Bool>(rhs);
}

Value operator||(const Value& lhs, const Value& rhs)
{
    return std::get<Bool>(lhs) || std::get<Bool>(rhs);
}
