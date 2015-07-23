#include "cvx/draw.hpp"
#include "cvx/exception.hpp"
#include <cstdint>
#include <random>

namespace cvx {
    color random_color() {
        std::random_device rnd;
        std::uniform_int_distribution<std::uint8_t> uid(0, 255);

        return color(uid(rnd), uid(rnd), uid(rnd));
    }

    #ifdef CVX_WITH_OPENCV
        namespace detail {
            cv::Scalar color_to_scalar(const color& color) {
                return cv::Scalar(color.r, color.g, color.b, color.a);
            }
        }

        void wait_for_key() {
            cv::waitKey(0);
        }
    #else
        void wait_for_key() {
            throw exception("Cannot draw, OpenCV is not available");
        }
    #endif // CVX_WITH_OPENCV
} // cvx
