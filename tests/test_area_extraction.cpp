#include <cvx.hpp>
#include <assert.h>
#include <iterator>
#include <type_traits>
#include <vector>

int main() {
    const int width = 12;
    const int height = 9;

    short array[][12] = { {1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                          {1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                          {0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0},
                          {0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0},
                          {0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                          {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
                          {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0} };

    auto first = std::begin(array[0]);
    auto last = std::end(array[height - 1]);

    std::vector<cvx::connected_component> components;
    int expected_areas[] = { 4, 5, 9, 4, 3 };

    try {
        // Label the array with 8-connectivity and background elements as zero
        auto ccs = cvx::label_connected_components(first,
                                                   last,
                                                   std::back_inserter(components),
                                                   width,
                                                   height,
                                                   8,
                                                   1,
                                                   0,
                                                   cvx::feature_flag::area);

        assert(ccs == 5);
        assert(components.size() == 5);

        for (size_t i = 0; i < components.size(); ++i) {
            assert(components[i].label() == i + 1);
            assert(components[i].size() == expected_areas[i]);
        }
    } catch (cvx::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
