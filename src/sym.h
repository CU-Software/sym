#ifndef SYM_H
#define SYM_H

// Inclusions must go prior to the namespace
#include <exception>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>

namespace sym {

bool isoperator(char c);

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
    void applyOp(T& val1, T val2, char op) {
        switch(op) {
            case '^': val1 = pow(val1, val2); break;
            case '*': val1 *= val2; break;
            case '/': val1 /= val2; break;
            case '+': val1 += val2; break;
            case '-': val1 -= val2; break;
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
    T at(T val, size_t idx = 0) {
        T result = 0;
        char op = 0;

        for (size_t i = idx; i < formula_.size(); ++i) {
            if (formula_[i] == ")") { // Closing Paren
                return result;
            }
            if (formula_[i] == "(") { // Opening Paren
                if (op) {
                    applyOp(result, at(val, i + 1), op);
                    op = 0;
                }
                else {
                    result = at(val, i + 1);
                }
            }
            if (isdigit(formula_[i][0])) { // Number
                double number;
                try {
                    number = std::stod(formula_[i]);
                } catch (std::invalid_argument) {
                    throw error("Failed to convert " + formula_[i] + " to number value");
                }
                if (op) {
                    applyOp(result, static_cast<T>(number), op);
                    op = 0;
                }
                else {
                    result = static_cast<T>(number);
                }
            }
            if (formula_[i] == "x") { // Variable
                if (op) {
                    applyOp(result, val, op);
                    op = 0;
                }
                else {
                    result = val;
                }
            } else if (isalpha(formula_[i][0])) { // Function
                auto it = mathFuncs.find(formula_[i]);
                if (it == mathFuncs.end()) {
                    throw error("Invalid function '" + formula_[i] + "'");
                }
                if (i + 1 < formula_.size() && formula_[i + 1] != "(") {
                    throw error("Missing parenthesis for function '" + formula_[i] + "'");
                }
                if (op) {
                    applyOp(result, static_cast<T>(it->second(at(val, i + 1))), op);
                    op = 0;
                }
                else {
                    result = static_cast<T>(it->second(at(val, i + 1)));
                }
            }
            if (isoperator(formula_[i][0])) { // Operator
                op = formula_[i][0];
            }
            std::cout << "result = " << result << ",\ti = " << i << '\n';
        }

        return result;
    }
};

} // namespace sym

#endif // SYM_H
