#include <cvx.hpp>
#include <assert.h>
#include <iterator>
#include <iostream>

int main() {
    const short short_array[8][12] = { {667, -494, 598, -985, -144, 711, -49, 733, -259, -564, 304, -163},
                                      {938, 545, -827, 100, -19, 273, 474, -55, 918, -413, 867, -681},
                                      {-904, -674, 669, -192, -459, 674, 58, -3, 132, -515, 269, 784},
                                      {-469, -785, -699, 460, -739, -161, -174, 614, 263, -140, 915, -473},
                                      {312, 647, -76, 225, 767, -101, 798, -638, -827, 577, 804, 36},
                                      {160, 119, 512, -437, -554, 758, 126, -17, -697, -351, -846, 401},
                                      {-189, 638, -659, 710, -609, -838, 245, -961, -504, -373, 409, -692},
                                      {481, 99, 167, -182, 191, 92, -693, 611, 892, -342, 192, 236} };
    
    const int int_array[15][6] = { {-985, 277, -188, -436, -740, 94},
                                 {770, 164, 100, 476, 838, -534},
                                 {197, -867, -310, 106, -450, -405},
                                 {-290, 180, -13, -795, 25, 938},
                                 {-859, 334, 280, -36, -284, 872},
                                 {632, -200, 614, -571, -593, -252},
                                 {630, -906, 872, -954, 629, -233},
                                 {197, 62, 256, 507, 165, -478},
                                 {799, -302, -618, 994, 277, -118},
                                 {-171, -327, 340, 147, 383, 520},
                                 {125, 32, 792, -700, -423, 815},
                                 {-427, 638, -523, 944, 481, -651},
                                 {-405, -967, -76, -680, 748, -513},
                                 {915, 206, -313, 455, 648, -921},
                                 {-797, 454, 756, 61, -772, -903} };

    try {
        cvx::array_view<const short*> short_view(&short_array[0][0],
                                                 &short_array[7][12],
                                                 12,
                                                 8);

        assert(short_view.valid());
        assert(short_view.contains(cvx::point2i(5, 2)));
        assert(short_view[78] == 245);
        assert(short_view.at(22) == 867);
        assert(short_view.at(6, 7) == -693);
        assert(short_view(3, 10) == 915);
        assert(short_view(cvx::point2<char>(8, 0)) == -259);
        assert(short_view.row(4) == short_array[4]);
        assert(short_view.width() == 12);
        assert(short_view.height() == 8);
        assert(short_view.pitch() == 12 * sizeof(short));
        assert(short_view.size() == 12 * 8);
        assert(short_view.bytesize() == 12 * 8 * sizeof(short));
        assert(short_view.begin() == &short_array[0][0]);
        assert(short_view.begin() == std::begin(short_array[0]));
        assert(short_view.end() == &short_array[7][12]);
        assert(short_view.end() == std::end(short_array[7]));

        cvx::array_view<const int*> int_view(&int_array[0][0],
                                             &int_array[14][6],
                                             6,
                                             15);

        assert(int_view.valid());
        assert(int_view.contains(cvx::point2i(4, 9)));
        assert(int_view[45] == 507);
        assert(int_view.at(5) == 94);
        assert(int_view.at(5, 14) == -903);
        assert(int_view(6, 2) == 872);
        assert(int_view(cvx::point2<long>(0, 10)) == 125);
        assert(int_view.row(10) == int_array[10]);
        assert(int_view.width() == 6);
        assert(int_view.height() == 15);
        assert(int_view.pitch() == 6 * sizeof(int));
        assert(int_view.size() == 6 * 15);
        assert(int_view.bytesize() == 6 * 15 * sizeof(int));
        assert(int_view.begin() == &int_array[0][0]);
        assert(int_view.begin() == std::begin(int_array[0]));
        assert(int_view.end() == &int_array[14][6]);
        assert(int_view.end() == std::end(int_array[14]));
    } catch (cvx::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
