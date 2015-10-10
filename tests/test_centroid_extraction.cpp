#include <cvx.hpp>
#include <assert.h>
#include <iterator>
#include <type_traits>
#include <vector>

void assert_approx_equals(float a, float b, float epsilon) {
    assert(std::fabs(a - b) <= epsilon);
}

int main() {
    const int width = 18;
    const int height = 9;
    const int pitch = width * sizeof(short);

    short array[][18] = { {1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0},
                          {0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0},
                          {0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0},
                          {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0},
                          {0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0},
                          {0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0},
                          {0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0} };

    auto first = std::begin(array[0]);
    auto last = std::end(array[height - 1]);

    std::vector<cvx::connected_component> components;
    const float epsilon = 0.5f;
    cvx::point2f expected_centroids[8] = { cvx::point2f(0.f, 0.f),
                                           cvx::point2f(4.277778f, 5.111111f),
                                           cvx::point2f(8.8f, 1.5f),
                                           cvx::point2f(2.f, 2.888f),
                                           cvx::point2f(14.5f, 2.5f),
                                           cvx::point2f(11.f, 4.f),
                                           cvx::point2f(8.571f, 6.143f),
                                           cvx::point2f(14.f, 7.f) };

    try {
        // Label the array with 4-connectivity and background elements as zero
        auto ccs = cvx::label_connected_components(first,
                                                   last,
                                                   std::back_inserter(components),
                                                   width,
                                                   height,
                                                   pitch,
                                                   8,
                                                   1,
                                                   0,
                                                   cvx::feature_flag::centroid);

        assert(ccs == 8);
        assert(components.size() == 8);

        for (size_t i = 0; i < components.size(); ++i) {
            assert(components[i].label() == i + 1);
            assert_approx_equals(components[i].centroid().x, expected_centroids[i].x, epsilon);
            assert_approx_equals(components[i].centroid().y, expected_centroids[i].y, epsilon);
        }
    } catch (cvx::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
