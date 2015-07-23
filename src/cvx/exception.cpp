#include "cvx/exception.hpp"

namespace cvx {
    exception::exception(const std::string& msg)
        : std::runtime_error(msg) {
    }

    exception::exception(const char* msg)
        : std::runtime_error(msg) {
    }
}
