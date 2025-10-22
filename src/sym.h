#ifndef SYM_H
#define SYM_H

// Inclusions must go prior to the namespace
#include <exception>
#include <string>

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

    public:
        sym();
        ~sym();
    };
}

#endif // SYM_H
