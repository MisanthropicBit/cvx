#ifndef CVX_ARRAY_VIEW_HPP
#define CVX_ARRAY_VIEW_HPP

#include "cvx/exception.hpp"
#include "cvx/export.hpp"
#include "cvx/point2.hpp"
#include "cvx/rectangle2.hpp"
#include "cvx/utils.hpp"
#include <iterator>
#include <type_traits>

namespace cvx {
    //////////////////////////////////////////////////////////////////////
    /// Abstracts a (pitched/strided) 2D view of an arbitrary range of
    /// values
    ///
    /// \param <T> Type of the iterator range that is viewed
    //////////////////////////////////////////////////////////////////////
    template<typename RandomAccessIterator>
    class CVX_EXPORT array_view final {

        #if defined(CVX_WITH_OPENCV) && !defined(OPENCV_NOSTL)
            // Note: We check this before the typedefs to avoid excess errors
            // Note: Update for contiguous_iterators when available
            static_assert(std::is_same<typename std::iterator_traits<RandomAccessIterator>::iterator_category,
                                       std::random_access_iterator_tag>::value,
                          "Iterator type must be random access");
        #endif

        public:
            using value_type        = typename std::iterator_traits<RandomAccessIterator>::value_type;
            using reference         = typename std::iterator_traits<RandomAccessIterator>::reference;
            using pointer           = typename std::iterator_traits<RandomAccessIterator>::pointer;
            using difference_type   = typename std::iterator_traits<RandomAccessIterator>::difference_type;
            using iterator_category = typename std::iterator_traits<RandomAccessIterator>::iterator_category;
            using iterator          = RandomAccessIterator;
            using const_iterator    = const RandomAccessIterator;
            using size_type         = std::size_t;

        public:
            //////////////////////////////////////////////////////////////////////
            /// Create an empty view
            //////////////////////////////////////////////////////////////////////
            array_view()
                : first(RandomAccessIterator()),
                  last(RandomAccessIterator()),
                  _width(0),
                  _height(0),
                  _pitch(0),
                  _pitched_width(0),
                  _padding(0) {}

            //////////////////////////////////////////////////////////////////////
            /// Create a view of some data
            ///
            /// \param first  Iterator to the beginning of the data
            /// \param last   Iterator to the end of the data
            /// \param width  Width of the data
            /// \param height Height of the data
            /// \param pitch  Pitch (in bytes) of the data
            //////////////////////////////////////////////////////////////////////
            array_view(RandomAccessIterator first,
                       RandomAccessIterator last,
                       std::size_t width,
                       std::size_t height,
                       std::size_t pitch)
                : first(first),
                  last(last),
                  _width(width),
                  _height(height),
                  _pitch(pitch),
                  _pitched_width(pitch / sizeof(value_type)),
                  _padding(_pitched_width - width) {
                if (!detail::check_pitched_distance(first, last, pitch, height)) {
                    throw exception("Iterator range does not match image dimensions");
                }
            }

            //////////////////////////////////////////////////////////////////////
            /// \return True if the view is pointing to valid data
            //////////////////////////////////////////////////////////////////////
            bool valid() const noexcept {
                return (first != last); 
            }

            //////////////////////////////////////////////////////////////////////
            /// \return True if the view contains the given point
            //////////////////////////////////////////////////////////////////////
            bool contains(const point2i& point) const noexcept {
                return (point.x < 0 || point.x >= width() || point.y < 0 || point.y >= height());
            }

            //////////////////////////////////////////////////////////////////////
            /// Access the data pointed to by the view by index
            ///
            /// \param i Index
            /// \return The data at i
            //////////////////////////////////////////////////////////////////////
            reference operator[](std::size_t i) {
                size_type x = i % width();
                size_type y = i / width();

                return operator()(y, x);
            }

            //////////////////////////////////////////////////////////////////////
            /// Access the data pointed to by the view by index
            ///
            /// \param i Index
            /// \return The data at i
            //////////////////////////////////////////////////////////////////////
            const reference operator[](std::size_t i) const {
                size_type x = i % width();
                size_type y = i / width();

                return operator()(y, x);
            }

            //////////////////////////////////////////////////////////////////////
            /// Access the data pointed to by the view by index. Does bounds
            /// checking as well.
            ///
            /// \param i Index
            /// \return The data at i
            //////////////////////////////////////////////////////////////////////
            reference at(std::size_t i) {
                if (i >= size()) {
                    throw exception("Index out of bounds");
                }

                return operator[](i);
            }

            //////////////////////////////////////////////////////////////////////
            /// Access the data pointed to by the view by index. Does bounds
            /// checking as well.
            ///
            /// \param i Index
            /// \return The data at i
            //////////////////////////////////////////////////////////////////////
            const reference at(std::size_t i) const {
                if (i >= size()) {
                    throw exception("Index out of bounds");
                }

                return operator[](i);
            }

            //////////////////////////////////////////////////////////////////////
            /// Treat the viewed data as a 2D array and access the data at (x, y)
            ///
            /// \param y Y-coordinate of data
            /// \param x X-coordinate of data
            /// \return The data at (x, y)
            //////////////////////////////////////////////////////////////////////
            reference at(std::size_t x, std::size_t y) {
                if (x >= width()) {
                    throw exception("X-coordinate out of bounds");
                }

                return row(y)[x];
            }

