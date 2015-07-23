#ifndef CVX_CONNECTED_COMPONENT_HPP
#define CVX_CONNECTED_COMPONENT_HPP

#include "cvx/array_view.hpp"
#include "cvx/color.hpp"
#include "cvx/export.hpp"
#include "cvx/point2.hpp"
#include "cvx/rectangle2.hpp"
#include <algorithm>
#include <limits>
#include <vector>

// Forward class declarations
namespace cvx {
    template<typename RandomAccessIterator>
    class array_view;

    class connected_component;

    namespace detail {
        class area_extractor;
        class centroid_extractor;
        class extent_extractor;
        class point_extractor;
        class bounding_box_extractor;

        template<typename RandomAccessIterator,
                 typename AssociativeContainer>
        void trace_external_contour(array_view<RandomAccessIterator>&,
                                    std::size_t,
                                    std::size_t,
                                    connected_component&,
                                    typename std::iterator_traits<RandomAccessIterator>::value_type,
                                    AssociativeContainer&);

        template<typename RandomAccessIterator,
                 typename AssociativeContainer>
        void trace_internal_contour(array_view<RandomAccessIterator>&,
                                    std::size_t,
                                    std::size_t,
                                    connected_component&,
                                    typename std::iterator_traits<RandomAccessIterator>::value_type,
                                    AssociativeContainer&);
    }
}

namespace cvx {
    //////////////////////////////////////////////////////////////////////
    /// Class representing a connected component
    ///
    /// Most 'getter' methods are lazy in the sense that if a feature was
    /// not initially extracted, it can be computed from as needed, e.g.
    /// the bounding box from its set of component points etc. If not,
    /// an exception is thrown
    //////////////////////////////////////////////////////////////////////
    class CVX_EXPORT connected_component final {
        // Friend declarations for all extractors
        friend class detail::area_extractor;
        friend class detail::centroid_extractor;
        friend class detail::extent_extractor;
        friend class detail::point_extractor;
        friend class detail::bounding_box_extractor;

        template<typename RandomAccessIterator,
                 typename AssociativeContainer>
        friend void detail::trace_external_contour(array_view<RandomAccessIterator>&,
                                                   std::size_t,
                                                   std::size_t,
                                                   connected_component&,
                                                   typename std::iterator_traits<RandomAccessIterator>::value_type,
                                                   AssociativeContainer&);

        template<typename RandomAccessIterator,
                 typename AssociativeContainer>
        friend void detail::trace_internal_contour(array_view<RandomAccessIterator>&,
                                                   std::size_t,
                                                   std::size_t,
                                                   connected_component&,
                                                   typename std::iterator_traits<RandomAccessIterator>::value_type,
                                                   AssociativeContainer&);

        public:
            //////////////////////////////////////////////////////////////////////
            /// Create an empty connected component
            //////////////////////////////////////////////////////////////////////
            connected_component();

            //////////////////////////////////////////////////////////////////////
            /// Create a connected component with a given label
            ///
            /// \param label Label assigned to this component
            //////////////////////////////////////////////////////////////////////
            connected_component(unsigned int label);

            //////////////////////////////////////////////////////////////////////
            /// Fill the interiors of the blob with a specified color
            ///
            /// \param color Fill color
            //////////////////////////////////////////////////////////////////////
            void set_fill_color(const color& fill_color);

            //////////////////////////////////////////////////////////////////////
            /// \return The color to fill this component with when displayed
            //////////////////////////////////////////////////////////////////////
            color fill_color() const;

            //////////////////////////////////////////////////////////////////////
            /// Query if a given point is part of the component
            ///
            /// \param p The query point
            /// \return True if the point is part of the component, false
            ///         otherwise
            //////////////////////////////////////////////////////////////////////
            bool contains(const point2i& p) const;

            //////////////////////////////////////////////////////////////////////
            /// \return The label of the blob
            //////////////////////////////////////////////////////////////////////
            std::size_t label() const noexcept;

