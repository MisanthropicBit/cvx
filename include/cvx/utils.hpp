#ifndef CVX_UTILS_HPP
#define CVX_UTILS_HPP

#include "cvx/connected_component.hpp"
#include "cvx/export.hpp"
#include "cvx/point2.hpp"
#include "cvx/feature_flag.hpp"
#include "cvx/detail/extractor.hpp"
#include "cvx/detail/area_extractor.hpp"
#include "cvx/detail/centroid_extractor.hpp"
#include "cvx/detail/point_extractor.hpp"
#include "cvx/detail/bounding_box_extractor.hpp"
#include <memory>
#include <vector>

#ifdef CVX_HAS_OPENCV
    #include <opencv2/core/core.hpp>
#endif

namespace cvx {
    //////////////////////////////////////////////////////////////////////
    /// Threshold arbitrary image data
    ///
    /// \param RandomAccessIterator Any random access iterator
    /// \param OutputIterator       An output iterator with arithmetic type
    /// \param UnaryPredicate       Predicate that identifies background
    ///                             elements
    /// \param first                Iterator to the start of the image
    ///                             data
    /// \param last                 Iterator to the end of the image data
    /// \param width                Width of the image data
    /// \param height               Height of the image data
    /// \param pitch                Pitch or stride of the image data
    /// \param out                  Destination iterator (may be the same
    ///                             as the input iterator)
    /// \param pred                 Unary predicate that identifies
    ///                             background elements
    //////////////////////////////////////////////////////////////////////
    //template<typename RandomAccessIterator,
    //         typename OutputIterator,
    //         typename UnaryPredicate>
    //CVX_EXPORT void threshold(RandomAccessIterator first,
    //                          RandomAccessIterator last,
    //                          std::size_t width,
    //                          std::size_t height,
    //                          std::size_t pitch,
    //                          OutputIterator out,
    //                          typename std::iterator_traits<RandomAccessIterator>::value_type background,
    //                          UnaryPredicate pred) {
    //   // Output iterator must have the connected component as value type
    //   static_assert(std::is_arithmetic<typename std::iterator_traits<OutputIterator>::value_type>::value,
    //                "Output iterator must have arithmetic type as value type");

    //   array_view<RandomAccessIterator> view(first,
    //                                         last,
    //                                         width,
    //                                         height,
    //                                         pitch);

    //   for (auto it = view.begin(); it != view.cend(); ++it, ++out) {
    //       if (pred(*it)) {
    //           *out = background;
    //       }
    //   }
    //}
    
    namespace detail {
        template<typename Iterator>
        using diff_type = typename std::iterator_traits<Iterator>::difference_type;

        //////////////////////////////////////////////////////////////////////
        /// Check that the distance covered by an iterator range corresponds
        /// to the given image dimensions
        ///
        /// \param RandomAccessIterator Any random access iterator
        /// \param width                Width of the image data
        /// \param height               Height of the image data
        /// \param pitch                Pitch or stride of the image data
        /// \return True if the distances match, false otherwise
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        bool check_pitched_distance(RandomAccessIterator first,
                                    RandomAccessIterator last,
                                    diff_type<RandomAccessIterator> pitch,
                                    diff_type<RandomAccessIterator> height) {
            using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;

            return (std::distance(first, last) == static_cast<diff_type<RandomAccessIterator>>(height * pitch / sizeof(value_type)));
        }

        template<typename OutputIterator>
        void make_extractors_from_flags(const feature_flag& flags,
                                        OutputIterator out) {
            if (!flags) {
                return;
            }

            // Static local vector of reuseable extractors to avoid consecutive reallocations
            static std::vector<std::shared_ptr<extractor>> extractors;

            if (extractors.empty()) {
                extractors.emplace_back(std::make_shared<area_extractor>());
                extractors.emplace_back(std::make_shared<centroid_extractor>());
                extractors.emplace_back(std::make_shared<point_extractor>());
                extractors.emplace_back(std::make_shared<bounding_box_extractor>());
            }

            // This is a very unelegant method of selecting the extractors
            // from the feature_flag, but we cannot iterate enums or enum
            // classes in C++
           if (any_flags(flags & feature_flag::area)) {
                *out++ = extractors[0];
            }

            if (any_flags(flags & feature_flag::centroid)) {
                *out++ = extractors[1];
            }

            if (any_flags(flags & feature_flag::points)) {
                *out++ = extractors[2];
            }

            if (any_flags(flags & feature_flag::bounding_box)) {
                *out++ = extractors[3];
            }
        }
    } // detail
} // cvx

#endif // CVX_UTILS_HPP
