#include <cvx.hpp>
#include <algorithm>
#include <assert.h>
#include <iterator>
#include <type_traits>

int main() {
    const int width = 10;
    const int height = 20;

    // Notice the use of a char. If there were more unique components than 1 << sizeof(char) - 1,
    // which is usually 255, the algorithm would mislabel some components because the label count
    // would wrap around 255 to zero
    char array[][10] = { {1, 1, 1, 1, 0, 1, 1, 1, 1, 0},
                         {1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
                         {1, 0, 0, 1, 0, 1, 1, 1, 1, 1},
                         {1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
                         {1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                         {1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
                         {0, 0, 1, 0, 1, 1, 1, 1, 1, 1},
                         {0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                         {1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
                         {1, 0, 1, 1, 1, 0, 0, 0, 1, 1},
                         {0, 0, 1, 1, 0, 0, 0, 0, 1, 1},
                         {1, 0, 1, 1, 0, 0, 0, 0, 1, 1},
                         {0, 0, 1, 1, 1, 0, 1, 0, 1, 1},
                         {1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                         {0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                         {0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                         {0, 0, 1, 1, 1, 1, 1, 0, 0, 0} };

    auto first = std::begin(array[0]);
    auto last  = std::end(array[height - 1]);

    std::vector<cvx::connected_component> components;

    std::vector<std::vector<cvx::point2i>> expected_points(9);
    expected_points[0].emplace_back(4, 0);
    expected_points[0].emplace_back(3, 1);
    expected_points[0].emplace_back(4, 1);
    expected_points[0].emplace_back(5, 1);
    expected_points[0].emplace_back(4, 2);

    expected_points[1].emplace_back(9, 0);

    expected_points[2].emplace_back(1, 2);
    expected_points[2].emplace_back(2, 2);
    expected_points[2].emplace_back(1, 3);

    expected_points[3].emplace_back(6, 5);
    expected_points[3].emplace_back(8, 5);
    expected_points[3].emplace_back(6, 6);
    expected_points[3].emplace_back(7, 6);
    expected_points[3].emplace_back(8, 6);
    expected_points[3].emplace_back(7, 7);

    expected_points[4].emplace_back(0, 8);
    expected_points[4].emplace_back(1, 8);
    expected_points[4].emplace_back(0, 9);
    expected_points[4].emplace_back(1, 9);
    expected_points[4].emplace_back(1, 10);
    expected_points[4].emplace_back(1, 11);
    expected_points[4].emplace_back(0, 12);
    expected_points[4].emplace_back(1, 12);
    expected_points[4].emplace_back(1, 13);
    expected_points[4].emplace_back(0, 14);
    expected_points[4].emplace_back(1, 14);

    expected_points[5].emplace_back(3, 8);

    expected_points[6].emplace_back(5, 10);
    expected_points[6].emplace_back(7, 10);
    expected_points[6].emplace_back(5, 11);
    expected_points[6].emplace_back(6, 11);
    expected_points[6].emplace_back(7, 11);
    expected_points[6].emplace_back(4, 12);
    expected_points[6].emplace_back(5, 12);
    expected_points[6].emplace_back(6, 12);
    expected_points[6].emplace_back(7, 12);
    expected_points[6].emplace_back(4, 13);
    expected_points[6].emplace_back(5, 13);
    expected_points[6].emplace_back(6, 13);
    expected_points[6].emplace_back(7, 13);
    expected_points[6].emplace_back(5, 14);
    expected_points[6].emplace_back(7, 14);
    expected_points[6].emplace_back(5, 15);

    expected_points[7].emplace_back(0, 17);
    expected_points[7].emplace_back(0, 18);
    expected_points[7].emplace_back(1, 18);
    expected_points[7].emplace_back(0, 19);
    expected_points[7].emplace_back(1, 19);

    expected_points[8].emplace_back(7, 19);
    expected_points[8].emplace_back(8, 19);
    expected_points[8].emplace_back(9, 19);

    try {
        // Label the array with 4-connectivity and background elements as zero
        auto ccs = cvx::label_connected_components(first,
                                                   last,
                                                   std::back_inserter(components),
                                                   width,
                                                   height,
                                                   4,
                                                   0,
                                                   1,
                                                   cvx::feature_flag::points);

        assert(ccs == 9);
        assert(components.size() == 9);

        for (unsigned int i = 0; i < ccs; ++i) {
            assert(components[i].label() == i + 1);

            auto& points = components[i].points();
            assert(!points.empty());
            assert(std::equal(points.cbegin(),
                              points.cend(),
                              expected_points[i].cbegin()));
        }
    } catch (cvx::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
