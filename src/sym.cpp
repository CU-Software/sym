#include <math.h>

#include "sym.h"

namespace sym {

bool isoperator(char c) {
    return std::string("^*/+-").find(c) != std::string::npos;
}

bool isnumber(const std::string& s) {
    return isdigit(s[0]) || s[0] == '.' ||
    (s.length() > 1 && s[0] == '-' && (isdigit(s[1]) || s[1] == '.'));
}

bool isspecial(const std::string& s) {
    return s == "nan" || s == "inf" || s == "-inf";
}

bool isvariable(const std::string& s) {
    return s == "x"; // TODO support multivariable expression
    // return s.length() == 1 && isalpha(s[0]);
}

bool isfunction(const std::string& s) {
    return s.length() > 1 &&
           std::find_if(s.begin(), s.end(), [](char c)
                        { return !isalpha(c); }) == s.end();
}

// Example usage: mathFuncs.find("cos")->second(M_PI / 3);
const std::unordered_map<std::string, std::function<double(double)>> sym::mathFuncs = {
    {"sin",  [](double x){ return std::sin(x);       }},
    {"csc",  [](double x){ return 1.0 / std::sin(x); }},
    {"cos",  [](double x){ return std::cos(x);       }},
    {"sec",  [](double x){ return 1.0 / std::cos(x); }},
    {"tan",  [](double x){ return std::tan(x);       }},
    {"cot",  [](double x){ return 1.0 / std::tan(x); }},
    {"asin", [](double x){ return std::asin(x);      }},
    {"acos", [](double x){ return std::acos(x);      }},
    {"atan", [](double x){ return std::atan(x);      }},
    {"log",  [](double x){ return std::log10(x);     }},
    {"ln",   [](double x){ return std::log(x);       }},
    {"sqrt", [](double x){ return std::sqrt(x);      }}
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
        if (c == '-' &&
            (isoperator(formula_.back()[0]) || formula_.back()[0] == '('))
        {
            buffer += c;
            c = equation[++i];
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
            }
            if (i > 0 && isdigit(formula_.back()[0])) {
                formula_.push_back("*");
            }
            formula_.push_back(buffer);
            continue;
        }
        if (c == '(' && formula_.size() &&
            !isoperator(formula_.back()[0]) &&
            (isdigit(formula_.back()[0]) || formula_.back().length() == 1))
        {
            formula_.push_back("*");
        }
        switch(c) {
            case '(': case ')': case '^': case '*': case '/': case '+': case '-':
                formula_.push_back(std::string(1, c));
                break;
            default:
                throw error("Invalid character " + std::string(1, c) + " at position " + std::to_string(i + 1));
        }
    }
    for (size_t i = 0; i < formula_.size(); ++i) { // Simplify parenthesized numbers
        if (i + 2 < formula_.size() &&
            formula_[i][0] == '(' &&
            (isnumber(formula_[i + 1]) || isvariable(formula_[i + 1])) &&
            formula_[i + 2][0] == ')')
        {
            formula_[i] = formula_[i + 1];
            formula_.erase(formula_.begin() + i + 1, formula_.begin() + i + 3);
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
const std::string& sym::getEquation() const {
    return equation_;
}

const std::vector<std::string>& sym::getTokens() const {
    return formula_;
}

} // namespace sym
