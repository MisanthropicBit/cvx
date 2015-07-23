#ifndef CVX_CONTOUR_DETAIL_HPP
#define CVX_CONTOUR_DETAIL_HPP

#include "cvx/array_view.hpp"
#include "cvx/exception.hpp"
#include <iterator>
#include <unordered_set>

#include <iostream>

namespace {
    //////////////////////////////////////////////////////////////////////
    /// A 2d array containing the offsets to get to the next contour
    /// trace point, i.e. for the current element e:
    ///
    ///  5  6  7
    ///   \ | /
    ///  4- e -0
    ///   / | \
    ///  3  2  1
    //////////////////////////////////////////////////////////////////////
    char trace_directions[][2] = { {1, 1},
                                   {1, 0},
                                   {1, -1},
                                   {0, -1},
                                   {-1, -1},
                                   {-1, 0},
                                   {-1, 1},
                                   {0, 1} };
}

namespace cvx {
    namespace detail {
        //////////////////////////////////////////////////////////////////////
        /// \param view       Array view of image data
        /// \param x          X-coordinate of point
        /// \param y          Y-coordinate of point
        /// \param background Value of background elements
        /// \return True if the image data pointed to by view is the start of
        ///         an external contour
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        bool is_external_contour(const array_view<RandomAccessIterator>& view,
                                 std::size_t x,
                                 std::size_t y,
                                 typename std::iterator_traits<RandomAccessIterator>::value_type background) {
            //return (view(y, x) != background) && (y > 0 && view(y - 1, x) == background) || true;

            if (view(y, x) != background) {
                if (y > 0) {
                    return view(y - 1, x) == background;
                }
                
                return true;
            }

            return false;
        }

        //////////////////////////////////////////////////////////////////////
        /// \param view       Array view of image data
        /// \param point      Point in question
        /// \param background Value of background elements
        /// \return True if the image data pointed to by view is the start of
        ///         an external contour
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        bool is_external_contour(const array_view<RandomAccessIterator>& view,
                                 const point2i& point,
                                 typename std::iterator_traits<RandomAccessIterator>::value_type background) {
            return is_external_contour(view,
                                       point.x,
                                       point.y,
                                       background);
        }

        //////////////////////////////////////////////////////////////////////
        /// \param view       Array view of image data
        /// \param x          X-coordinate of point
        /// \param y          Y-coordinate of point
        /// \param background Value of background elements
        /// \return True if the image data pointed to by view is the start of
        ///         an internal contour
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        bool is_internal_contour(const array_view<RandomAccessIterator>& view,
                                 std::size_t x,
                                 std::size_t y,
                                 typename std::iterator_traits<RandomAccessIterator>::value_type background) {
            if (view(y, x) != background) {
                if (y < view.height() - 1) {
                    return view(y + 1, x) == background;
                }

                return true;
            }

            return false;
        }

        //////////////////////////////////////////////////////////////////////
        /// \param view       Array view of image data
        /// \param point      Point in question
        /// \param background Value of background elements
        /// \return True if the image data pointed to by view is the start of
        ///         an internal contour
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        bool is_internal_contour(const array_view<RandomAccessIterator>& view,
                                 const point2i& point,
                                 typename std::iterator_traits<RandomAccessIterator>::value_type background) {
            return is_internal_contour(view,
                                       point.x, 
                                       point.y,
                                       background);
        }

        //////////////////////////////////////////////////////////////////////
        /// Get the next contour point according to the Moore neighbourhood
        ///
        /// \param view  View of image data
        /// \param point Point in question
        /// \param dir   The direction to search in
        /// \return Number of Moore neighbourhood pixels checked
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        int next_contour_point(const array_view<RandomAccessIterator>& view,
                               point2i& point,
                               int& direction,
                               typename std::iterator_traits<RandomAccessIterator>::value_type background) {
            int count = 0;
            point2i temp_point;

            while (count++ < 8) {
                temp_point.x = point.x + trace_directions[direction][0];
                temp_point.y = point.y + trace_directions[direction][1];

                if (view.contains(temp_point)) {
                    if (view(temp_point) == background) {
                        direction = ++direction % 8;
                    } else {
                        break;
                    }
                } else {
                    direction = ++direction % 8;
                }
            }

            point = temp_point;
            return count;
        }

