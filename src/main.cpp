#include <iostream>

// This is our library
#include "sym.h"

// Please don't use namespace std, we have a sym workspace
// using namespace std;

int main(int argc, char** argv) {
    std::cout << "Testing\n";
    
    sym::sym f("2x+3");
    for ( const auto& v : f.formula_ ) { std::cout << v << std::endl; }

    std::cout << std::endl;
    return 0;
}
