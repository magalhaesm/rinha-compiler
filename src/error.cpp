#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "types.hpp"
#include "error.hpp"

const std::string RED = "\x1b[1;31m";
const std::string GRAY = "\x1b[1;90m";
const std::string RESET = "\x1b[0m";

static int getLinebreaksAt(int start, std::ifstream& file);

BinaryOperationError::BinaryOperationError(const char* msg, const Node& node)
    : GenericError(msg, node)
{
}

UnrecognizedTerm::UnrecognizedTerm(const Node& node)
    : GenericError("unrecognized term", node)
{
}

UndeclaredIdentifier::UndeclaredIdentifier(const Node& node)
    : GenericError("undeclared identifier in this scope", node)
{
}

InvalidArguments::InvalidArguments(const Node& node)
    : GenericError("invalid arguments", node)
{
}

GenericError::GenericError(const char* msg, const Node& node)
    : m_msg(constructErrorMessage(msg, node))
{
}

std::string GenericError::constructErrorMessage(const char* msg, const Node& node)
{
    std::string error_message;

    try
    {
        const Node& location = node["location"];
        int start = location["start"].GetInt();
        int length = location["end"].GetInt() - start;

        std::ifstream file(location["filename"].GetString(), std::ios::binary);
        int lineNum = getLinebreaksAt(start, file);

        std::string term;
        term.resize(length);
        file.read(&term[0], length);

        std::string ctxMessage = "\n  ";
        ctxMessage += GRAY + std::to_string(lineNum) + " |  " + RESET + term;

        error_message = RED + "Error: " + RESET + msg + ctxMessage;

        file.close();
    }
    catch (const std::exception&)
    {
        error_message = RED + "Error: " + RESET + msg;
    }

    return error_message;
}

const char* GenericError::what() const throw()
{
    return m_msg.c_str();
}

int getLinebreaksAt(int start, std::ifstream& file)
{
    char chr;
    int linebreaks = 1;
    while (file.get(chr) && file.tellg() < start)
    {
        if (chr == '\n')
        {
            ++linebreaks;
        }
    }
    return linebreaks;
}
