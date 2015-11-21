#ifndef CVX_ARRAY_VIEW_HPP
#define CVX_ARRAY_VIEW_HPP

#include "cvx/exception.hpp"
#include "cvx/export.hpp"
#include "cvx/point2.hpp"
#include "cvx/rectangle2.hpp"
#include "cvx/utils.hpp"
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <type_traits>

#ifdef CVX_WITH_OPENCV
    #include <opencv2/opencv.hpp>
#endif

namespace cvx {
    //////////////////////////////////////////////////////////////////////
    /// Abstracts a 2D view of an arbitrary range of values
    ///
    /// \param <RandomAccessIterator> Type of the iterator range that is
    ///                               being viewed
    //////////////////////////////////////////////////////////////////////
    template<typename RandomAccessIterator>
    class CVX_EXPORT array_view final {

        #if defined(CVX_WITH_OPENCV) && !defined(OPENCV_NOSTL)
            // We check this before the typedefs to avoid excess errors
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
                : array_view(RandomAccessIterator(),
                             RandomAccessIterator(),
                             0,
                             0) {
            }

            //////////////////////////////////////////////////////////////////////
            /// Create a view of some data
            ///
            /// \param first  Iterator to the beginning of the data
            /// \param last   Iterator to the end of the data
            /// \param width  Width of the data
            /// \param height Height of the data
            //////////////////////////////////////////////////////////////////////
            array_view(RandomAccessIterator first,
                       RandomAccessIterator last,
                       size_type width,
                       size_type height)
                : array_view(first,
                             last,
                             width,
                             height,
                             rectangle2<size_type>(0, 0, width, height)) {
            }

            //////////////////////////////////////////////////////////////////////
            /// Create a subview of another array view
            ///
            /// \param view Iterator to the beginning of the data
            /// \param roi  Rectangle defining the subview or region of interest
            //////////////////////////////////////////////////////////////////////
            template<typename U>
            array_view(const array_view<RandomAccessIterator>& view,
                       const rectangle2<U>& roi)
                : array_view(view.first,
                             view.last,
                             view.width(),
                             view.height(),
                             roi) {
            }

            //////////////////////////////////////////////////////////////////////
            /// Create a subview of some data
            ///
            /// \param first  Iterator to the beginning of the data
            /// \param last   Iterator to the end of the data
            /// \param width  Width of the data
            /// \param height Height of the data
            /// \param roi    Rectangle defining the subview or region of interest
            //////////////////////////////////////////////////////////////////////
            template<typename U>
            array_view(RandomAccessIterator first,
                       RandomAccessIterator last,
                       size_type width,
                       size_type height,
                       const rectangle2<U>& roi)
                : first(first + roi.x + roi.y * width),
                  last(last + roi.right() + roi.bottom() * width),
                  _bounds(roi) {
                check_bounds();
            }

            #ifdef CVX_WITH_OPENCV
                //////////////////////////////////////////////////////////////////////
                /// Create a view of an OpenCV Mat(rix)
                ///
                /// \param mat An OpenCV matrix
                //////////////////////////////////////////////////////////////////////
                array_view(const cv::Mat& mat)
                    : array_view(mat.begin(),
                                 mat.end(),
                                 mat.width,
                                 mat.height) {
                }

                //////////////////////////////////////////////////////////////////////
                /// Create a subview of an OpenCV Mat(rix)
                ///
                /// \param mat    An OpenCV matrix
                /// \param bounds Bounds specifying the subview
                //////////////////////////////////////////////////////////////////////
                array_view(const cv::Mat& mat,
                           const rectangle2i& roi)
                    : array_view(mat.begin(),
                                 mat.end(),
                                 mat.width,
                                 mat.height,
                                 roi) {
                }

                //////////////////////////////////////////////////////////////////////
                /// Create a view of an OpenCV templated Mat(rix)
                ///
                /// \param view Iterator to the beginning of the data
                /// \param roi  Rectangle defining the subview or region of interest
                //////////////////////////////////////////////////////////////////////
                template<typename T>
                array_view(const cv::Mat_<T>& mat)
                    : array_view(mat.begin(),
                                 mat.end(),
                                 mat.width,
                                 mat.height) {
                }

                //////////////////////////////////////////////////////////////////////
                /// Create a subview of an OpenCV templated Mat(rix)
                ///
                /// \param mat    An OpenCV matrix
                /// \param bounds Bounds specifying the subview
                //////////////////////////////////////////////////////////////////////
                template<typename T>
                array_view(const cv::Mat_<T>& mat,
                           const rectangle2i& bounds)
                    : array_view(mat.begin(),
                                 mat.end(),
                                 mat.width,
                                 mat.height,
                                 bounds) {
                }
            #endif // CVX_WITH_OPENCV