            //////////////////////////////////////////////////////////////////////
            /// \return The area of the blob
            //////////////////////////////////////////////////////////////////////
            std::size_t size() const;

            //////////////////////////////////////////////////////////////////////
            /// Alias for size()
            ///
            /// \return The area of the blob
            //////////////////////////////////////////////////////////////////////
            std::size_t area() const;
            
            //////////////////////////////////////////////////////////////////////
            /// \return True if the connected component has no points or area
            //////////////////////////////////////////////////////////////////////
            bool empty() const;
            
            //////////////////////////////////////////////////////////////////////
            /// \param Precision The desired floating-point precision
            /// \return The center of mass of the blob
            //////////////////////////////////////////////////////////////////////
            template<typename Precision = float>
            point2<Precision> centroid() const {
                static_assert(std::is_floating_point<Precision>::value,
                              "Precision must be a floating point type");

                if (_centroid.x == -1.f && _centroid.y == -1.f) {
                    if (points().empty()) {
                        throw exception("Need at least point set to compute centroid");
                    }

                    _centroid.zero();

                    for (const auto& p : points()) {
                        _centroid += p;
                    }

                    _centroid.x /= static_cast<Precision>(points().size());
                    _centroid.y /= static_cast<Precision>(points().size());
                }

                return point2<Precision>(_centroid);
            }

            //////////////////////////////////////////////////////////////////////
            /// \return The points that the blob consists of
            //////////////////////////////////////////////////////////////////////
            const std::vector<point2i>& points() const;

            //////////////////////////////////////////////////////////////////////
            /// \return The outer contour of the blob
            //////////////////////////////////////////////////////////////////////
            const std::vector<point2i>& contour() const;

            //////////////////////////////////////////////////////////////////////
            /// \return True if this component has no inner contours
            //////////////////////////////////////////////////////////////////////
            bool solid() const noexcept;

            //////////////////////////////////////////////////////////////////////
            /// \return The inner contour of the connected component 
            //////////////////////////////////////////////////////////////////////
            //const std::vector<std::vector<point2i>>& inner_contours() const;

            //////////////////////////////////////////////////////////////////////
            /// \return The convex hull of the component
            //////////////////////////////////////////////////////////////////////
            std::vector<point2i> convex_hull() const;

            //////////////////////////////////////////////////////////////////////
            /// \return The convex hull of the component
            //////////////////////////////////////////////////////////////////////
            //ellipse ellipse() const;

            //////////////////////////////////////////////////////////////////////
            /// \return The view of the image data corresponding to the
            ///         component's bounding box
            //////////////////////////////////////////////////////////////////////
            //array_view<T> view();

            //////////////////////////////////////////////////////////////////////
            /// \return The view of the image data corresponding to the
            ///         component's bounding box
            //////////////////////////////////////////////////////////////////////
            //const array_view<T> view() const;

            //////////////////////////////////////////////////////////////////////
            /// \return The length of the outer contour as per the marching
            ///         squares algorithm
            //////////////////////////////////////////////////////////////////////
            //template<typename Precision = float>
            //Precision contour_length_ms();

            std::vector<std::vector<point2i>> contour_hierarchy() const;

            //////////////////////////////////////////////////////////////////////
            /// \return The blob's bounding box
            //////////////////////////////////////////////////////////////////////
            rectangle2i bounding_box() const;

            //////////////////////////////////////////////////////////////////////
            /// \return The extent of the blob
            //////////////////////////////////////////////////////////////////////
            template<typename Precision = float>
            Precision extent() const {
                static_assert(std::is_floating_point<Precision>::value,
                              "Precision must be a floating point type");

                if (_extent == -1.f) {
                    _extent = static_cast<Precision>(bounding_box().area()) /
                              static_cast<Precision>(size());
                }

                return _extent;
            }

