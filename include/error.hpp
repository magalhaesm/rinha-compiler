#ifndef ERROR_HPP

#include <stdexcept>

#include "types.hpp"

class GenericError : public std::exception
{
public:
    GenericError(const char* msg, const Node& location);
    std::string constructErrorMessage(const char* msg, const Node& node);
    const char* what() const throw();

private:
    std::string m_msg;
};

class BinaryOperationError : public GenericError
{
public:
    BinaryOperationError(const char* msg, const Node& node);
};

class UnrecognizedTerm : public GenericError
{
public:
    UnrecognizedTerm(const Node& node);
};

class UndeclaredIdentifier : public GenericError
{
public:
    UndeclaredIdentifier(const Node& node);
};

class InvalidArguments : public GenericError
{
public:
    InvalidArguments(const Node& node);
};

#endif // !ERROR_HPP