            //////////////////////////////////////////////////////////////////////
            /// \return True if the view is pointing to valid data
            //////////////////////////////////////////////////////////////////////
            bool valid() const noexcept {
                return (first != last); 
            }

            //////////////////////////////////////////////////////////////////////
            /// \return True if the view contains the given value
            //////////////////////////////////////////////////////////////////////
            bool contains(RandomAccessIterator value) const {
                RandomAccessIterator end = cend();

                return std::find(cbegin(), end, value) != end;
            }

            //////////////////////////////////////////////////////////////////////
            /// Access the data pointed to by the view by index
            ///
            /// \param i Index
            /// \return The data at i
            //////////////////////////////////////////////////////////////////////
            reference operator[](size_type i) {
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
            const reference operator[](size_type i) const {
                return operator[](i);
                //size_type x = i % width();
                //size_type y = i / width();

                //return operator()(y, x);
            }

            //////////////////////////////////////////////////////////////////////
            /// Access the data pointed to by the view by index. Does bounds
            /// checking as well.
            ///
            /// \param i Index
            /// \return The data at i
            //////////////////////////////////////////////////////////////////////
            reference at(size_type i) {
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
            const reference at(size_type i) const {
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
            reference at(size_type x, size_type y) {
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
            const reference at(size_type y, size_type x) const {
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
            reference operator()(size_type y, size_type x) {
                return *(first + width() * y + x);
            }

            //////////////////////////////////////////////////////////////////////
            /// Treat the viewed data as a 2D array and access the data at (x, y)
            ///
            /// \param y Y-coordinate of data
            /// \param x X-coordinate of data
            /// \return The data at (x, y)
            //////////////////////////////////////////////////////////////////////
            const reference operator()(size_type y, size_type x) const {
                return *(first + x + y * step());
                return *(first + width() * y + x);
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
            /// Return a pointer to the specified row
            ///
            /// \param y Row to query
            /// \return The yth row
            //////////////////////////////////////////////////////////////////////
            pointer row(size_type y) {
                if (y >= height()) {
                    throw exception("Y-coordinate out of bounds");
                }

                return first + y * width();
            }

            //////////////////////////////////////////////////////////////////////
            /// Return a pointer to the specified row
            ///
            /// \param y Row to query
            /// \return The yth row
            //////////////////////////////////////////////////////////////////////
            const pointer row(size_type y) const {
                if (y >= height()) {
                    throw exception("Y-coordinate out of bounds");
                }

                return first + y * width();
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The width of the data viewed as a 2D array
            //////////////////////////////////////////////////////////////////////
            size_type width() const noexcept {
                return _width;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The height of the data viewed as a 2D array
            //////////////////////////////////////////////////////////////////////
            size_type height() const noexcept {
                return _height;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The stride or pitch of the data viewed as a 2D array
            //////////////////////////////////////////////////////////////////////
            size_type pitch() const noexcept {
                return _width * sizeof(value_type);
            }

            //////////////////////////////////////////////////////////////////////
            /// Create a subview from this view
            ///
            /// \param bounds The bounds specifying the subview
            /// \return The subview of this view
            //////////////////////////////////////////////////////////////////////
            array_view<RandomAccessIterator> subview(const rectangle2i& bounds) {
                if (!this->bounds().contains(bounds)) {
                    throw std::out_of_range("Subview bounds out of range");
                }

                if (bounds.area() == 0) {
                    throw std::logic_error("Cannot create an empty subview");
                }

                return array_view<RandomAccessIterator>(*this,
                                                        bounds);
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The total number of elements in the view
            //////////////////////////////////////////////////////////////////////
            size_type size() const noexcept {
                return _width * _height;
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The total number of bytes spanned by the view
            //////////////////////////////////////////////////////////////////////
            size_type bytesize() const noexcept {
                return pitch() * _height;
            }
            
            //////////////////////////////////////////////////////////////////////
            /// The (x, y) coordinate of the rectangle specifies an offset if this
            /// view is a subview of a parent view
            ///
            /// \return The bounds spanned by the view.
            //////////////////////////////////////////////////////////////////////
            rectangle2i bounds() const {
                return rectangle2i(0, 0, width(), height());
            }

            //////////////////////////////////////////////////////////////////////
            /// \return True if this is a subview of another view, false otherwise
            //////////////////////////////////////////////////////////////////////
            bool is_subview() const {
                return (_step < width());
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
            inline size_type step() const noexcept {
                return _step;
            }

            void check_bounds() {
                difference_type dist = last - first;

                if (dist >= size()) {
                    throw exception("Iterator range out of bounds in array view");
                }
            }

        private:
            RandomAccessIterator first, last;
            size_type _width, _height, _step;
            rectangle2<size_type> _bounds;
    };
} // cvx

#endif // CVX_ARRAY_VIEW_HPP
