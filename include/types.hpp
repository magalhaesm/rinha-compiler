#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstddef>
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

struct Function;
struct Tuple;

using Value = std::variant<Int, Str, Bool, Function, Tuple>;
using Context = std::unordered_map<std::string, Value>;
using Array = std::vector<Value>;

struct Function
{
    std::function<Value(Array& args)> call;
};

struct Tuple
{
    std::shared_ptr<Value> first;
    std::shared_ptr<Value> second;
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

Array getParams(const Node& node);
Array getArgs(const Node& node, Context& ctx);

namespace Type
{
    int32_t Int(const Node& node);
    std::string Str(const Node& node);
    bool Bool(const Node& node);
    Function Function(const Node& node, const Context& ctx);

    std::string to_string(const ::Str& str);
    std::string to_string(const ::Int& integer);
    std::string to_string(const ::Bool& boolean);
    std::string to_string(const ::Function&);
    std::string to_string(const ::Tuple& tuple);
    std::string to_string(const ::Value& value);
}

#endif // !TYPES_HPP