            //////////////////////////////////////////////////////////////////////
            /// Merges this blob with another. This operation may be expensive as
            /// it recalculates all statistics
            ///
            /// \param other The other connected component to merge with
            //////////////////////////////////////////////////////////////////////
            void merge(const connected_component& other);

            //////////////////////////////////////////////////////////////////////
            /// Fill all the internal holes in the connected component. Once a
            /// component has been filled, it cannot be "unfilled" again
            //////////////////////////////////////////////////////////////////////
            void fill();

            //////////////////////////////////////////////////////////////////////
            /// \return The smallest distance to another component
            //////////////////////////////////////////////////////////////////////
            template<typename Precision = float>
            void distance(const connected_component& other) const {
                static_assert(std::is_floating_point<Precision>::value,
                              "Precision must be floating-point");

                Precision dist = std::numeric_limits<Precision>::max();

                for (auto i = cbegin(); i != cend(); ++i) {
                    for (auto j = other.cbegin(); j != other.cend(); ++j) {
                        dist = std::min(dist, i->distance(*j));
                    }
                }

                return dist;
            }

            ////////////////////////////////////////////////////////////////////////
            ///// \return The maximum intensity value of the component
            ////////////////////////////////////////////////////////////////////////
            //unsigned char max_intensity() const;
            
            ////////////////////////////////////////////////////////////////////////
            ///// \return The minimum intensity value of the component
            ////////////////////////////////////////////////////////////////////////
            //unsigned char min_intensity() const;

            //template<typename Precision = float>
            //Precision mean_intensity() const {
            //    static_assert(std::is_floating_point<Precision>::value,
            //                  "Precision must be floating-point");

            //    if (_mean_intensity == -1.f) {
            //        Precision sum = 0.f;

            //        //for (auto& p : _points) {
            //            //sum += _view(p.y, p.x);
            //        //}

            //        _mean_intensity = sum / static_cast<Precision>(_points.size());
            //    }

            //    return _mean_intensity;
            //}

            //template<typename Precision = float>
            //Precision stddev_intensity() const {
            //    static_assert(std::is_floating_point<Precision>::value,
            //                  "Precision must be floating-point");

            //    if (_stddev_intensity == -1.f) {
            //        Precision diff = 0.f;

            //        //for (auto& p : _points) {
            //            //diff += std::pow(_view(p.y, p.x) - mean_intensity(), 2);
            //        //}

            //        _mean_intensity = diff / static_cast<Precision>(_points.size());
            //    }

            //    return _mean_intensity;
            //}

            //////////////////////////////////////////////////////////////////////
            /// Iterators for the connected component's points and contour
            //////////////////////////////////////////////////////////////////////
            using const_iterator = std::vector<point2i>::const_iterator;

            const_iterator cbegin()         const noexcept;
            const_iterator cend()           const noexcept;
            const_iterator contour_cbegin() const noexcept;
            const_iterator contour_cend()   const noexcept;
            const_iterator hull_cbegin()    const noexcept;
            const_iterator hull_cend()      const noexcept;

        private:
            //point2i centroid();
            //std::vector<point2i>& points();
            //std::vector<point2i>& outer_contour();
            //std::vector<std::vector<point2i>>& inner_contours();
            //std::size_t& area();
            //rectangle2i bounding_box();
            //void set_view(const array_view<T>& view);

        private:
            unsigned int _label;
            color _color;
            mutable std::size_t _area;
            mutable point2f _centroid;
            mutable std::vector<point2i> _points;
            mutable std::vector<point2i> _contour;
            mutable std::vector<std::vector<point2i>> _inner_contours;
            mutable std::vector<point2i> _hull;
            mutable rectangle2i _bounding_box;
            mutable float _extent;
            mutable short _max_intensity;
            mutable short _min_intensity;
            mutable float _mean_intensity;
            mutable float _stddev_intensity;
            //array_view<T> _view;
    };
} // cvx

#endif // CVX_CONNECTED_COMPONENT_HPP
