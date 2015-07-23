#ifndef CVX_DRAW_HPP
#define CVX_DRAW_HPP

#include "cvx/color.hpp"
#include "cvx/connected_component.hpp"
#include "cvx/exception.hpp"
#include "cvx/feature_flag.hpp"
#include <algorithm>
#include <map>

#ifdef CVX_WITH_OPENCV
    #include <opencv2/core/core.hpp>
    #include <opencv2/highgui/highgui.hpp>
#endif

namespace cvx {
    //////////////////////////////////////////////////////////////////////
    /// \return Generates and returns a random color
    //////////////////////////////////////////////////////////////////////
    color random_color();

    #ifdef CVX_WITH_OPENCV
        namespace detail {
            cv::Scalar color_to_scalar(const color& color);
        }

        //////////////////////////////////////////////////////////////////////
        /// Waits for the user to press any key, then returns
        //////////////////////////////////////////////////////////////////////
        CVX_EXPORT void wait_for_key();

        //////////////////////////////////////////////////////////////////////
        /// Display the connected components in a labelled image
        ///
        /// \param RandomAccessIterator Iterator type providing random access
        /// \param first  Iterator to the beginning of the image
        ///               data
        /// \param last   Iterator to the end of the image data
        /// \param width  Width of the image data
        /// \param height Height of the image data
        /// \param pitch  Pitch in bytes of the image data
        /// \param title  Title of the image when displayed
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        CVX_EXPORT void display_connected_components(RandomAccessIterator first,
                                                     RandomAccessIterator last,
                                                     std::size_t width,
                                                     std::size_t height,
                                                     std::size_t pitch,
                                                     const std::string& title = "");

        //////////////////////////////////////////////////////////////////////
        /// Display the connected components and select features in a labelled
        /// image
        ///
        /// \param RandomAccessIterator Iterator type providing random access
        /// \param InputIterator        Input iterator type
        /// \param first                Iterator to the beginning of the image
        ///                             data
        /// \param last                 Iterator to the end of the image data
        /// \param width                Width of the image data
        /// \param height               Height of the image data
        /// \param pitch                Pitch in bytes of the image data
        /// \param first2               InputIterator to the beginning of
        ///                             connected components
        /// \param last2                InputIterator to the end of connected
        ///                             components
        /// \param flags                Features to draw
        /// \param title                Title of the image when displayed
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator, typename InputIterator>
        CVX_EXPORT void display_connected_components(RandomAccessIterator first,
                                                     RandomAccessIterator last,
                                                     std::size_t width,
                                                     std::size_t height,
                                                     std::size_t pitch,
                                                     InputIterator first2,
                                                     InputIterator last2,
                                                     const feature_flag& flags = feature_flag::none,
                                                     const std::string& title = "");
    #else
        // Dummy functions if OpenCV is not available
        CVX_EXPORT void wait_for_key();

        template<typename RandomAccessIterator>
        CVX_EXPORT void display_connected_components(RandomAccessIterator first,
                                                     RandomAccessIterator last,
                                                     std::size_t width,
                                                     std::size_t height,
                                                     std::size_t pitch,
                                                     const std::string& title = "");

        template<typename RandomAccessIterator, typename InputIterator>
        CVX_EXPORT void display_connected_components(RandomAccessIterator first,
                                                     RandomAccessIterator last,
                                                     std::size_t width,
                                                     std::size_t height,
                                                     std::size_t pitch,
                                                     InputIterator first2,
                                                     InputIterator last2,
                                                     const feature_flag& flags = feature_flag::none,
                                                     const std::string& title = "");
    #endif // CVX_WITH_OPENCV
} // cvx

#include "cvx/draw.inl"

#endif // CVX_DRAW_HPP
