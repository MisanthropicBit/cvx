#ifndef CVX_CCL_DETAIL_HPP
#define CVX_CCL_DETAIL_HPP

#include "cvx/detail/contour.hpp"
#include "cvx/detail/twopass_label.hpp"

namespace cvx {
    namespace detail {
        //////////////////////////////////////////////////////////////////////
        /// Label the connected components in an image given by a view
        ///
        /// \param view               The view of some image data
        /// \param out                Output iterator for storing connected
        ///                           components, e.g. a std::vector<>
        /// \param connectivity       Neighbourhood connectivity (4 or 8)
        /// \param foreground         Value of foreground elements
        /// \param background         Value of background elements
        /// \param flags              Bitflag of the component features to
        ///                           extract
        /// \return The number of connected components found
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator,
                 typename OutputIterator>
        std::size_t label_connected_components_view(array_view<RandomAccessIterator>& view,
                                                    OutputIterator out,
                                                    unsigned char connectivity,
                                                    iterator_value_type<RandomAccessIterator> foreground,
                                                    iterator_value_type<RandomAccessIterator> background,
                                                    const feature_flag& flags) {
            std::size_t label_count = 0;

            if (any_flags(flags & feature_flag::all_contours) > 0) {
                // If contours need to be extracted, call the labelling algorithm
                // that labels by finding contours
                label_count = contour_label(view,
                                            out,
                                            connectivity,
                                            foreground,
                                            background,
                                            flags);
            } else {
                label_count = two_pass_label(view,
                                             out,
                                             connectivity,
                                             background,
                                             flags);
            }

            return label_count;
        }

        //////////////////////////////////////////////////////////////////////
        /// Validate the given arguments. Throws an exception if validation
        /// fails
        ///
        /// \param connectivity       Neighbourhood connectivity (4 or 8)
        /// \param foreground         Value of foreground elements
        /// \param background         Value of background elements
        //////////////////////////////////////////////////////////////////////
        template<typename T>
        void validate_arguments(unsigned char connectivity,
                                const T& foreground,
                                const T& background) {
            if (connectivity != 4 && connectivity != 8) {
                throw exception("Connectivity must be 4 or 8");
            }

            if (foreground == background) {
                throw exception("Foreground and background values cannot be identical");
            }
        }

        //////////////////////////////////////////////////////////////////////
        /// Implementation of the connected component algorithm based on:
        /// "Optimizing two-pass connected-component labeling algorithms" by
        /// Kesheng Wu, Ekow Otoo and Kenji Suzuki. Does not extract components
        ///
        /// \param first              Iterator to the beginning of the image
        ///                           data source
        /// \param last               Iterator to the end of the image data
        ///                           source
        /// \param first2             Iterator to the beginning of the image
        ///                           data target
        /// \param last2              Iterator to the end of the image data
        ///                           target
        /// \param width              Width of the image data
        /// \param height             Height of the image data
        /// \param connectivity       Neighbourhood connectivity (4 or 8)
        /// \param pred               Unary predicate that identifies a
        ///                           background element
        /// \return The number of connected components found
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        std::size_t label_connected_components(RandomAccessIterator first,
                                               RandomAccessIterator last,
                                               std::size_t width,
                                               std::size_t height,
                                               unsigned char connectivity,
                                               iterator_value_type<RandomAccessIterator> foreground,
                                               iterator_value_type<RandomAccessIterator> background) {
            validate_arguments(connectivity, foreground, background);

            array_view<RandomAccessIterator> view(first,
                                                  last,
                                                  width,
                                                  height);

            return two_pass_label(view, connectivity, background);
        }

        //////////////////////////////////////////////////////////////////////
        /// Implementation of the connected component algorithm based on:
        /// "Optimizing two-pass connected-component labeling algorithms" by
        /// Kesheng Wu, Ekow Otoo and Kenji Suzuki
        ///
        /// \param first              Iterator to the beginning of the image
        ///                           data source
        /// \param last               Iterator to the end of the image data
        ///                           source
        /// \param out                Output iterator for storing connected
        ///                           components, e.g. a std::vector<>
        /// \param width              Width of the image data
        /// \param height             Height of the image data
        /// \param connectivity       Neighbourhood connectivity (4 or 8)
        /// \param foreground         Value of foreground elements
        /// \param background         Value of background elements
        /// \param flags              Bitflag of the component features to
        ///                           extract
        /// \return The number of connected components found
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator,
                 typename OutputIterator>
        std::size_t label_connected_components(RandomAccessIterator first,
                                               RandomAccessIterator last,
                                               OutputIterator out,
                                               std::size_t width,
                                               std::size_t height,
                                               unsigned char connectivity,
                                               iterator_value_type<RandomAccessIterator> foreground,
                                               iterator_value_type<RandomAccessIterator> background,
                                               const feature_flag& flags = feature_flag::none) {
            // TODO: Necessary?
            //if (first == last) {
            //    return 0;
            //}

            if (any_flags(flags)) {
                validate_arguments(connectivity, foreground, background);

                array_view<RandomAccessIterator> view(first,
                                                      last,
                                                      width,
                                                      height);

                return label_connected_components_view(view,
                                                       out,
                                                       connectivity,
                                                       foreground,
                                                       background,
                                                       flags);
            }

            // If no features need to be extracted, call the function
            // that does no extraction instead
            return label_connected_components(first,
                                              last,
                                              width,
                                              height,
                                              connectivity,
                                              foreground,
                                              background);
        }
    } // detail
} // cvx

#endif // CVX_CCL_DETAIL_HPP
