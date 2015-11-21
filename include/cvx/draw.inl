namespace cvx {
   namespace detail {
        // Define a table of 64 distinctly visible colors
        const color unique_color_table64[] = {
            color(0x30, 0xb5, 0x99),
            color(0xff, 0xff, 0x00),
            color(0x1c, 0xe6, 0xff),
            color(0xff, 0x34, 0xff),
            color(0xff, 0x4a, 0x46),
            color(0x00, 0x89, 0x41),
            color(0x00, 0x6f, 0xa6),
            color(0xa3, 0x00, 0x59),
            color(0xff, 0xdb, 0xe5),
            color(0x7a, 0x49, 0x00),
            color(0x00, 0x00, 0xa6),
            color(0x63, 0xff, 0xac),
            color(0xb7, 0x97, 0x62),
            color(0x00, 0x4d, 0x43),
            color(0x8f, 0xb0, 0xff),
            color(0x99, 0x7d, 0x87),
            color(0x5a, 0x00, 0x07),
            color(0x80, 0x96, 0x93),
            color(0xfe, 0xff, 0xe6),
            color(0x1b, 0x44, 0x00),
            color(0x4f, 0xc6, 0x01),
            color(0x3b, 0x5d, 0xff),
            color(0x4a, 0x3b, 0x53),
            color(0xff, 0x2f, 0x80),
            color(0x61, 0x61, 0x5a),
            color(0xba, 0x09, 0x00),
            color(0x6b, 0x79, 0x00),
            color(0x00, 0xc2, 0xa0),
            color(0xff, 0xaa, 0x92),
            color(0xff, 0x90, 0xc9),
            color(0xb9, 0x03, 0xaa),
            color(0xd1, 0x61, 0x00),
            color(0xdd, 0xef, 0xff),
            color(0x00, 0x00, 0x35),
            color(0x7b, 0x4f, 0x4b),
            color(0xa1, 0xc2, 0x99),
            color(0x30, 0x00, 0x18),
            color(0x0a, 0xa6, 0xd8),
            color(0x01, 0x33, 0x49),
            color(0x00, 0x84, 0x6f),
            color(0x37, 0x21, 0x01),
            color(0xff, 0xb5, 0x00),
            color(0xc2, 0xff, 0xed),
            color(0xa0, 0x79, 0xbf),
            color(0xcc, 0x07, 0x44),
            color(0xc0, 0xb9, 0xb2),
            color(0xc2, 0xff, 0x99),
            color(0x00, 0x1e, 0x09),
            color(0x00, 0x48, 0x9c),
            color(0x6f, 0x00, 0x62),
            color(0x0c, 0xbd, 0x66),
            color(0xee, 0xc3, 0xff),
            color(0x45, 0x6d, 0x75),
            color(0xb7, 0x7b, 0x68),
            color(0x7a, 0x87, 0xa1),
            color(0x78, 0x8d, 0x66),
            color(0x88, 0x55, 0x78),
            color(0xfa, 0xd0, 0x9f),
            color(0xff, 0x8a, 0x9a),
            color(0xd1, 0x57, 0xa0),
            color(0xbe, 0xc4, 0x59),
            color(0x45, 0x66, 0x48),
            color(0x00, 0x86, 0xed),
            color(0x88, 0x6f, 0x4c),
            color(0x34, 0x36, 0x2d),
            color(0xb4, 0xa8, 0xbd),
            color(0x00, 0xa6, 0xaa),
            color(0x45, 0x2c, 0x2c),
            color(0x63, 0x63, 0x75),
            color(0xa3, 0xc8, 0xc9),
            color(0xff, 0x91, 0x3f),
            color(0x93, 0x8a, 0x81),
            color(0x57, 0x53, 0x29),
            color(0x00, 0xfe, 0xcf),
            color(0xb0, 0x5b, 0x6f),
            color(0x8c, 0xd0, 0xff),
            color(0x3b, 0x97, 0x00),
            color(0x04, 0xf7, 0x57),
            color(0xc8, 0xa1, 0xa1),
            color(0x1e, 0x6e, 0x00),
            color(0x79, 0x00, 0xd7),
            color(0xa7, 0x75, 0x00),
            color(0x63, 0x67, 0xa9),
            color(0xa0, 0x58, 0x37),
            color(0x6b, 0x00, 0x2c),
            color(0x77, 0x26, 0x00),
            color(0xd7, 0x90, 0xff),
            color(0x9b, 0x97, 0x00),
            color(0x54, 0x9e, 0x79),
            color(0xff, 0xf6, 0x9f),
            color(0x20, 0x16, 0x25),
            color(0x72, 0x41, 0x8f),
            color(0xbc, 0x23, 0xff),
            color(0x99, 0xad, 0xc0),
            color(0x3a, 0x24, 0x65),
            color(0x92, 0x23, 0x29),
            color(0x5b, 0x45, 0x34),
            color(0xfd, 0xe8, 0xdc),
            color(0x40, 0x4e, 0x55),
            color(0x00, 0x89, 0xa3),
            color(0xcb, 0x7e, 0x98),
            color(0xa4, 0xe8, 0x04),
            color(0x32, 0x4e, 0x72),
            color(0x6a, 0x3a, 0x4c),
            color(0x83, 0xab, 0x58),
            color(0x00, 0x1c, 0x1e),
            color(0xd1, 0xf7, 0xce),
            color(0x00, 0x4b, 0x28),
            color(0xc8, 0xd0, 0xf6),
            color(0xa3, 0xa4, 0x89),
            color(0x80, 0x6c, 0x66),
            color(0x22, 0x28, 0x00),
            color(0xbf, 0x56, 0x50),
            color(0xe8, 0x30, 0x00),
            color(0x66, 0x79, 0x6d),
            color(0xda, 0x00, 0x7c),
            color(0xff, 0x1a, 0x59),
            color(0x8a, 0xdb, 0xb4),
            color(0x1e, 0x02, 0x00),
            color(0x5b, 0x4e, 0x51),
            color(0xc8, 0x95, 0xc5),
            color(0x32, 0x00, 0x33),
            color(0xff, 0x68, 0x32),
            color(0x66, 0xe1, 0xd3),
            color(0xcf, 0xcd, 0xac),
            color(0xd0, 0xac, 0x94),
            color(0x7e, 0xd3, 0x79),
            color(0x01, 0x2c, 0x58)
        };
   } // detail
} // cvx

