#include <iostream>

#ifdef CVX_WITH_OPENCV
    #include <cvx/ccl.hpp>
    #include <cvx/draw.hpp>
    #include <opencv2/core/core.hpp>
    #include <opencv2/highgui/highgui.hpp>
    #include <opencv2/imgproc/imgproc.hpp>
    #include <chrono>
    #include <sstream>
    #include <iterator>
    #include <stdexcept>
    #include <vector>

    using hires_clock = std::chrono::high_resolution_clock;

    int main(int argc, char** argv) {
        if (argc < 3) {
            std::cerr << "Provide a filename of an image and a threshold value ([0, 255])" << std::endl;
            return 1;
        }

        cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
        int threshold;
        std::istringstream iss(argv[2]);
        iss >> threshold;

        if (threshold < 0 || threshold > 255) {
            std::cerr << "Threshold value must be between 0 and 255, inclusive" << std::endl;
            return 1;
        }

        cv::Mat binary(image.rows, image.cols, CV_8UC1);
        cv::threshold(image, binary, threshold, 255, cv::THRESH_BINARY);

        // Convert the binary image to a 32-bit integer image so it can contain
        // a potentially large number of labels
        binary.convertTo(binary, CV_32S);

        std::vector<cvx::connected_component> components;

        try {
            auto start = hires_clock::now();
            auto first = binary.begin<int>();
            auto last  = binary.end<int>();

            auto ccs = cvx::label_connected_components(first,
                                                       last,
                                                       std::back_inserter(components),
                                                       binary.cols, 
                                                       binary.rows,
                                                       8,
                                                       0,
                                                       255,
                                                       cvx::feature_flag::outer_contours);

            auto end = hires_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "Found " << ccs << " connected components and outer contours in " << duration << " ms" << std::endl;

            std::cout << components.size() << std::endl;

            // Display all visual features of the connected components
            cvx::display_connected_components(first,
                                              last,
                                              binary.cols,
                                              binary.rows,
                                              components.cbegin(),
                                              components.cend(),
                                              cvx::feature_flag::outer_contours,
                                              "Components and outer contours");
            cvx::wait_for_key();
        } catch (cvx::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            return 1;
        }

        return 0;
    }
#else
    int main() {
        std::cout << "You need to compile with OpenCV support for this example" << std::endl;
        return 0;
    }
#endif // CVX_WITH_OPENCV
