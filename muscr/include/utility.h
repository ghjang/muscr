#ifndef MUSCR_UTILITY_H
#define MUSCR_UTILITY_H


#include <string>
#include <iostream>
#include <fstream>


inline std::string read_from_file(char const* infile)
{
    std::ifstream instream(infile);
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << infile << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);      // No white space skipping!
    return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
                       std::istreambuf_iterator<char>());
}


#endif //MUSCR_UTILITY_H

