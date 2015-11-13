#ifndef CVX_MAIN_HPP
#define CVX_MAIN_HPP

#include <string>

#define CVX_MAJOR_VERSION 0
#define CVX_MINOR_VERSION 1
#define CVX_REVISION      0

namespace cvx {
    const std::string version_string = "0.1.0";
    constexpr unsigned int major_version  = 0;
    constexpr unsigned int minor_version  = 1;
    constexpr unsigned int revision       = 0;
}

//#include "cvx/algorithms.hpp"
#include "cvx/ccl.hpp"
#include "cvx/color.hpp"
#include "cvx/connected_component.hpp"
#include "cvx/draw.hpp"
//#include "cvx/ellispe.hpp"
#include "cvx/exception.hpp"
#include "cvx/feature_flag.hpp"
#include "cvx/point2.hpp"
#include "cvx/rectangle2.hpp"

#endif // CVX_MAIN_HPP
