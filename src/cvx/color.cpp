#include "cvx/color.hpp"
#include <algorithm>
#include <stdexcept>

namespace cvx {
    color::color()
        : r(0),
          g(0),
          b(0),
          a(0) {}

    color::color(std::uint8_t value)
        : color(value, value, value, 0) {}

    color::color(std::uint8_t r, std::uint8_t g, std::uint8_t b)
        : color(r, g, b, 0) {}

    color::color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
        : r(r),
          g(g),
          b(b),
          a(a) {
        clamp();
    }

    color::color(const color& color)
        : r(color.r),
          g(color.g),
          b(color.b),
          a(color.a) {
        clamp();
    }

    color::color(const color& color, float lerp) {
        lerp = std::min(std::max(0.f, lerp), 1.f);

        r = static_cast<float>(color.r) * lerp;
        g = static_cast<float>(color.g) * lerp;
        b = static_cast<float>(color.b) * lerp;
        a = static_cast<float>(color.a) * lerp;
        clamp();
    }

    color operator+(const color& a, const color& b) {
        return color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
    }

    color operator-(const color& a, const color& b) {
        return color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
    }

    color operator*(const color& a, const color& b) {
        return color(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);
    }

    color operator/(const color& a, const color& b) {
        if (b.r == 0 || b.g == 0 || b.b == 0) {
            throw std::out_of_range("Division by zero");
        }

        return color(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);
    }

    color& color::operator+=(const color& c) {
        r += c.r;
        g += c.g;
        b += c.b;
        a += c.a;

        return *this;
    }

    color& color::operator-=(const color& c) {
        r -= c.r;
        g -= c.g;
        b -= c.b;
        a -= c.a;

        return *this;
    }

    color& color::operator*=(const color& c) {
        r *= c.r;
        g *= c.g;
        b *= c.b;
        a *= c.a;

        return *this;
    }

    color& color::operator/=(const color& c) {
        if (c.r == 0 || c.g == 0 || c.b == 0) {
            throw std::out_of_range("Division by zero");
        }

        r /= c.r;
        g /= c.g;
        b /= c.b;
        a /= c.a;

        return *this;
    }

    bool color::operator==(const color& c) {
        return (r == c.r && g == c.g && b == c.b && a == c.a);
    }

    bool color::operator!=(const color& c) {
        return !(*this == c);
    }

    void color::clamp() {
        r = std::min(std::max(r, static_cast<std::uint8_t>(0)), static_cast<std::uint8_t>(255));
        g = std::min(std::max(g, static_cast<std::uint8_t>(0)), static_cast<std::uint8_t>(255));
        b = std::min(std::max(b, static_cast<std::uint8_t>(0)), static_cast<std::uint8_t>(255));
        a = std::min(std::max(a, static_cast<std::uint8_t>(0)), static_cast<std::uint8_t>(255));
    }

    const color color::black(0, 0, 0, 0);
    const color color::white(255, 255, 255, 0);
    const color color::red(255, 0, 0, 0);
    const color color::green(0, 255, 0, 0);
    const color color::blue(0, 0, 255, 0);
    const color color::cyan(0, 0, 0, 0);
    const color color::magenta(0, 0, 0, 0);
    const color color::yellow(0, 0, 0, 0);
    const color color::gray(0, 0, 0, 0);
} // cvx
