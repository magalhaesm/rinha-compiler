#include <memory>

#include "types.hpp"
#include "error.hpp"
#include "interpreter.hpp"

static Array getParams(const Node& node);

Int Type::Int(const Node& node)
{
    return node["value"].GetInt();
}

Str Type::Str(const Node& node)
{
    return node["value"].GetString();
}

Bool Type::Bool(const Node& node)
{
    return node["value"].GetBool();
}

Function Type::Function(const Node& node, const Context& ctx)
{
    struct Closure fn;
    fn.call = [&](Array& args)
    {
        Context newCtx(ctx);

        auto params = getParams(node);
        if (args.size() != params.size())
        {
            throw std::runtime_error("");
        }

        for (size_t idx = 0; idx < params.size(); ++idx)
        {
            auto text = std::get< ::Str>(params[idx]);
            newCtx[text] = args[idx];
        }

        return eval(node["value"], newCtx);
    };
    return std::make_shared<Closure>(fn);
}

Tuple Type::Tuple(const Node& node, Context& ctx)
{
    auto first = eval(node["first"], ctx);
    auto second = eval(node["second"], ctx);

    struct Tuple tuple
    {
        std::make_shared<Value>(first), std::make_shared<Value>(second),
    };
    return tuple;
}

inline Array getParams(const Node& node)
{
    auto params = node["parameters"].GetArray();
    Array arr;

    for (auto& param : params)
    {
        arr.push_back(param["text"].GetString());
    }
    return arr;
}

std::string Type::to_string(const ::Value& value)
{
    return std::visit([](const auto& v) { return Type::to_string(v); }, value);
}

inline std::string Type::to_string(const ::Str& str)
{
    return str;
}

inline std::string Type::to_string(const ::Int& integer)
{
    return std::to_string(integer);
}

inline std::string Type::to_string(const ::Bool& boolean)
{
    return std::to_string(boolean);
}

inline std::string Type::to_string(const ::Function&)
{
    return "<#closure>";
}

inline std::string Type::to_string(const ::Tuple& tuple)
{
    return "(" + Type::to_string(*(tuple.first)) + ", " + Type::to_string(*(tuple.second)) + ")";
}
