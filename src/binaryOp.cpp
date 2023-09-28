#include "binaryOp.hpp"

struct ValueAdditionVisitor
{
    Value operator()(const Int& lhs, const Int& rhs) const
    {
        return Int{ lhs + rhs };
    }

    Value operator()(const Str& lhs, const Str& rhs) const
    {
        return Str{ lhs + rhs };
    }

    Value operator()(const Int& lhs, const Str& rhs) const
    {
        return Str{ std::to_string(lhs) + rhs };
    }

    Value operator()(const Str& lhs, const Int& rhs) const
    {
        return Str{ lhs + std::to_string(rhs) };
    }

    template <typename... Types>
    Value operator()(const Types&...) const
    {
        throw std::bad_variant_access();
    }
};

Value operator+(const Value& lhs, const Value& rhs)
{
    return std::visit(ValueAdditionVisitor(), lhs, rhs);
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
        throw std::runtime_error("division by zero");
    }
    return std::get<Int>(lhs) / rhsValue;
}

Value operator%(const Value& lhs, const Value& rhs)
{
    Int rhsValue = std::get<Int>(rhs);
    if (rhsValue == 0)
    {
        throw std::runtime_error("division by zero");
    }
    return std::get<Int>(lhs) % rhsValue;
}

struct ValueEqualityVisitor
{
    bool operator()(const Int& lhs, const Int& rhs) const
    {
        return lhs == rhs;
    }

    bool operator()(const Str& lhs, const Str& rhs) const
    {
        return lhs == rhs;
    }

    bool operator()(const Bool& lhs, const Bool& rhs) const
    {
        return lhs == rhs;
    }

    template <typename T, typename U>
    bool operator()(const T&, const U&) const
    {
        throw std::bad_variant_access();
    }
};

Value operator==(const Value& lhs, const Value& rhs)
{
    return std::visit(ValueEqualityVisitor(), lhs, rhs);
}

Value operator!=(const Value& lhs, const Value& rhs)
{
    return !std::visit(ValueEqualityVisitor(), lhs, rhs);
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