            //////////////////////////////////////////////////////////////////////
            /// Treat the viewed data as a 2D array and access the data at (x, y)
            ///
            /// \param y Y-coordinate of data
            /// \param x X-coordinate of data
            /// \return The data at (x, y)
            //////////////////////////////////////////////////////////////////////
            const reference at(std::size_t y, std::size_t x) const {
                if (x >= width()) {
                    throw exception("X-coordinate out of bounds");
                }

                return row(y)[x];
            }

            //////////////////////////////////////////////////////////////////////
            /// Treat the viewed data as a 2D array and access the data at (x, y)
            ///
            /// \param y Y-coordinate of data
            /// \param x X-coordinate of data
            /// \return The data at (x, y)
            //////////////////////////////////////////////////////////////////////
            reference operator()(std::size_t y, std::size_t x) {
                return *(first + _pitched_width * y + x);
            }

            //////////////////////////////////////////////////////////////////////
            /// Treat the viewed data as a 2D array and access the data at (x, y)
            ///
            /// \param point Where to access the data
            /// \return The data at (x, y)
            //////////////////////////////////////////////////////////////////////
            template<typename T>
            reference operator()(const point2<T>& point) {
                return operator()(point.y, point.x);
            }

            //////////////////////////////////////////////////////////////////////
            /// Treat the viewed data as a 2D array and access the data at (x, y)
            ///
            /// \param point Where to access the data
            /// \return The data at (x, y)
            //////////////////////////////////////////////////////////////////////
            template<typename T>
            const reference operator()(const point2<T>& point) const {
                return operator()(point.y, point.x);
            }

            //////////////////////////////////////////////////////////////////////
            /// Treat the viewed data as a 2D array and access the data at (x, y)
            ///
            /// \param y Y-coordinate of data
            /// \param x X-coordinate of data
            /// \return The data at (x, y)
            //////////////////////////////////////////////////////////////////////
            const reference operator()(std::size_t y, std::size_t x) const {
                return *(first + pitched_width() * y + x);
            }

            //////////////////////////////////////////////////////////////////////
            /// Return a pointer to the specified row
            ///
            /// \param y Row to query
            /// \return The yth row
            //////////////////////////////////////////////////////////////////////
            pointer row(std::size_t y) {
                if (y >= height()) {
                    throw exception("Y-coordinate out of bounds");
                }

                return first + y * pitched_width();
            }

            //////////////////////////////////////////////////////////////////////
            /// Return a pointer to the specified row
            ///
            /// \param y Row to query
            /// \return The yth row
            //////////////////////////////////////////////////////////////////////
            const pointer row(std::size_t y) const {
                if (y >= height()) {
                    throw exception("Y-coordinate out of bounds");
                }

                return first + y * pitched_width();
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The width of the data viewed as a 2D array
            //////////////////////////////////////////////////////////////////////
            std::size_t width() const noexcept {
                return _width;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The height of the data viewed as a 2D array
            //////////////////////////////////////////////////////////////////////
            std::size_t height() const noexcept {
                return _height;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The stride or pitch of the data viewed as a 2D array
            //////////////////////////////////////////////////////////////////////
            std::size_t pitch() const noexcept {
                return _pitch;
            }

            //////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////
            std::size_t pitched_width() const noexcept {
                return _pitched_width;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The padding applied to the width to achieve the right
            ///         stride or pitch
            //////////////////////////////////////////////////////////////////////
            std::size_t padding() const noexcept {
                return _padding;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The padding applied to the width to achieve the right
            ///         stride or pitch
            //////////////////////////////////////////////////////////////////////
            //array_view subview(const rectangle2i& bounds) {
            //    if (bounds.x < 0 || bounds.y < 0 || bounds.right() >= width || bounds.bottom() >= height) {
            //       throw std::out_of_range("Subview bounds out of range");
            //    }

            //    RandomAccessIterator iter = first + bounds.x + stride * bounds.y;

            //    return array_view(iter,
            //                     iter + bounds.area(), // Include padding
            //                     bounds.width,
            //                     bounds.height,
            //                     ?);

            //   return array_view<RandomAccessIterator>();
            //}

            //////////////////////////////////////////////////////////////////////
            /// \return The total number of elements in the view
            //////////////////////////////////////////////////////////////////////
            std::size_t size() const {
                return _width * _height;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The total number of bytes spanned by the view
            //////////////////////////////////////////////////////////////////////
            std::size_t bytesize() const {
                return _pitch * _height;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The total number of elements in the view, including the
            ///         pitch
            //////////////////////////////////////////////////////////////////////
            std::size_t pitchsize() const {
                return _pitched_width * _height;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return An iterator to the beginning of the data
            //////////////////////////////////////////////////////////////////////
            iterator begin() noexcept {
                return first;
            }
            
            //////////////////////////////////////////////////////////////////////
            /// \return An iterator to the end of the data
            //////////////////////////////////////////////////////////////////////
            iterator end() noexcept {
                return last;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return An iterator to the end of the data
            //////////////////////////////////////////////////////////////////////
            const_iterator cbegin() const noexcept {
                return first;
            }
            
            //////////////////////////////////////////////////////////////////////
            /// \return An iterator to the end of the data
            //////////////////////////////////////////////////////////////////////
            const_iterator cend() const noexcept {
                return last;
            }

        private:
            RandomAccessIterator first, last;
            std::size_t _width, _height, _pitch, _pitched_width, _padding;
    };
} // cvx

#endif // CVX_ARRAY_VIEW_HPP
