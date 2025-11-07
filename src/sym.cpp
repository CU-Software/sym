#include <math.h>

#include "sym.h"

namespace sym {

bool isoperator(char c) {
    return std::string("^*/+-").find(c) != std::string::npos;
}

const std::unordered_map<std::string, std::function<double(double)>> sym::mathFuncs = {
    {"sin",  [](double x){ return std::sin(x);   }},
    {"cos",  [](double x){ return std::cos(x);   }},
    {"tan",  [](double x){ return std::tan(x);   }},
    {"asin", [](double x){ return std::asin(x);  }},
    {"acos", [](double x){ return std::acos(x);  }},
    {"atan", [](double x){ return std::atan(x);  }},
    {"log",  [](double x){ return std::log10(x); }},
    {"ln",   [](double x){ return std::log(x);   }}
};

// Utility functions
void sym::tokenize(const std::string& equation) {
    std::string buffer;
    char c;
    for (size_t i = 0; i < equation.length(); ++i) {
        buffer = "";
        c = equation[i];
        if (isspace(c)) {
            continue;
        }
        if (isdigit(c) || c == '.') {
            bool hasDecimal = false;
            while (i < equation.length() && (isdigit(c) || c == '.')) {
                if (hasDecimal && c == '.') {
                    throw error("Found more than one decimal point in number at index " + std::to_string(i));
                } else if (c == '.') {
                    hasDecimal = true;
                }
                buffer += c;
                ++i;
                c = equation[i];
            }
            --i;
            formula_.push_back(buffer);
            continue;    
        }
        if (isalpha(c)) {
            while (i < equation.length() && std::isalpha(c)) {
                buffer += c;
                ++i;
                c = equation[i];
            }
            --i;
            if (buffer.length() == 1) {
                if (buffer[0] != 'x') {
                    throw error("Variables other than 'x' are not supported");
                }
                formula_.push_back("*");
            }
            formula_.push_back(buffer);
            continue;    
        }
        switch(c) {
            case '(': case ')': case '^': case '*': case '/': case '+': case '-':
                formula_.push_back(std::string(1, c));
                break;
            default: throw error("Invalid character " + std::string(1, c) + " at position " + std::to_string(i + 1));
        }
    }
}

// Constructors
sym::sym()
  : equation_(nullptr) {}

sym::sym(std::string equation)
  : equation_(equation) {
    tokenize(equation_);
}

sym::~sym() {}

// Public functions
std::string sym::getEquation() {
    return equation_;
}

std::vector<std::string> sym::getTokens() {
    return formula_;
}

} // namespace sym
