#ifndef CVX_LABEL_CONNECTED_COMPONENTS_HPP
#define CVX_LABEL_CONNECTED_COMPONENTS_HPP

#include "cvx/export.hpp"
#include "cvx/feature_flag.hpp"
#include "cvx/detail/ccl.hpp" // See for 'iterator_value_type'

namespace cvx {
    //////////////////////////////////////////////////////////////////////
    /// Label the connected components in some binary image data
    /// given by the iterator range [first, last[
    ///
    /// \param RandomAccessIterator Iterator type providing random access
    /// \param first                Iterator to the beginning of the image
    ///                             data
    /// \param last                 Iterator to the end of the image data
    /// \param width                Width of the image data
    /// \param height               Height of the image data
    /// \param connectivity         Neighbourhood connectivity (4 or 8)
    /// \param foreground           Value of foreground elements
    /// \param background           Value of background elements
    /// \return The number of connected components found
    //////////////////////////////////////////////////////////////////////
    template<typename RandomAccessIterator>
    CVX_EXPORT std::size_t label_connected_components(RandomAccessIterator first,
                                                      RandomAccessIterator last,
                                                      std::size_t width,
                                                      std::size_t height,
                                                      unsigned char connectivity,
                                                      iterator_value_type<RandomAccessIterator> foreground,
                                                      iterator_value_type<RandomAccessIterator> background) {
        return detail::label_connected_components(first,
                                                  last,
                                                  width,
                                                  height,
                                                  connectivity,
                                                  foreground,
                                                  background);
    }

    //////////////////////////////////////////////////////////////////////
    /// Label the connected components in some binary image data
    /// given by the iterator range [first, last[ and extract features
    ///
    /// \param RandomAccessIterator Iterator type providing random access
    /// \param OutputIterator       Output iterator type for components
    /// \param first                Iterator to the beginning of the image
    ///                             data
    /// \param last                 Iterator to the end of the image data
    /// \param out                  Output iterator for storing connected
    ///                             components, e.g. a std::vector
    /// \param width                Width of the image data
    /// \param height               Height of the image data
    /// \param connectivity         Neighbourhood connectivity (4 or 8)
    /// \param foreground           Value of foreground elements
    /// \param background           Value of background elements
    /// \param flags                Bitflag of the component features to
    ///                             extract
    /// \return The number of connected components found
    //////////////////////////////////////////////////////////////////////
    template<typename RandomAccessIterator,
             typename OutputIterator>
    CVX_EXPORT std::size_t label_connected_components(RandomAccessIterator first,
                                                      RandomAccessIterator last,
                                                      OutputIterator out,
                                                      std::size_t width,
                                                      std::size_t height,
                                                      unsigned char connectivity,
                                                      iterator_value_type<RandomAccessIterator> foreground,
                                                      iterator_value_type<RandomAccessIterator> background,
                                                      const feature_flag& flags = feature_flag::none) {
        return detail::label_connected_components(first,
                                                  last,
                                                  out,
                                                  width,
                                                  height,
                                                  connectivity,
                                                  foreground,
                                                  background,
                                                  flags);
    }
} // cvx

#endif // CVX_LABEL_CONNECTED_COMPONENTS_HPP
