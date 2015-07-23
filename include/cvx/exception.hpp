#ifndef CVX_EXCEPTION_HPP
#define CVX_EXCEPTION_HPP

#include "cvx/export.hpp"
#include <stdexcept>

namespace cvx {
    class CVX_EXPORT exception final : public std::runtime_error {
        public:
            explicit exception(const std::string& msg);
            explicit exception(const char* msg);
            ~exception() = default;
    };
} // cvx

#endif // CVX_EXCEPTION_HPP
