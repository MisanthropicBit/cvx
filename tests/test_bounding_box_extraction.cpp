#include <cvx.hpp>
#include <assert.h>
#include <iterator>
#include <type_traits>

int main() {
    const int width = 19;
    const int height = 11;

    // Notice that we operate on a smaller part of the array's full width
    int array[][25] = { {0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 2, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 2, 2, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 2, 2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };

    auto first = std::begin(array[0]);
    auto last = std::end(array[height - 1]);

    std::vector<cvx::connected_component> components;
    cvx::rectangle2i expected_bounding_boxes[] = { cvx::rectangle2i(7, 0, 3, 5),
                                                   cvx::rectangle2i(1, 1, 4, 6),
                                                   cvx::rectangle2i(7, 6, 5, 5),
                                                   cvx::rectangle2i(5, 8, 1, 1) };

    try {
        // Label the array with 4-connectivity and background elements as zero
        auto ccs = cvx::label_connected_components(first,
                                                   last,
                                                   std::back_inserter(components),
                                                   width,
                                                   height,
                                                   4,
                                                   2,
                                                   0,
                                                   cvx::feature_flag::bounding_box);

        assert(ccs == 4);
        assert(components.size() == 4);

        for (size_t i = 0; i < components.size(); ++i) {
            assert(components[i].label() == i + 1);
            assert(components[i].bounding_box() == expected_bounding_boxes[i]);
        }
    } catch (cvx::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
