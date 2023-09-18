#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unordered_map>

#include "interpreter.hpp"

std::unordered_map<std::string, Kind> kindTable = {

    { "Int", Kind::Int },
    { "Str", Kind::Str },
    { "Bool", Kind::Bool },
    { "Print", Kind::Print },
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
    }
    throw std::runtime_error("Unrecognized expression");
}

Kind match(const Node& node)
{
    return kindTable[node["kind"].GetString()];
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
