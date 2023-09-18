#include "types.hpp"

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
