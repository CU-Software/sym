#include <iostream>
#include <math.h>

// This is our library
#include "sym.h"

// Please don't use namespace std, we have a sym workspace
// using namespace std;

// Testing function for the sym object, feel free to ignore these
/*
template<typename T>
T functionF(T val) {
   return 2 * std::pow(val, 3) + 3 * std::pow(val, 2) - 5 * val; 
}

bool test() {
    try {
        sym::sym f("2x^3 + 3x^2 - 5x - ( 3 * 0 )"); // We make function f
        for ( int i = 0; i < 100; i++ ) {
            if ( f.at(i) != functionF(i) ) {
                return false;
            }
        }
    } catch ( sym::error e ) {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}
*/

int main(int argc, char** argv) {
    std::cout << "Hello CUSEC Devs!" << std::endl;
    std::cout << "Hello World!" << std::endl;
    return 0;
}