namespace cvx {
    #ifdef CVX_WITH_OPENCV
        template<typename RandomAccessIterator>
        void display_connected_components(RandomAccessIterator first,
                                          RandomAccessIterator last,
                                          std::size_t width,
                                          std::size_t height,
                                          const std::string& title) {
            array_view<RandomAccessIterator> view(first,
                                                  last,
                                                  width,
                                                  height);

            cv::Mat label_image(view.height(),
                                view.width(),
                                CV_8UC4);

            // Fill all image pixels with black to denote the background
            label_image = cv::Scalar(0, 0, 0, 0);

            // A mapping for labels > 64 to random colors
            std::map<typename decltype(view)::value_type, color> color_map;

            for (std::size_t y = 0; y < view.height(); ++y) {
                for (std::size_t x = 0; x < view.width(); ++x) {
                    auto label = view(y, x);
                    color label_color;

                    if (label) {
                        if (label < 65) {
                            label_color = detail::unique_color_table64[label - 1];
                        } else {
                            if (color_map.count(label)) {
                                label_color = color_map[label];
                            } else {
                                label_color = random_color();
                                color_map[label] = label_color;
                            }
                        }
                    }

                    label_image.at<cv::Vec4b>(y, x) = detail::color_to_scalar(label_color);
                }
            }

            cv::imshow(title, label_image);
        }

        template<typename RandomAccessIterator, typename InputIterator>
        void display_connected_components(RandomAccessIterator first,
                                          RandomAccessIterator last,
                                          std::size_t width,
                                          std::size_t height,
                                          InputIterator first2,
                                          InputIterator last2,
                                          const feature_flag& flags,
                                          const std::string& title) {
            static_assert(std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                                       connected_component>::value,
                          "Input iterators must have cvx::connected_component as value type");

            if (first2 == last2) {
                throw exception("Cannot draw components, no components to draw");
            }

            if (flags == feature_flag::none) {
                throw exception("Cannot draw components, no features requested");
            }

            auto empty_cc = [](const connected_component& cc) {
                return cc.empty();
            };

            if (std::all_of(first2, last2, empty_cc)) {
                throw exception("Cannot draw components, all connected components are empty");
            }
            
            unsigned int color_count = 0;
            array_view<RandomAccessIterator> view(first,
                                                  last,
                                                  width,
                                                  height);

            cv::Mat label_image(view.height(),
                                view.width(),
                                CV_8UC4);

            // Fill all image pixels with black to denote the background
            label_image = cv::Scalar(0, 0, 0, 0);

            while (first2 != last2) {
                auto cc = *first2++;
               
                // TODO: Check if fill color was already set
                cc.set_fill_color(detail::unique_color_table64[color_count++]);
                cv::Scalar cv_color = detail::color_to_scalar(cc.fill_color());

                if (any_flags(flags & feature_flag::points)) {
                    for (auto& p : cc.points()) {
                        label_image.at<cv::Vec4b>(p.y, p.x) = cv_color;
                    }
                }

                if (any_flags(flags & feature_flag::bounding_box)) {
                    const auto bb = cc.bounding_box();
                    cv::Point top_left(bb.x, bb.y);
                    cv::Point bottom_right(bb.right(), bb.bottom());

                    cv::rectangle(label_image,
                                  top_left,
                                  bottom_right,
                                  cv_color);
                }

                if (any_flags(flags & feature_flag::centroid)) {
                    const auto centroid = cc.centroid();

                    cv::circle(label_image,
                               cv::Point(centroid.x, centroid.y),
                               1,
                               cv::Scalar(0, 0, 255),
                               -1);
                }

                // TODO: Add remaining flags when possible
            }

            cv::imshow(title, label_image);
        }
    #else
        template<typename RandomAccessIterator>
        void display_connected_components(RandomAccessIterator first,
                                          RandomAccessIterator last,
                                          std::size_t width,
                                          std::size_t height,
                                          const std::string& title) {
            throw exception("Cannot draw, OpenCV is not available");
        }
        
        template<typename RandomAccessIterator, typename InputIterator>
        void display_connected_components(RandomAccessIterator first,
                                          RandomAccessIterator last,
                                          std::size_t width,
                                          std::size_t height,
                                          InputIterator first2,
                                          InputIterator last2,
                                          const feature_flag& flags,
                                          const std::string& title) {
            throw exception("Cannot draw, OpenCV is not available");
        }
    #endif // CVX_WITH_OPENCV
}
