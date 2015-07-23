#ifndef CVX_POINT2_HPP
#define CVX_POINT2_HPP

#include "cvx/export.hpp"
#include <cmath>
#include <ostream>
#include <stdexcept>
#include <type_traits>

namespace cvx {
    template<typename T>
    class CVX_EXPORT point2 {
        static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                      "Point coordinate type must be integral or floating-point");

        public:
            //////////////////////////////////////////////////////////////////////
            /// Create a point at (0, 0)
            //////////////////////////////////////////////////////////////////////
            point2()
                : x(T()),
                  y(T()) {}

            //////////////////////////////////////////////////////////////////////
            /// Create a point at (value, value)
            ///
            /// \param value A value
            //////////////////////////////////////////////////////////////////////
            point2(T value)
                : x(value),
                  y(value) {}
            
            //////////////////////////////////////////////////////////////////////
            /// Create a point at (x, y)
            ///
            /// \param x X-coordinate of the point
            /// \param y Y-coordinate of the point
            //////////////////////////////////////////////////////////////////////
            point2(T x, T y)
                : x(x),
                  y(y) {}

            //////////////////////////////////////////////////////////////////////
            /// Copy constructor
            ///
            /// \param point Point to copy
            //////////////////////////////////////////////////////////////////////
            point2(const point2<T>& p)
                : x(p.x),
                  y(p.y) {}

            //////////////////////////////////////////////////////////////////////
            /// Move the point by an offset
            ///
            /// \param dx X-coordinate of the offset
            /// \param dy Y-coordinate of the offset
            //////////////////////////////////////////////////////////////////////
            void move(T dx, T dy) {
                x += dx;
                y += dy;
            }

            point2<T>& operator+=(const point2<T>& p) {
                x += p.x;
                y += p.y;

                return *this;
            }

            point2<T>& operator-=(const point2<T>& p) {
                x -= p.x;
                y -= p.y;

                return *this;
            }

            point2<T>& operator*=(const point2<T>& p) {
                x *= p.x;
                y *= p.y;

                return *this;
            }

            point2<T>& operator/=(const point2<T>& p) {
                if (p.x == static_cast<T>(0) || p.y == static_cast<T>(0)) {
                    throw std::out_of_range("Division by zero");
                }

                x /= p.x;
                y /= p.y;

                return *this;
            }

            bool operator==(const point2<T>& p) const {
                return (x == p.x && y == p.y);
            }

            bool operator!=(const point2<T>& p) const {
                return !(*this == p);
            }

            //////////////////////////////////////////////////////////////////////
            /// Cast the point to another point type
            ///
            /// \param U Another type to cast to
            //////////////////////////////////////////////////////////////////////
            template<typename U>
            operator point2<U>() const {
                return point2<U>(static_cast<U>(x),
                                 static_cast<U>(y));
            }

            //////////////////////////////////////////////////////////////////////
            /// Zero out the point to (0, 0)
            //////////////////////////////////////////////////////////////////////
            void zero() {
                x = y = static_cast<T>(0);
            }

            //////////////////////////////////////////////////////////////////////
            /// \param point Other point to calculate distance to
            /// \return Distance to the given point
            //////////////////////////////////////////////////////////////////////
            T distance(const point2<T>& p) const {
                return std::sqrt(((x - p.x) * (x - p.x)) + ((y - p.y) * (y - p.y)));
            }

        public:
            T x, y;
    };
} // cvx

namespace cvx {
    /// Global binary operators
    template<typename T>
    point2<T> operator+(const point2<T>& a, const point2<T>& b) {
        return point2<T>(a.x + b.x, a.y + b.y);
    }

    template<typename T>
    point2<T> operator-(const point2<T>& a, const point2<T>& b) {
        return point2<T>(a.x - b.x, a.y - b.y);
    }

    template<typename T>
    point2<T> operator*(const point2<T>& a, const point2<T>& b) {
        return point2<T>(a.x * b.x, a.y * b.y);
    }

    template<typename T>
    point2<T> operator/(const point2<T>& a, const point2<T>& b) {
        if (b.x == static_cast<T>(0) || b.y == static_cast<T>(0)) {
            throw std::out_of_range("Division by zero");
        }

        return point2<T>(a.x / b.x, a.y / b.y);
    }
} // cvx

/// Global ostream operator
template<typename T>
std::ostream& operator<<(std::ostream& os, const cvx::point2<T>& p) {
    return (os << "(" << p.x << ", " << p.y << ")");
}

namespace std {
    /// Hash function for points
    template<typename T>
    struct hash<cvx::point2<T>> {
        size_t operator()(const cvx::point2<T>& point) const {
            size_t x_hash = std::hash<T>()(point.x);
            size_t y_hash = std::hash<T>()(point.y);

            return x_hash ^ y_hash;
        }
    };
}

namespace cvx {
    using point2i = point2<int>;
    using point2f = point2<float>;
    using point2d = point2<double>;
} // cvx

#endif // CVX_POINT2_HPP
