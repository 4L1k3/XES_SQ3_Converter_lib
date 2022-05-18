#include <iostream>
#include <string>

#include "includes/XesToSq3Converter.hpp"
#include "includes/Sq3ToXesConverter.hpp"


int main (int argc, char* argv[]) {
    if (argc != 3)
    {
        std::cout << "arguments error\nneed 3 arguments" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "to_sq3") {
        return xes_to_sq3(argv[2]);
    }
    if (mode == "to_xes") {
        return sq3_to_xes(argv[2], "test.xes");
    }
}
