#include <memory>
#include <fstream>
#include <variant>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "error.hpp"
#include "types.hpp"
#include "binaryOp.hpp"
#include "interpreter.hpp"

std::unordered_map<std::string, Kind> kindTable = {

    { "Int", Kind::Int },       { "Str", Kind::Str },       { "Bool", Kind::Bool },
    { "Print", Kind::Print },   { "Binary", Kind::Binary }, { "If", Kind::If },
    { "Let", Kind::Let },       { "Var", Kind::Var },       { "Function", Kind::Function },
    { "Call", Kind::Call },     { "Tuple", Kind::Tuple },   { "First", Kind::First },
    { "Second", Kind::Second },
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

Value eval(const Node& node, Context& ctx)
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
        return print(eval(node["value"], ctx));
    case Kind::Binary:
        return evalBinary(node, ctx);
    case Kind::If:
    {
        auto condition = std::get<Bool>(eval(node["condition"], ctx));
        if (condition == true)
        {
            return eval(node["then"], ctx);
        }
        return eval(node["otherwise"], ctx);
    }
    case Kind::Let:
    {
        auto name = node["name"]["text"].GetString();
        auto value = eval(node["value"], ctx);

        ctx[name] = value;
        return hasNext(node) ? eval(node["next"], ctx) : Void;
    }
    case Kind::Var:
    {
        auto text = node["text"].GetString();
        auto value = ctx.find(text);
        if (value != ctx.end())
        {
            return value->second;
        }
        throw UnrecognizedIdentifier(node);
    }
    case Kind::Function:
        return Type::Function(node, ctx);
    case Kind::Call:
        return evalCall(node, ctx);
    case Kind::Tuple:
    {
        return Tuple{

            std::make_shared<Value>(eval(node["first"], ctx)),
            std::make_shared<Value>(eval(node["second"], ctx))
        };
    }
    case Kind::First:
    {
        auto tuple = std::get<Tuple>(eval(node["value"], ctx));
        return *(tuple.first);
    }
    case Kind::Second:
    {
        auto tuple = std::get<Tuple>(eval(node["value"], ctx));
        return *(tuple.second);
    }
    }
    throw UnrecognizedTerm(node);
}

inline Value print(const Value& value)
{
    std::cout << Type::to_string(value) << '\n';
    return value;
}

inline Value evalBinary(const Node& node, Context& ctx)
{
    auto lhs = eval(node["lhs"], ctx);
    auto rhs = eval(node["rhs"], ctx);

    try
    {
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
    }
    catch (const std::bad_variant_access&)
    {
        throw BinaryOperationError("invalid operands", node);
    }
    throw BinaryOperationError("unrecognized operator", node);
}

inline Value evalCall(const Node& node, Context& ctx)
{
    Function fn;
    try
    {
        fn = std::get<Function>(eval(node["callee"], ctx));
    }
    catch (const std::bad_variant_access&)
    {
        throw UndeclaredSymbol(node);
    };

    auto args = getArgs(node, ctx);
    uint32_t key = hashValues(args);

    auto value = fn->cache.find(key);
    if (value != fn->cache.end())
    {
        return value->second;
    }

    auto result = fn->call(args);
    fn->cache[key] = result;
    return result;
}

inline Kind match(const Node& node)
{
    return kindTable[node["kind"].GetString()];
}

inline BinaryOp matchOp(const Node& node)
{
    return binaryOpTable[node["op"].GetString()];
}

inline bool hasNext(const Node& node)
{
    auto itr = node.FindMember("next");
    if (itr != node.MemberEnd())
    {
        return true;
    }
    return false;
}
