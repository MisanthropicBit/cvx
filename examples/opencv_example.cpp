#include <iostream>

#ifdef CVX_WITH_OPENCV
    #include <cvx/ccl.hpp>
    #include <cvx/draw.hpp>
    #include <cvx/exception.hpp>
    #include <opencv2/core/core.hpp>
    #include <opencv2/imgproc/imgproc.hpp>
    #include <opencv2/highgui/highgui.hpp>
    #include <chrono>
    #include <cmath>
    #include <ctime>
    #include <random>

    using hi_clock = std::chrono::high_resolution_clock;
    using ms       = std::chrono::milliseconds;

    int main() {
        try {
            // Fill a matrix with random values
            cv::Mat mat(500, 500, CV_32S);
            auto first = mat.begin<int>();
            auto last  = mat.end<int>();

            std::random_device rnd;
            std::uniform_real_distribution<float> urd(0.f, 1.f);

            for (auto it = first; it != last; ++it) {
                *it = urd(rnd) < 0.4f ? 0 : 255;
            }

            // Show the image prior to labelling
            cv::Mat binary(500, 500, CV_8UC1);
            mat.convertTo(binary, CV_8UC1);
            cv::imshow("Binary image", binary);
            std::cout << "Press any key to continue..." << std::endl;
            cv::waitKey(0);

            auto start = hi_clock::now();

            // Label all connected components
            auto ccs = cvx::label_connected_components(first,
                                                       last,
                                                       mat.cols,
                                                       mat.rows,
                                                       mat.step,
                                                       8,
                                                       0,
                                                       255);

            auto end = hi_clock::now();
            auto duration = std::chrono::duration_cast<ms>(end - start).count();

            // Print number of connected components and show image
            std::cout << "Found " << ccs << " connected component(s) in " << duration << " ms" << std::endl;

            // Display the labelled image
            cvx::display_connected_components(first,
                                              last,
                                              mat.cols,
                                              mat.rows,
                                              mat.step,
                                              "Labelled image");
            cvx::wait_for_key();
        } catch (cv::Exception& ex) {
            std::cerr << ex.what() << std::endl;
        } catch (cvx::exception& ex) {
            std::cerr << ex.what() << std::endl;
        }

        return 0;
    }
#else
    int main() {
        std::cout << "You need to compile with OpenCV support for this example" << std::endl;
        return 0;
    }
#endif // CVX_OPENCV
