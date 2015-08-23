#include <cvx.hpp>
#include <assert.h>
#include <iterator>
#include <type_traits>
#include <vector>

int main() {
    const int width = 12;
    const int height = 9;
    const int pitch = width * sizeof(short);

    //short array[][12] = { {1, 1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},
    //                      {1, 1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},
    //                      {0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},
    //                      {0, 0, 2, 1, 0, 0, 0, 0, 3, 0, 0, 0},
    //                      {0, 0, 2, 2, 0, 2, 0, 3, 0, 0, 0, 0},
    //                      {0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0},
    //                      {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0},
    //                      {0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0},
    //                      {0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0} };

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
                                                   cvx::feature_flag::area);

        assert(ccs == 5);
        assert(components.size() == 5);
        assert(components[0].size() == expected_areas[0]);
        assert(components[1].size() == expected_areas[1]);
        assert(components[2].size() == expected_areas[2]);
        assert(components[3].size() == expected_areas[3]);
        assert(components[4].size() == expected_areas[4]);
    } catch (cvx::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
