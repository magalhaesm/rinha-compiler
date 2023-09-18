#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "binaryOp.hpp"
#include "interpreter.hpp"

std::unordered_map<std::string, Kind> kindTable = {

    { "Int", Kind::Int },     { "Str", Kind::Str },       { "Bool", Kind::Bool },
    { "Print", Kind::Print }, { "Binary", Kind::Binary }, { "If", Kind::If },
};

std::unordered_map<std::string, BinaryOp> binaryOpTable = {

    { "Add", BinaryOp::Add }, { "Sub", BinaryOp::Sub }, { "Mul", BinaryOp::Mul },
    { "Div", BinaryOp::Div }, { "Rem", BinaryOp::Rem }, { "Eq", BinaryOp::Eq },
    { "Neq", BinaryOp::Neq }, { "Lt", BinaryOp::Lt },   { "Gt", BinaryOp::Gt },
    { "Lte", BinaryOp::Lte }, { "Gte", BinaryOp::Gte }, { "And", BinaryOp::And },
    { "Or", BinaryOp::Or },
};

Document parse(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open the JSON file.");
    }

    rapidjson::IStreamWrapper input(file);

    Document document;
    document.ParseStream(input);

    if (document.HasParseError())
    {
        throw std::runtime_error("Error parsing the JSON.");
    }
    return document;
}

Value eval(const Node& node)
{
    switch (match(node))
    {
    case Kind::Int:
        return Type::Int(node);
    case Kind::Str:
        return Type::Str(node);
    case Kind::Bool:
        return Type::Bool(node);
    case Kind::Print:
        return print(eval(node["value"]));
    case Kind::Binary:
        return evalBinary(node);
    case Kind::If:
    {
        auto condition = std::get<Bool>(eval(node["condition"]));
        return condition == true ? eval(node["then"]) : eval(node["otherwise"]);
    }
    }
    throw std::runtime_error("Unrecognized term");
}

Value evalBinary(const Node& node)
{
    auto lhs = eval(node["lhs"]);
    auto rhs = eval(node["rhs"]);

    switch (matchOp(node))
    {
    case BinaryOp::Add:
        return lhs + rhs;
    case BinaryOp::Sub:
        return lhs - rhs;
    case BinaryOp::Mul:
        return lhs * rhs;
    case BinaryOp::Div:
        return lhs / rhs;
    case BinaryOp::Rem:
        return lhs % rhs;
    case BinaryOp::Eq:
        return lhs == rhs;
    case BinaryOp::Neq:
        return lhs != rhs;
    case BinaryOp::Lt:
        return lhs < rhs;
    case BinaryOp::Gt:
        return lhs > rhs;
    case BinaryOp::Lte:
        return lhs <= rhs;
    case BinaryOp::Gte:
        return lhs >= rhs;
    case BinaryOp::And:
        return lhs && rhs;
    case BinaryOp::Or:
        return lhs || rhs;
    }
    throw std::runtime_error("Unrecognized operation");
}

Kind match(const Node& node)
{
    return kindTable[node["kind"].GetString()];
}

BinaryOp matchOp(const Node& node)
{
    return binaryOpTable[node["op"].GetString()];
}

Value print(const Value& val)
{
    std::visit(
        [](const auto& v)
        {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, Str>)
            {
                std::cout << v << '\n';
            }
            else if constexpr (std::is_same_v<T, Int>)
            {
                std::cout << v << '\n';
            }
            else if constexpr (std::is_same_v<T, Bool>)
            {
                std::cout << std::boolalpha << v << '\n';
            }
        },
        val);
    return val;
}
