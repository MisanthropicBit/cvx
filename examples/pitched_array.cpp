#include <cvx.hpp>
#include <assert.h>
#include <iostream>
#include <iterator>
#include <type_traits>

#ifdef CVX_WITH_OPENCV
    #include <cvx/draw.hpp>
#endif

template<typename T>
void print_2darray(T array[][26], std::size_t width, std::size_t height) {
    for (std::size_t y = 0; y < height; ++y) {
        for (std::size_t x = 0; x < width; ++x) {
            T l = array[y][x];

            if (l == 0) {
                std::cout << "--";
            } else if (l < 10) {
                std::cout << "0" << l;
            } else {
                std::cout << l;
            }

            std::cout << "  ";
        }

        std::cout << std::endl;
    }
}

int main() {
    // A sample 9x25 array with a pitch of 26 * sizeof(int) (presumably 104 bytes for a 4 byte integer)
    int pitched_array[][26] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9},
                                {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 9} ,
                                {0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 9},
                                {0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 9},
                                {0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 9},
                                {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 9},
                                {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 9},
                                {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 9},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9} };

    using T = decltype(pitched_array);
    std::size_t width  = std::extent<T, 1>::value - 1;
    std::size_t height = std::extent<T, 0>::value;
    std::size_t pitch  = std::extent<T, 1>::value * sizeof(int);
    auto first = &pitched_array[0][0];
    auto last  = &pitched_array[height - 1][width + 1];

    #ifndef CVX_WITH_OPENCV
        std::cout << "Binary image:" << std::endl;
        print_2darray(pitched_array, width, height);
    #endif

    try {
        // Label the array with 4-connectivity and background elements as zero
        auto ccs = cvx::label_connected_components(first,
                                                   last,
                                                   width,
                                                   height,
                                                   pitch,
                                                   4,
                                                   1,
                                                   0);

        std::cout << "Found " << ccs << " connected components" << std::endl;
        assert(ccs == 3);
    } catch (cvx::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    #ifdef CVX_WITH_OPENCV
        cvx::display_connected_components(first,
                                          last,
                                          width,
                                          height,
                                          pitch,
                                          "CVX!");
        cvx::wait_for_key();
    #else
        std::cout << "\nLabelled image:" << std::endl;
        print_2darray(pitched_array, width, height);
    #endif

    return 0;
}
