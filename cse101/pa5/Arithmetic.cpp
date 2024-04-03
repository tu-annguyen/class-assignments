//---------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE101 PA5
// Arithmetic.cpp 
// Top-level client and main program for PA5. 
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include "BigInteger.h"

int main(int argc, char **argv) {
    std::ifstream in;
    std::ofstream out;

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
        return(EXIT_FAILURE);
    }

    in.open(argv[1]);
    if(!in.is_open()){
        std::cerr << "Unable to open file " << argv[1] << " for reading" << std::endl;
        return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if(!out.is_open()){
        std::cerr << "Unable to open file " << argv[2] << " for writing" << std::endl;
        return(EXIT_FAILURE);
    }

    std::string line;
    BigInteger A, B;

    // Store first line in A.
    std::getline(in, line);
    A = BigInteger(line);

    // Skip the blank line.
    std::getline(in, line);

    // Store first line in B.
    std::getline(in, line);
    B = BigInteger(line);

    out << A << std::endl << std::endl;
    out << B << std::endl << std::endl;
    out << A + B << std::endl << std::endl;
    out << A - B << std::endl << std::endl;
    out << A - A << std::endl << std::endl;
}