        //////////////////////////////////////////////////////////////////////
        /// Get the starting direction for tracing a contour for a given
        /// position
        ///
        /// \param view       Array view of image data
        /// \param position   Position in question
        /// \param background Value of background elements
        /// \return Starting direction to trace contour in
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator>
        int get_starting_direction(array_view<RandomAccessIterator>& view,
                                   const point2i& position,
                                   typename std::iterator_traits<RandomAccessIterator>::value_type background) {
            if (is_external_contour(view, position, background)) {
                return 7;
            } else if (is_internal_contour(view, position, background)) {
                return 3;
            }

            return 0;
            //next_dir = 0 + 2 % 8; // TODO: True?
        }

        //////////////////////////////////////////////////////////////////////
        /// \param T          Element type
        /// \param value      Value type of the elements
        /// \param foreground Value denoting foreground elements
        /// \param background Value denoting background elements
        /// \return True if the point value is an unlabelled element
        //////////////////////////////////////////////////////////////////////
        template<typename T>
        bool is_unlabelled(const T& value,
                           const T& foreground) {
            return (value == foreground);
        }

        //////////////////////////////////////////////////////////////////////
        /// Trace a single contour in binary image data
        ///
        /// \param T              Image data type
        /// \param OutputIterator The type of the output iterator for storing
        ///                       the contour
        /// \param UnaryPredicate A unary predicate for identifying background
        ///                       elements
        /// \param view           View of the image data
        /// \param point          The point to start tracing from
        /// \param out            Output iterator to store the contour
        /// \param background     Value of background elements
        //////////////////////////////////////////////////////////////////////
        //template<typename RandomAccessIterator,
        //         typename OutputIterator>
        //void trace_contour(const array_view<RandomAccessIterator>& view,
        //                   const point2i& point,
        //                   OutputIterator out,
        //                   typename std::iterator_traits<RandomAccessIterator>::value_type background) {
        //    trace_contour(view,
        //                  point.x,
        //                  point.y,
        //                  out,
        //                  background);
        //}

        //////////////////////////////////////////////////////////////////////
        /// Trace a single contour in binary image data from a given starting
        /// point.
        ///
        /// This implements the "Contour tracing" procedure from the article,
        /// and since contour tracing is not used for labelling here, we do
        /// not need to label white pixels with negative integers.
        ///
        /// \param RandomAccessIterator Image data type
        /// \param AssociativeContainer An associative container type
        /// \param OutputIterator       The type of the output iterator for
        ///                             storing the contour
        /// \param view                 View of the image data
        /// \param x                    X-coordinate of the starting to trace
        ///                             from
        /// \param y                    Y-coordinate of the starting to trace
        ///                             from
        /// \param marked               Marked external and internal contour
        ///                             points
        /// \param out                  Output iterator to store the contour
        /// \param background           Value of background elements
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator,
                 typename OutputIterator,
                 typename AssociativeContainer>
        void trace_contour(array_view<RandomAccessIterator>& view,
                           std::size_t x,
                           std::size_t y,
                           unsigned int label,
                           OutputIterator out,
                           AssociativeContainer& marked,
                           typename std::iterator_traits<RandomAccessIterator>::value_type background,
                           bool is_external) {
            point2i start(x, y);
            point2i pos = start;
            int next_dir = (is_external ? 7 : 3);
            int first_dir = next_dir;

            if (!is_external) {
                marked.emplace(x, y);
            }

            view(start) = label;
            *out++ = pos;

            std::cout << x << ", " << y << std::endl;

            // Start tracing the contour until we encounter the starting point
            // again and the next point is the same as when we started
            do {
                int count = next_contour_point(view, pos, next_dir, background);

                std::cout << next_dir << std::endl;

                if (count == 8) {
                    // We are tracing an isolated point, so stop here
                    break;
                } else {
                    view(pos) = label;
                    *out++ = pos;
                    next_dir = ++next_dir % 8;

                    if (is_internal_contour(view, x, y, background)) {
                        marked.emplace(pos.x, pos.y);
                    }
                }
            } while (pos != start && next_dir != first_dir);
        }

        //////////////////////////////////////////////////////////////////////
        /// Trace a single external contour.
        ///
        /// \param RandomAccessIterator Image data type
        /// \param AssociativeContainer An associative container type
        /// \param view                 View of the image data
        /// \param x                    X-coordinate of the starting to trace
        ///                             from
        /// \param y                    Y-coordinate of the starting to trace
        ///                             from
        /// \param cc                   Connected component associated with
        ///                             the external contour
        /// \param marked               Marked external and internal contour
        ///                             points
        /// \param background           Value of background elements
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator,
                 typename AssociativeContainer>
        void trace_external_contour(array_view<RandomAccessIterator>& view,
                                    std::size_t x,
                                    std::size_t y,
                                    connected_component& cc,
                                    typename std::iterator_traits<RandomAccessIterator>::value_type background,
                                    AssociativeContainer& marked) {
            // Note: Preallocate some points?
            auto out = std::back_inserter(cc._contour);
            bool is_external = true;

            trace_contour(view,
                          x, y,
                          cc.label(),
                          out,
                          marked,
                          background,
                          is_external);
        }

