#ifndef SYM_H
#define SYM_H

// usage: DEBUG(TITLE) shows formula tokens at current evaluation point in at()
#if 0
    #define DEBUG(x) printf("\033[32m-- %4s:  ", #x); for (auto token : formula) { std::cout << token << " "; } puts("\33[m");
#else
    #define DEBUG(x)
#endif

// Inclusions must go prior to the namespace
#include <exception>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>

namespace sym {

bool isoperator(char c);
bool isnumber(const std::string& s);
bool isspecial(const std::string& s);
bool isvariable(const std::string& s);
bool isfunction(const std::string& s);

class error : public std::exception {
private:
    std::string msg_;
public:
    error(const std::string& msg) : msg_(msg) {}
    const char* what() const noexcept override {
        return msg_.c_str();
    }
};

class sym {
private:
    std::string equation_;
    std::vector<std::string> formula_;
    static const std::unordered_map<std::string, std::function<double(double)>> mathFuncs;

    void tokenize(const std::string& equation);

    template <typename T>
    T applyOp(T val1, T val2, char op) {
        switch(op) {
            case '^': return pow(val1, val2);
            case '*': return val1 * val2;
            case '/': return val1 / val2;
            case '+': return val1 + val2;
            case '-': return val1 - val2;
            default: throw error("Invalid operator " + std::string(1, op));
        }
    }

public:
    sym();
    sym(std::string equation);
    ~sym();

    const std::string& getEquation() const;
    const std::vector<std::string>& getTokens() const;

    template <typename T>
    T at(T val) {
        std::vector<std::string> formula(formula_);
        for (size_t i = 0; i < formula.size(); ++i) { // Variable subsitution
            if (isvariable(formula[i])) {
                formula[i] = std::to_string(val);
            }
        }
        DEBUG(INIT)
        return at(val, formula);
    }

    template <typename T>
    T at(T val, std::vector<std::string>& formula, size_t idx = 0) {
        for (size_t i = idx; i < formula.size(); ++i) { // Opening Paren
            if (formula[i] == "(") {
                formula[i] = std::to_string(at(val, formula, i + 1));
                formula.erase(formula.begin() + i + 1, formula.begin() + i + 3);
                DEBUG(OPEN)
            }
        }
        for (size_t i = idx; i < formula.size(); ++i) { // Function
            if (isfunction(formula[i]) && !isspecial(formula[i])) {
                auto it = mathFuncs.find(formula[i]);
                if (it == mathFuncs.end()) {
                    throw error("Invalid function call: '" + formula[i] + "'");
                } else if (i + 1 >= formula.size()) {
                    throw error("No arguments to function: '" + formula[i] + "'");
                }
                formula[i] = std::to_string(it->second(std::stod(formula[i + 1])));
                formula.erase(formula.begin() + i + 1);
                DEBUG(FUNC)
            }
        }
        for (size_t i = idx; i < formula.size() && formula[i][0] != ')'; ++i) { // Exponent
            if (formula[i][0] == '^') {
                double val1 = std::stod(formula[i - 1]);
                double val2 = std::stod(formula[i + 1]);
                formula[i - 1] = std::to_string(applyOp(val1, val2, formula[i][0]));
                formula.erase(formula.begin() + i, formula.begin() + i + 2);
                --i;
                DEBUG(EXPO)
            }
        }
        for (size_t i = idx; i < formula.size() && formula[i][0] != ')'; ++i) { // Multiply/Divide
            if (formula[i][0] == '*' || formula[i][0] == '/') {
                double val1 = std::stod(formula[i - 1]);
                double val2 = std::stod(formula[i + 1]);
                formula[i - 1] = std::to_string(applyOp(val1, val2, formula[i][0]));
                formula.erase(formula.begin() + i, formula.begin() + i + 2);
                --i;
                DEBUG(MD)
            }
        }
        for (size_t i = idx; i < formula.size() && formula[i][0] != ')'; ++i) { // Add/Subtract
            if (formula[i][0] == '+' ||
                (formula[i][0] == '-' && !isnumber(formula[i]) && !isspecial(formula[i])))
            {
                double val1 = std::stod(formula[i - 1]);
                double val2 = std::stod(formula[i + 1]);
                formula[i - 1] = std::to_string(applyOp(val1, val2, formula[i][0]));
                formula.erase(formula.begin() + i, formula.begin() + i + 2);
                --i;
                DEBUG(AS)
            }
        }
        
        double result = std::stod(formula[idx]);
        if (result == 0) // correct for negative zero
            return 0;
        return result;
    }
};

} // namespace sym

#endif // SYM_H
