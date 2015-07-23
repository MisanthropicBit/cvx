#ifndef CVX_COLOR_HPP
#define CVX_COLOR_HPP

#include "cvx/export.hpp"
#include <cstdint>

namespace cvx {
    class CVX_EXPORT color final {
        public:
            //////////////////////////////////////////////////////////////////////
            /// Create a default color (black)
            //////////////////////////////////////////////////////////////////////
            color();
            
            //////////////////////////////////////////////////////////////////////
            /// Create a grayscale color
            ///
            /// \param value Value assigned to all color components
            //////////////////////////////////////////////////////////////////////
            color(std::uint8_t value);
            
            //////////////////////////////////////////////////////////////////////
            /// Create a grayscale color
            ///
            /// \param value Value assigned to all color components
            /// \param alpha Transparency of the color
            //////////////////////////////////////////////////////////////////////
            color(std::uint8_t value, std::uint8_t alpha);
            
            //////////////////////////////////////////////////////////////////////
            /// Create a grayscale color
            ///
            /// \param red   Value of the red component
            /// \param green Value of the green component
            /// \param blue  Value of the blue component
            //////////////////////////////////////////////////////////////////////
            color(std::uint8_t r, std::uint8_t g, std::uint8_t b);
            
            //////////////////////////////////////////////////////////////////////
            /// Create a grayscale color
            ///
            /// \param red   Value of the red component
            /// \param green Value of the green component
            /// \param blue  Value of the blue component
            /// \param alpha Value of the alpha component
            //////////////////////////////////////////////////////////////////////
            color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
            
            //////////////////////////////////////////////////////////////////////
            /// Copy constructor
            ///
            /// \param color Another color to copy
            //////////////////////////////////////////////////////////////////////
            color(const color& color);
            
            //////////////////////////////////////////////////////////////////////
            /// Copy another color and adjust it through linear interpolation
            ///
            /// \param color Another color to copy
            /// \param lerp  Linear interpolation value ([0, 1])
            //////////////////////////////////////////////////////////////////////
            color(const color& color, float lerp);

            color& operator+=(const color& p);
            color& operator-=(const color& p);
            color& operator*=(const color& p);
            color& operator/=(const color& p);
            bool operator==(const color& p);
            bool operator!=(const color& p);

        public:
            /// Predefined colors
            static const color black;
            static const color white;
            static const color red;
            static const color green;
            static const color blue;
            static const color cyan;
            static const color magenta;
            static const color yellow;
            static const color gray;

        public:
            std::uint8_t r, g, b, a;

        private:
            void clamp();
    };

    /// Global binary operators
    color operator+(const color& a, const color& b);
    color operator-(const color& a, const color& b);
    color operator*(const color& a, const color& b);
    color operator/(const color& a, const color& b);
} // cvx

#endif // CVX_COLOR_HPP
