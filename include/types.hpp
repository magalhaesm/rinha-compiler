#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>
#include <variant>
#include <unordered_map>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

using rapidjson::Document;
using Node = rapidjson::Value;

using Int = int32_t;
using Str = std::string;
using Bool = bool;

using Value = std::variant<Int, Str, Bool>;
using Context = std::unordered_map<std::string, Value>;

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
    int32_t Int(const Node& node);
    std::string Str(const Node& node);
    bool Bool(const Node& node);
}

#endif // !TYPES_HPP
