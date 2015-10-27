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
            //////////////////////////////////////////////////////////////////////
            /// Create a rectangle with the default values of the template type
            //////////////////////////////////////////////////////////////////////
            rectangle2()
                : x(T()),
                  y(T()),
                  width(T()),
                  height(T()) {}

            //////////////////////////////////////////////////////////////////////
            /// Create a rectangle at (x, y), but without a width and height
            ///
            /// \param x      x-coordinate of the rectangle
            /// \param y      y-coordinate of the rectangle
            //////////////////////////////////////////////////////////////////////
            rectangle2(T x, T y)
                : x(x),
                  y(y),
                  width(T()),
                  height(T()) {}
            
            //////////////////////////////////////////////////////////////////////
            /// Create a rectangle at (x, y) with a width and height
            ///
            /// \param x      x-coordinate of the rectangle
            /// \param y      y-coordinate of the rectangle
            /// \param width  Width of the rectangle
            /// \param height Height of the rectangle
            //////////////////////////////////////////////////////////////////////
            rectangle2(T x, T y, T width, T height)
                : x(x),
                  y(y),
                  width(width),
                  height(height) {}

            //////////////////////////////////////////////////////////////////////
            /// Create a rectangle from another rectangle
            ///
            /// \param rect The other rectangle to base this one on
            //////////////////////////////////////////////////////////////////////
            rectangle2(const rectangle2<T>& rectangle) {
                x = rectangle.x;
                y = rectangle.y;
                width = rectangle.width;
                height = rectangle.height;
            }

            //////////////////////////////////////////////////////////////////////
            /// Create a rectangle from another rectangle of another type
            ///
            /// \param rect The other rectangle to base this one on
            //////////////////////////////////////////////////////////////////////
            template<typename U>
            rectangle2(const rectangle2<U>& rectangle) {
                x = static_cast<U>(rectangle.x);
                y = static_cast<U>(rectangle.y);
                width = static_cast<U>(rectangle.width);
                height = static_cast<U>(rectangle.height);
            }

            //////////////////////////////////////////////////////////////////////
            /// Checks if a point is contained within this rectangle
            ///
            /// \param x x-coordinate of a point
            /// \param y y-coordinate of a point
            /// \return Whether or not the point is contained in the rectangle
            //////////////////////////////////////////////////////////////////////
            bool contains(T x, T y) const noexcept {
                return x >= this->x && x <= right() && y >= this->y && y <= bottom();
            }

            //////////////////////////////////////////////////////////////////////
            /// Checks if a point is contained within this rectangle
            ///
            /// \param U Coordinate type of the point
            /// \param p A point
            /// \return Whether or not the point is contained in the rectangle
            //////////////////////////////////////////////////////////////////////
            template<typename U>
            bool contains(const point2<U>& p) const {
                return contains(p.x, p.y);
            }

            //////////////////////////////////////////////////////////////////////
            /// Checks if another rectangle is contained within this one
            ///
            /// \param rectangle A rectangle
            /// \return Whether or not the rectnalge is contained in this rectangle
            //////////////////////////////////////////////////////////////////////
            bool contains(const rectangle2<T>& rectangle) {
                return (x < rectangle.x &&
                        y < rectangle.y &&
                        right() > rectangle.right() &&
                        bottom() > rectangle.bottom());
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The area of the rectangle
            //////////////////////////////////////////////////////////////////////
            T area() const {
                return width * height;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The aspect ratio (width / height) of the rectangle
            //////////////////////////////////////////////////////////////////////
            template<typename Precision = float>
            float aspect_ratio() const {
                return static_cast<Precision>(width) / static_cast<Precision>(height);
            }

            //////////////////////////////////////////////////////////////////////
            /// Move the rectangle by an offset
            ///
            /// \param dx Amount to move in the x-direction
            /// \param dy Amount to move in the y-direction
            //////////////////////////////////////////////////////////////////////
            void move(T dx, T dy) {
                x += dx;
                y += dy;
            }

            //////////////////////////////////////////////////////////////////////
            /// Adjust the rectangle to fit a given point
            ///
            /// \param x x-coordinate of the point
            /// \param y y-coordinate of the point
            //////////////////////////////////////////////////////////////////////
            void adjust(T x, T y) {
                if (!contains(x, y)) {
                    this->x = std::min(this->x, x);
                    this->y = std::min(this->y, y);

                    width   = std::max(this->x + width, x) - this->x;
                    height  = std::max(this->y + height, y) - this->y;
                }
            }

            //////////////////////////////////////////////////////////////////////
            /// Adjust the rectangle to fit a given point
            ///
            /// \param x x-coordinate of the point
            /// \param y y-coordinate of the point
            //////////////////////////////////////////////////////////////////////
            void adjust(const point2<T>& p) {
                adjust(p.x, p.y);
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The x-coordinate of the right side of the rectangle
            //////////////////////////////////////////////////////////////////////
            inline T right() const noexcept {
                return x + width;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The y-coordinate of the bottom side of the rectangle
            //////////////////////////////////////////////////////////////////////
            inline T bottom() const noexcept {
                return y + height;
            }

            //////////////////////////////////////////////////////////////////////
            /// Compare two rectangles
            ///
            /// \param rhs The other rectangle to compare this with
            /// \return True if the rectangles have the same coordinates and
            ///         dimensions, false otherwise
            //////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
/// Output a rectangle to an output stream
///
/// \param os        An output stream
/// \param rectangle The rectangle to output
/// \return A reference to the output stream after outputting the
///         rectangle
//////////////////////////////////////////////////////////////////////
template<typename T>
std::ostream& operator<<(std::ostream& os, const cvx::rectangle2<T>& rectangle) {
    return (os << "rectangle2(" << rectangle.x << ", " << rectangle.y << ", " << rectangle.width << ", " << rectangle.height << ")");
}

namespace cvx {
    using rectangle2i = rectangle2<int>;
    using rectangle2f = rectangle2<float>;
    using rectangle2d = rectangle2<double>;
} // cvx

#endif // CVX_RECTANGLE_HPP
