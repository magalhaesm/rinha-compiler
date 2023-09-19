#include "types.hpp"
#include "interpreter.hpp"

int32_t Type::Int(const Node& node)
{
    return node["value"].GetInt();
}

std::string Type::Str(const Node& node)
{
    return node["value"].GetString();
}

bool Type::Bool(const Node& node)
{
    return node["value"].GetBool();
}

Function Type::Function(const Node& node, const Context& ctx)
{
    struct Function fn;
    fn.call = [&](Array& args)
    {
        Context newCtx(ctx);

        auto params = getParams(node);
        if (args.size() != params.size())
        {
            throw std::runtime_error("Invalid parameters");
        }

        for (size_t idx = 0; idx < params.size(); ++idx)
        {
            auto text = std::get<std::string>(params[idx]);
            newCtx[text] = args[idx];
        }

        return eval(node["value"], newCtx);
    };
    return fn;
}

Array getParams(const Node& node)
{
    auto params = node["parameters"].GetArray();
    Array arr;

    for (auto& param : params)
    {
        arr.push_back(param["text"].GetString());
    }
    return arr;
}

Array getArgs(const Node& node, Context& ctx)
{
    auto args = node["arguments"].GetArray();
    Array arr;

    for (auto& arg : args)
    {
        arr.push_back(eval(arg, ctx));
    }
    return arr;
}
