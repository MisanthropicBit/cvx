#ifndef CVX_CONVEX_HULL_HPP
#define CVX_CONVEX_HULL_HPP

#include "cvx/export.hpp"
#include <deque>
#include <iterator>

namespace cvx {
    namespace detail {
        //////////////////////////////////////////////////////////////////////
        /// Determine which side a point lies relative to a line spanned by a
        /// and b
        ///
        /// \param T Coordinate type of the point
        /// \param p Query point
        /// \param a One end-point of the line
        /// \param b The other end-point of the line
        /// \return -1 if on the left side, 0 on the line and 1 if on the
        ///            right side
        //////////////////////////////////////////////////////////////////////
        template<typename T>
        CVX_EXPORT short point_line_side(const point2<T>& p, const point2<T>& a, const point2<T>& b) {
            static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                          "T must be integral or floating point type");

            return (a.x - p.x) * (b.y - p.y) - (b.x - p.x) * (a.y - p.y);
        }

        //////////////////////////////////////////////////////////////////////
        /// An implementation of Melkman's algorithm for finding the convex
        /// hull of a set of points in O(n) time. The points of the resulting
        /// convex hull are returned in counter clock-wise order
        ///
        /// This is guaranteed to work with connected components as the chain
        /// of points passed as input are always simple given the component's
        /// outer contour
        ///
        /// \param InputIterator  Input iterator type
        /// \param OutputIterator A destination iterator for the convex hull
        /// \param first          Iterator to the beginning of the point set
        /// \param last           Iterator to the end of the point set
        /// \param out            Destination for the convex hull points
        //////////////////////////////////////////////////////////////////////
        template<typename InputIterator, typename OutputIterator>
        CVX_EXPORT void melkman_convex_hull(InputIterator first, InputIterator last, OutputIterator out) {
            std::deque<typename std::iterator_traits<InputIterator>::value_type> deque;

            auto vertex1 = *first++;
            auto vertex2 = *first++;
            auto vertex3 = *first++;

            deque.emplace_front(vertex3, vertex1, vertex2, vertex3);

            // Construct an initial convex hull from the first three points
            if (point_line_side(vertex1, vertex2, vertex3) >= 0) {
                std::swap(deque[1], deque[2]);
            }

            while (first != last) {
                auto point = *first++;

                // If the current point is interior to the growing convex hull, ignore it
                if (point_line_side(deque[0], deque[1], point) > 0 &&
                    point_line_side(deque[deque.size() - 2], deque[deque.size() - 1], point) > 0) {
                    continue;
                }

                // If the
                while (point_line_side(deque[0], deque[1], point) <= 0) {
                    deque.pop_back();
                }

                deque.push_back(point);

                // If the
                while (point_line_side(deque[deque.size() - 2], deque[deque.size() - 1], point) <= 0) {
                    deque.pop_front();
                }

                deque.push_front(point);
            }

            // Move the hull points to the output iterator
            std::move(deque.begin(), deque.end(), out);
        }
    } // detail
} // cvx

#endif // CVX_CONVEX_HULL_HPP
