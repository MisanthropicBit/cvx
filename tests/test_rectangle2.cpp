#include <cvx.hpp>
#include <assert.h>

void assert_approx_equals(float a, float b, float epsilon) {
    assert(std::fabs(a - b) <= epsilon);
}

int main() {
    // TODO: Test the adjust method

    const float epsilon = 10e-12;

    cvx::rectangle2i rectangle1;
    cvx::rectangle2d rectangle2(20., 40.);
    cvx::rectangle2<long> rectangle3(10, -10, 50, 30);
    cvx::rectangle2f rectangle4(rectangle2);

    assert(rectangle1.x == int());
    assert(rectangle1.y == int());
    assert(rectangle1.width == int());
    assert(rectangle1.height == int());
    assert(rectangle1.area() == 0);
    assert(rectangle1.right() == 0);
    assert(rectangle1.bottom() == 0);

    assert(rectangle2.x == 20.);
    assert(rectangle2.y == 40.);
    assert(rectangle2.width == int());
    assert(rectangle2.height == int());
    assert(rectangle2.area() == 0.);
    assert(rectangle2.right() == 20.);
    assert(rectangle2.bottom() == 40.);

    assert(rectangle3.x == 10L);
    assert(rectangle3.y == -10L);
    assert(rectangle3.width == 50L);
    assert(rectangle3.height == 30L);
    assert(rectangle3.area() == 1500L);
    assert(rectangle3.right() == 60L);
    assert(rectangle3.bottom() == 20L);
    assert(rectangle3.contains(60, 20));
    assert(rectangle3.contains(cvx::point2<long>(20, 12)));
    assert(!rectangle3.contains(0, 61));
    assert_approx_equals(rectangle3.aspect_ratio(), 1.66666666667f, epsilon);

    rectangle4.width = 17.f;
    rectangle4.height = 5.f;

    assert(rectangle4.x == 20.f);
    assert(rectangle4.y == 40.f);
    assert(rectangle4.width == 17.f);
    assert(rectangle4.height == 5.f);
    assert(rectangle4.area() == 85.f);
    assert(rectangle4.right() == 37.f);
    assert(rectangle4.bottom() == 45.f);
    assert(rectangle4.contains(cvx::rectangle2f(25.f, 43.f, 1.991f, 1.25f)));
    assert_approx_equals(rectangle4.aspect_ratio(), 3.4f, epsilon);

    rectangle1.x = 0;
    rectangle1.y = 0;
    rectangle1.width = 5;
    rectangle1.height = 5;
    rectangle1.move(5, 5);
    assert(rectangle1.x == 5);
    assert(rectangle1.y == 5);
    assert(rectangle1.width == 5);
    assert(rectangle1.height == 5);
    assert(rectangle1.right() == 10);
    assert(rectangle1.bottom() == 10);

    cvx::rectangle2i rectangle5(5, 5, 5, 5);
    assert(rectangle1 == rectangle5);

    return 0;
}
