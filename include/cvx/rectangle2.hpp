#ifndef CVX_RECTANGLE_HPP
#define CVX_RECTANGLE_HPP

#include "cvx/export.hpp"
#include "cvx/point2.hpp"
#include <algorithm>

namespace cvx {
    template<typename T>
    class CVX_EXPORT rectangle2 {
        static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                      "Type must be integral or floating-point");

        public:
            rectangle2()
                : x(T()),
                  y(T()),
                  width(T()),
                  height(T()) {}

            rectangle2(T x, T y)
                : x(x),
                  y(y),
                  width(T()),
                  height(T()) {}
            
            rectangle2(T x, T y, T width, T height)
                : x(x),
                  y(y),
                  width(width),
                  height(height) {}

            rectangle2(const rectangle2<T>& rect) {
                x      = rect.x;
                y      = rect.y;
                width  = rect.width;
                height = rect.height;
            }

            bool contains(T a, T b) const noexcept {
                return a >= x && a <= right() && b >= y && b <= bottom();
            }

            template<typename U>
            bool contains(const point2<U>& p) const {
                return contains(p.x, p.y);
            }

            T area() const {
                return width * height;
            }

            float aspect_ratio() const {
                return width / height;
            }

            void move(T dx, T dy) {
                x += dx;
                y += dy;
            }

            void adjust(T x, T y) {
                if (!contains(x, y)) {
                    this->x = std::min(this->x, x);
                    this->y = std::min(this->y, y);

                    width   = std::max(this->x + width, x) - this->x;
                    height  = std::max(this->y + height, y) - this->y;
                }
            }

            void adjust(const point2<T>& p) {
                adjust(p.x, p.y);
            }

            inline T right() const noexcept {
                return x + width;
            }

            inline T bottom() const noexcept {
                return y + height;
            }

            bool operator==(const rectangle2<T>& rhs) {
                return x == rhs.x &&
                       y == rhs.y &&
                       width == rhs.width &&
                       height == rhs.height;
            }

        public:
            T x, y, width, height;
    };
} // cvx

template<typename T>
std::ostream& operator<<(std::ostream& os, const cvx::rectangle2<T>& r) {
    return (os << "rectangle2(" << r.x << ", " << r.y << ", " << r.width << ", " << r.height << ")");
}

namespace cvx {
    using rectangle2i = rectangle2<int>;
    using rectangle2f = rectangle2<float>;
    using rectangle2d = rectangle2<double>;
} // cvx

#endif // CVX_RECTANGLE_HPP
