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

int main(/* int argc, char** argv */) {
    std::cout << "Hello CUSEC Devs!" << std::endl;

    sym::sym eq("3x^2+11-1");

    std::cout << "Tokenized equation -> ";
    auto tokens = eq.getTokens();
    for (auto&& tok : tokens) {
        std::cout << tok << ", ";
    }
    std::cout << std::endl;

    double in = 0;
    while (in != 67) {
        printf("value @ %g is %g\n", in, eq.at(in));
        printf("Enter next value (67 to quit): ");
        scanf("%lf", &in);
    }

    return 0;
}
