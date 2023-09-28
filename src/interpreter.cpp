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

static Array getArgs(const Node& node, Context& ctx);
static uint32_t hashValues(Array& args);

std::unordered_map<std::string, Value> global;

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
    file.close();

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
        return print(node, ctx);
    case Kind::Binary:
        return evalBinary(node, ctx);
    case Kind::If:
        return evalIfElse(node, ctx);
    case Kind::Let:
        return evalLet(node, ctx);
    case Kind::Var:
        return evalVar(node, ctx);
    case Kind::Function:
        return Type::Function(node, ctx);
    case Kind::Call:
        return evalCall(node, ctx);
    case Kind::Tuple:
        return Type::Tuple(node, ctx);
    case Kind::First:
        return evalTupleFirst(node, ctx);
    case Kind::Second:
        return evalTupleSecond(node, ctx);
    }
    throw UnrecognizedTerm(node);
}

inline Value print(const Node& node, Context& ctx)
{
    auto value = eval(node["value"], ctx);
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
    catch (const std::exception& e)
    {
        throw GenericError(e.what(), node);
    }
    throw BinaryOperationError("unrecognized operator", node);
}

inline Value evalIfElse(const Node& node, Context& ctx)
{
    auto condition = std::get<Bool>(eval(node["condition"], ctx));
    if (condition == true)
    {
        return eval(node["then"], ctx);
    }
    return eval(node["otherwise"], ctx);
}

inline Value evalLet(const Node& node, Context& ctx)
{
    auto name = node["name"]["text"].GetString();
    auto value = eval(node["value"], ctx);

    if (node["value"]["kind"].GetString() == std::string("Function"))
    {
        global[name] = value;
    }
    else
    {
        ctx[name] = value;
    }

    return hasNext(node) ? eval(node["next"], ctx) : Void;
}

inline Value evalVar(const Node& node, Context& ctx)
{
    auto text = node["text"].GetString();
    auto value = ctx.find(text);
    if (value != ctx.end())
    {
        return value->second;
    }

    value = global.find(text);
    if (value != global.end())
    {
        return value->second;
    }
    throw UndeclaredIdentifier(node);
}

inline Value evalTupleFirst(const Node& node, Context& ctx)
{
    auto tuple = std::get<Tuple>(eval(node["value"], ctx));
    return *(tuple.first);
}

inline Value evalTupleSecond(const Node& node, Context& ctx)
{
    auto tuple = std::get<Tuple>(eval(node["value"], ctx));
    return *(tuple.second);
}

inline Value evalCall(const Node& node, Context& ctx)
{
    try
    {
        Function fn = std::get<Function>(eval(node["callee"], ctx));

        auto args = getArgs(node, ctx);
        uint32_t key = hashValues(args);

        auto value = fn->cache.find(key);
        if (value != fn->cache.end())
        {
            return value->second;
        }

        fn->cache[key] = fn->call(args);
        return fn->cache[key];
    }
    catch (const std::bad_variant_access&)
    {
        throw UndeclaredIdentifier(node);
    }
    catch (const std::runtime_error&)
    {
        throw InvalidArguments(node);
    }
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

inline Array getArgs(const Node& node, Context& ctx)
{
    auto args = node["arguments"].GetArray();
    Array arr;

    for (auto& arg : args)
    {
        arr.push_back(eval(arg, ctx));
    }
    return arr;
}

inline uint32_t hashValues(Array& values)
{
    std::hash<Value> valueHasher;

    uint32_t hash = 2166136261;
    for (auto& value : values)
    {
        hash ^= valueHasher(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}
