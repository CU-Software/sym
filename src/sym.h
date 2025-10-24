#ifndef SYM_H
#define SYM_H

// Inclusions must go prior to the namespace
#include <exception>
#include <string>
#include <vector>

namespace sym {
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
        std::string rawEquation_;
        std::vector<std::string> formula_;
    public:
        sym();
        sym(std::string equation);
        ~sym();

        std::string getRawEquation();

        template<typename T>
        T at(T val);
    };
}

#endif // SYM_H