        //////////////////////////////////////////////////////////////////////
        /// Trace a single internal contour.
        ///
        /// \param RandomAccessIterator Image data type
        /// \param AssociativeContainer An associative container type
        /// \param view                 View of the image data
        /// \param x                    X-coordinate of the starting to trace
        ///                             from
        /// \param y                    Y-coordinate of the starting to trace
        ///                             from
        /// \param cc                   Connected component associated with
        ///                             the external contour
        /// \param marked               Marked external and internal contour
        ///                             points
        /// \param background           Value of background elements
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator,
                 typename AssociativeContainer>
        void trace_internal_contour(array_view<RandomAccessIterator>& view,
                                    std::size_t x,
                                    std::size_t y,
                                    connected_component& cc,
                                    typename std::iterator_traits<RandomAccessIterator>::value_type background,
                                    AssociativeContainer& marked) {
            cc._inner_contours.emplace_back();
            // Note: Preallocate some points?
            auto out = std::back_inserter(cc._inner_contours.back());
            bool is_external = false;

            std::cout << "i" << std::endl;

            trace_contour(view,
                          x, y,
                          cc.label(),
                          out,
                          marked,
                          background,
                          is_external);
        }

        //////////////////////////////////////////////////////////////////////
        /// Label the connected components in a binary image and trace all
        /// contours. The implementation is based on "A linear-time component-
        /// labeling algorithm using contour tracing technique" by Fu Chang,
        /// Chun-Jen Chen and Chi-Jen Lu, with slight modifications to avoid
        /// modifying the image data.
        /// 
        /// \param OutputIterator The type of the output iterator for storing
        /// \param first          Iterator to the beginning of the image
        ///                       data source
        /// \param last           Iterator to the end of the image data
        ///                       source
        /// \param out            Output iterator to store the contour
        /// \param out            Output iterator for storing connected
        ///                       components, e.g. a std::vector<>
        /// \param width          Width of the image data
        /// \param height         Height of the image data
        /// \param pitch          Pitch of the image data
        /// \param connectivity   Neighbourhood connectivity (4 or 8)
        /// \param background     Value of background elements
        //////////////////////////////////////////////////////////////////////
        template<typename RandomAccessIterator,
                 typename OutputIterator>
        std::size_t contour_label(array_view<RandomAccessIterator>& view,
                                  OutputIterator out,
                                  unsigned char connectivity,
                                  typename std::iterator_traits<RandomAccessIterator>::value_type foreground,
                                  typename std::iterator_traits<RandomAccessIterator>::value_type background,
                                  const feature_flag& flags) {
            using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;

            std::size_t label_count = 0;
            const bool extract_inner_contours = any_flags(flags & feature_flag::inner_contours);
            std::vector<connected_component> components;

            // We separately track marked pixels to avoid modifying mage data
            // (Is there a better alternative?)
            std::unordered_set<point2i> marked;

            for (std::size_t y = 0; y < view.height(); ++y) {
                for (std::size_t x = 0; x < view.width(); ++x) {
                    value_type& e = view(y, x);
                    
                    //if (y > 0 && e == 0) {
                    //    std::cout << e << ", " << view(y - 1, x) << std::endl;
                    //}

                    if (e == background) {
                        //e = 0;
                    } else if (is_unlabelled(e, foreground)) {
                        if (is_external_contour(view, x, y, background)) {
                            std::cout << "EXTERNAL" << std::endl;
                            components.emplace_back(++label_count);
                            e = label_count;

                            trace_external_contour(view,
                                                   x, y,
                                                   components.back(),
                                                   background,
                                                   marked);
                        } else if (extract_inner_contours) {
                            if (is_internal_contour(view, x, y, background)) {
                                if (marked.count(point2i(x, y)) == 0) {
                                    trace_internal_contour(view,
                                                           x, y,
                                                           components[e - 1],
                                                           background,
                                                           marked);
                                }

                                e = view(y, x - 1);
                            }
                        } else {
                            e = view(y, x - 1);
                        }
                    }
                }
            }

            std::move(components.begin(),
                      components.end(),
                      out);

            components.clear();

            return label_count;
        }
    } // detail
} // cvx

#endif // CVX_CONTOUR_DETAIL_HPP
