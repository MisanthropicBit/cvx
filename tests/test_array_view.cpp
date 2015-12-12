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
    
    int int_array[15][6] = { {-985, 277, -188, -436, -740, 94},
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
        cvx::array_view<const short*> short_view(std::begin(short_array[0]),
                                                 std::end(short_array[7]),
                                                 12,
                                                 8);

        assert(!short_view.is_subview());
        assert(short_view.valid());
        assert(short_view.contains_point(cvx::point2i(5, 2)));
        assert(short_view[78] == 245);
        assert(short_view.at(22) == 867);
        assert(short_view.at(6, 7) == -961);
        assert(short_view(3, 10) == 915);
        assert(short_view(cvx::point2<char>(8, 0)) == -259);
        assert(short_view.row(4) == short_array[4]);
        assert(short_view.width() == 12);
        assert(short_view.height() == 8);
        assert(short_view.pitch() == 12 * sizeof(short));
        assert(short_view.size() == 12 * 8);
        assert(short_view.bytesize() == 12 * 8 * sizeof(short));
        assert(short_view.bounds() == cvx::rectangle2i(0, 0, 12, 8));
        assert(short_view.begin() == &short_array[0][0]);
        assert(short_view.begin() == std::begin(short_array[0]));
        assert(short_view.end() == &short_array[7][12]);
        assert(short_view.end() == std::end(short_array[7]));

        for (size_t y = 0; y < short_view.height(); ++y) {
            for (size_t x = 0; x < short_view.width(); ++x) {
                assert(short_view.at(y, x) == short_array[y][x]);
            }
        }

        cvx::array_view<int*> int_view(&int_array[0][0],
                                       &int_array[14][6],
                                       6,
                                       15);

        assert(!int_view.is_subview());
        assert(int_view.valid());
        assert(int_view.contains_point(cvx::point2i(4, 9)));
        assert(int_view[45] == 507);
        assert(int_view.at(5) == 94);
        assert(int_view.at(14, 5) == -903);
        assert(int_view(6, 2) == 872);
        assert(int_view(cvx::point2<long>(0, 10)) == 125);
        assert(int_view.row(10) == int_array[10]);
        assert(int_view.width() == 6);
        assert(int_view.height() == 15);
        assert(int_view.pitch() == 6 * sizeof(int));
        assert(int_view.size() == 6 * 15);
        assert(int_view.bytesize() == 6 * 15 * sizeof(int));
        assert(int_view.bounds() == cvx::rectangle2i(0, 0, 6, 15));
        assert(int_view.begin() == &int_array[0][0]);
        assert(int_view.begin() == std::begin(int_array[0]));
        assert(int_view.end() == &int_array[14][6]);
        assert(int_view.end() == std::end(int_array[14]));

        for (size_t y = 0; y < int_view.height(); ++y) {
            for (size_t x = 0; x < int_view.width(); ++x) {
                assert(int_view.at(y, x) == int_array[y][x]);
            }
        }

        // Test subviews. With the given bounds, this is the resulting
        // view of int_view:
        //   {-310, 106},
        //   {-13, -795},
        //   {280, -36},

        cvx::rectangle2i bounds(2, 2, 2, 3);
        cvx::array_view<int*> int_subview(int_view,
                                                bounds);
        auto same_int_subview = int_view.subview(bounds);
        typedef decltype(int_subview)::value_type subview_type;

        assert(int_subview.is_subview());
        assert(int_subview.valid());
        assert(same_int_subview.is_subview());
        assert(same_int_subview.valid());
        assert(int_subview == same_int_subview);

        assert(int_subview[4] == 280);
        assert(int_subview.at(2) == -13);
        assert(int_subview.at(0, 1) == 106);
        assert(int_subview(1, 1) == -795);
        assert(int_subview(cvx::point2<long>(0, 0)) == -310);
        assert(int_subview.row(0) == &int_array[2][2]);
        assert(int_subview.width() == 2);
        assert(int_subview.height() == 3);
        assert(int_subview.pitch() == 2 * sizeof(subview_type));
        assert(int_subview.size() == 2 * 3);
        assert(int_subview.bytesize() == 2 * 3 * sizeof(subview_type));
        assert(int_subview.bounds() == cvx::rectangle2i(2, 2, 2, 3));
        assert(int_subview.begin() == &int_array[2][2]);
        assert(int_subview.end() == &int_array[4][4]);

        // Test that modifications to subviews are reflected in the parent view
        assert(int_subview[1] == 106);
        assert(same_int_subview[1] == 106);
        assert(int_view[15] == 106);

        int_subview[1] = 90;
        assert(int_subview[1] == 90);
        assert(same_int_subview[1] == 90);
        assert(int_view[15] == 90);

        #ifdef CVX_WITH_OPENCV
            // Test views of OpenCV matrices
            cv::Mat int_mat(30, 30, CV_32S);
            cvx::array_view<cv::MatConstIterator_<int>> opencv_view(int_mat);

            assert(!opencv_view.is_subview());
            assert(opencv_view.valid());
            assert(opencv_view.contains_point(cvx::point2i(15, 21)));
            //assert(opencv_view[35] == );
            //assert(opencv_view.at(888) == );
            //assert(opencv_view.at(12, 9) == );
            //assert(opencv_view(28, 0) == );
            //assert(opencv_view(cvx::point2<long>(0, 10)) == );
            //assert(opencv_view.row(10) == int_array[10]);
            assert(int_mat.cols == opencv_view.width());
            assert(int_mat.rows == opencv_view.height());
            assert(opencv_view.size() == 30 * 30);
            assert(opencv_view.bytesize() == 30 * 30 * sizeof(int));
            assert(opencv_view.pitch() == 30 * sizeof(int));

            //for (int y = 0; y < opencv_view.height(); ++y) {
            //    for (int x = 0; x < opencv_view.width(); ++x) {
            //        assert(opencv_view.at(y, x) == int_mat.at<int>(y, x));
            //    }
            //}
        #endif // CVX_WITH_OPENCV
    } catch (const cvx::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    } catch (const std::out_of_range& ex) {
        std::cerr << "Out of range error: " << ex.what() << std::endl;
        return 1;
    } catch (const std::logic_error& ex) {
        std::cerr << "Logic error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
