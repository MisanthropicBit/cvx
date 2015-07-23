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

        cv::imshow("Grayscale image", image);
        cv::waitKey(0);

        cv::Mat binary(image.rows, image.cols, CV_8UC1);
        cv::threshold(image, binary, threshold, 255, cv::THRESH_BINARY);
        cv::imshow("Binary image", binary);
        cv::waitKey(0);

        // Convert the binary image to a 32-bit integer image so it can contain
        // a potentially large number of labels
        binary.convertTo(binary, CV_32S);

        std::vector<cvx::connected_component> components;

        auto extraction_flags = cvx::feature_flag::area |
                                cvx::feature_flag::centroid |
                                cvx::feature_flag::bounding_box |
                                cvx::feature_flag::points;

        try {
            auto start = hires_clock::now();
            auto first = binary.begin<int>();
            auto last  = binary.end<int>();

            auto ccs = cvx::label_connected_components(first,
                                                       last,
                                                       std::back_inserter(components),
                                                       binary.cols, 
                                                       binary.rows,
                                                       binary.step,
                                                       8,
                                                       0,
                                                       255,
                                                       extraction_flags);

            auto end = hires_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "Found " << ccs << " connected components in " << duration << " ms" << std::endl;

            // Display some statistics
            std::cout << "Area   Centroid   Extent   Bounding box   Solid?\n";

            for (auto& cc : components) {
                std::cout << cc.area() << "  " << cc.centroid() << "  " << cc.extent() << "  " << cc.bounding_box() << "  " << cc.solid() << std::endl;
            }

            // Display all visual features of the connected components
            cvx::display_connected_components(first,
                                              last,
                                              binary.cols,
                                              binary.rows,
                                              binary.step,
                                              components.cbegin(),
                                              components.cend(),
                                              cvx::feature_flag::all,
                                              "Components and extracted features");
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
