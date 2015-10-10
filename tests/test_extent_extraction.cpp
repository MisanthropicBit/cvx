#include <cvx.hpp>
#include <assert.h>
#include <iterator>
#include <type_traits>
#include <vector>

void assert_approx_equals(double a, double b, double epsilon) {
    assert(std::fabs(a - b) <= epsilon);
}

int main() {
    const int width = 9;
    const int height = 20;
    const int pitch = width * sizeof(long);

    long array[][width] = { {0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 1, 1, 1, 0},
                            {0, 0, 0, 0, 1, 0, 1, 1, 0},
                            {0, 1, 0, 0, 0, 1, 1, 1, 0},
                            {1, 1, 1, 0, 0, 1, 0, 1, 0},
                            {0, 1, 0, 0, 1, 1, 1, 1, 0},
                            {0, 0, 0, 0, 0, 0, 1, 1, 0},
                            {0, 0, 0, 0, 1, 1, 1, 1, 0},
                            {0, 0, 0, 0, 0, 1, 0, 1, 0},
                            {0, 0, 0, 0, 1, 1, 1, 1, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 1, 1, 1, 1, 1, 0, 0},
                            {0, 1, 0, 0, 0, 0, 0, 1, 0},
                            {0, 1, 0, 1, 0, 1, 0, 1, 0},
                            {0, 1, 0, 0, 0, 0, 0, 1, 0},
                            {0, 1, 0, 1, 0, 1, 0, 1, 0},
                            {0, 1, 0, 1, 1, 1, 0, 1, 0},
                            {0, 1, 0, 0, 0, 0, 0, 1, 0},
                            {0, 0, 1, 1, 1, 1, 1, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0} };

    auto first = std::begin(array[0]);
    auto last = std::end(array[height - 1]);

    std::vector<cvx::connected_component> components;
    const double epsilon = 10e-8;
    double expected_extents[] = { 1.38461538462,
                                  1.0,
                                  1.8,
                                  1.0,
                                  1.0,
                                  1.0,
                                  1.0,
                                  1.0,
                                  1.2,
                                  1.0 };

    try {
        // Label the array with 4-connectivity and background elements as zero
        auto ccs = cvx::label_connected_components(first,
                                                   last,
                                                   std::back_inserter(components),
                                                   width,
                                                   height,
                                                   pitch,
                                                   4,
                                                   1,
                                                   0,
                                                   cvx::feature_flag::extent);

        assert(ccs == 10);
        assert(components.size() == 10);

        for (size_t i = 0; i < components.size(); ++i) {
            assert(components[i].label() == i + 1);
            assert_approx_equals(components[i].extent<double>(), expected_extents[i], epsilon);
        }
    } catch (cvx::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
