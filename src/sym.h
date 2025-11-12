#ifndef SYM_H
#define SYM_H

// usage: DEBUG(TITLE) shows formula tokens at current evaluation point in at()
#define DEBUG(x) printf("\033[32m--%8s:\t", #x); for (auto token : formula) { std::cout << token << " "; } puts("\33[m");

// Inclusions must go prior to the namespace
#include <exception>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>

namespace sym {

bool isfunction(std::string f);

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

    std::string getEquation();
    std::vector<std::string> getTokens();

    template <typename T>
    T at(T val) {
        std::vector<std::string> formula(formula_);
        for (size_t i = 0; i < formula.size(); ++i) { // Variable subsitution
            if (formula[i] == "x") {
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
                DEBUG(OPEN)
            }
        }
        for (size_t i = idx; i < formula.size(); ++i) { // Exponent
            if (formula[i][0] == ')') {
                formula.erase(formula.begin() + idx, formula.begin() + i + 1);
                DEBUG(CLOSE@E)
                return std::stod(formula[idx]);
            }
            if (formula[i][0] == '^') {
                double val1 = std::stod(formula[i - 1]);
                double val2 = std::stod(formula[i + 1]);
                formula[i - 1] = std::to_string(applyOp(val1, val2, formula[i][0]));
                formula.erase(formula.begin() + i, formula.begin() + i + 2);
                --i;
                DEBUG(EXPO)
            }
        }
        for (size_t i = idx; i < formula.size(); ++i) { // Multiply/Divide
            if (formula[i][0] == ')') {
                formula.erase(formula.begin() + idx, formula.begin() + i + 1);
                DEBUG(CLOSE@MD)
                return std::stod(formula[idx]);
            }
            if (formula[i][0] == '*' || formula[i][0] == '/') {
                double val1 = std::stod(formula[i - 1]);
                double val2 = std::stod(formula[i + 1]);
                formula[i - 1] = std::to_string(applyOp(val1, val2, formula[i][0]));
                formula.erase(formula.begin() + i, formula.begin() + i + 2);
                --i;
                DEBUG(MD)
            }
        }
        for (size_t i = idx; i < formula.size(); ++i) { // Add/Subtract
            if (formula[i][0] == ')') {
                formula.erase(formula.begin() + idx, formula.begin() + i + 1);
                DEBUG(CLOSE@AS)
                return std::stod(formula[idx]);
            }
            if (formula[i][0] == '+' || formula[i][0] == '-') {
                double val1 = std::stod(formula[i - 1]);
                double val2 = std::stod(formula[i + 1]);
                formula[i - 1] = std::to_string(applyOp(val1, val2, formula[i][0]));
                formula.erase(formula.begin() + i, formula.begin() + i + 2);
                --i;
                DEBUG(AS)
            }
        }
        return std::stod(formula[idx]);
    }
};

} // namespace sym

#endif // SYM_H
