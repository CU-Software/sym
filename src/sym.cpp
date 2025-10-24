#include <math.h>
#include "sym.h"

// To access something in sym.h, start with sym:: for the namespace, then the class name, then the method / value.

// Utility functions
void sym::sym::parse(const std::string& equation) {
    std::string buffer;
    char c;
    for (size_t i = 0; i < equation.length(); ++i) {
        buffer = "";
        c = equation[i];
        if (isspace(c)) {
            continue;
        } else if (isdigit(c) || c == '.') {
            bool hasDecimal = false;
            while (i < equation.length() && (isdigit(c) || c == '.')) {
                if (hasDecimal && c == '.') {
                    throw error("Found more than one decimal point in number");
                } else if (c == '.') {
                    hasDecimal = true;
                }
                buffer += c;
                ++i;
            }
            --i;
            formula_.push_back(buffer);
            continue;    
        } else if (isalpha(c)) {
            while (i < equation.length() && std::isalpha(c)) {
                buffer += c;
                ++i;
            }
            --i;
            formula_.push_back(buffer);
            continue;    
        }
        switch(c) {
            case '*': case '-': case '+': case '/': case '^': case '(': case ')':
                formula_.push_back(std::string(1, c));
                break;
            default: throw error("Invalid character " + std::string(1, c) + " at position " + std::to_string(i + 1));
        }
    }
}

// Constructors
sym::sym::sym()
  : equation_(nullptr) {}

sym::sym::sym(std::string equation)
  : equation_(equation) {
    parse(equation_);
}

sym::sym::~sym() {}

std::string sym::sym::getEquation() {
    return equation_;
}
