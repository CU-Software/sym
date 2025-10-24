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
        std::string equation_;
        std::vector<std::string> formula_;

        void parse(const std::string& equation);
    public:
        sym();
        sym(std::string equation);
        ~sym();

        std::string getEquation();

        template<typename T>
        T at(T val, size_t idx);
        template<typename T>
        T at(T val);
    };
}

#endif // SYM_H
