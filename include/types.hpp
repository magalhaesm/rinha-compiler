#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <utility>
#include <vector>
#include <variant>
#include <memory>
#include <functional>
#include <unordered_map>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

using rapidjson::Document;
using Node = rapidjson::Value;

const int Void = 0;

using Int = int32_t;
using Str = std::string;
using Bool = bool;

struct Closure;
using Function = std::shared_ptr<Closure>;

struct Tuple;

using Value = std::variant<Int, Str, Bool, Function, Tuple>;
using Context = std::unordered_map<std::string, Value>;
using Array = std::vector<Value>;

struct Closure
{
    std::function<Value(Array& args)> call;
    std::unordered_map<uint32_t, Value> cache;
};

template <>
struct std::hash<Function>
{
    std::size_t operator()(const Function& fn) const noexcept
    {
        return reinterpret_cast<size_t>(&fn);
    }
};

struct Tuple
{
    std::shared_ptr<Value> first;
    std::shared_ptr<Value> second;
};

template <>
struct std::hash<Tuple>
{
    std::size_t operator()(const Tuple& tuple) const noexcept
    {
        return reinterpret_cast<size_t>(&tuple);
    }
};

enum class Kind
{
    Int,
    Str,
    Bool,
    Print,
    Binary,
    If,
    Let,
    Var,
    Function,
    Call,
    Tuple,
    First,
    Second
};

enum class BinaryOp
{
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    Eq,
    Neq,
    Lt,
    Gt,
    Lte,
    Gte,
    And,
    Or,
};

namespace Type
{
    Int Int(const Node& node);
    Str Str(const Node& node);
    Bool Bool(const Node& node);
    Function Function(const Node& node, const Context& ctx);
    Tuple Tuple(const Node& node, Context& ctx);

    std::string to_string(const ::Str& str);
    std::string to_string(const ::Int& integer);
    std::string to_string(const ::Bool& boolean);
    std::string to_string(const ::Function&);
    std::string to_string(const ::Tuple& tuple);
    std::string to_string(const ::Value& value);
}

#endif // !TYPES_HPP
