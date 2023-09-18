#include <cstdlib>
#include <iostream>

#include "interpreter.hpp"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <JSON>" << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        Document parsed = parse(argv[1]);
        eval(parsed["expression"]);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Erro: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
