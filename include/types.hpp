#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstddef>
#include <vector>
#include <variant>
#include <functional>
#include <unordered_map>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

using rapidjson::Document;
using Node = rapidjson::Value;

using Int = int32_t;
using Str = std::string;
using Bool = bool;
constexpr decltype(nullptr) Void = nullptr;

struct Function;

using Value = std::variant<Int, Str, Bool, Function>;
using Context = std::unordered_map<std::string, Value>;

using Array = std::vector<Value>;

struct Function
{
    std::function<Value(Array& args)> call;
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
}

#endif // !TYPES_HPP
