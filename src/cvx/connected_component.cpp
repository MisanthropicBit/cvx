#include "cvx/connected_component.hpp"
#include "cvx/convex_hull.hpp"
#include "cvx/exception.hpp"
#include "cvx/detail/contour.hpp"
#include <iterator>

namespace cvx {
    connected_component::connected_component()
        : connected_component(0) {
    }

    connected_component::connected_component(unsigned int label)
      : _label(label),
        _area(0),
        _extent(0.0),
        _max_intensity(0),
        _min_intensity(0),
        _mean_intensity(0.0),
        _stddev_intensity(0.0) {
            // Properly initialise centroid, extent and bounding box to invalid states
            _centroid.x = -1.f;
            _centroid.y = -1.f;

            _extent = -1.f;

            _bounding_box.x = std::numeric_limits<int>::max();
            _bounding_box.y = std::numeric_limits<int>::max();
            _bounding_box.width  = 0;
            _bounding_box.height = 0;
    }

    void connected_component::set_fill_color(const color& fill_color) {
        _color = fill_color;
    }

    color connected_component::fill_color() const {
        return _color;
    }

    bool connected_component::contains(const point2i& p) const {
        if (points().empty()) {
            throw exception("No point data");
        }

        return std::find(_points.cbegin(), _points.cend(), p) != _points.cend();
    }

    std::size_t connected_component::label() const noexcept {
        return _label;
    }

    std::size_t connected_component::size() const {
        return _area;
    }

    std::size_t connected_component::area() const {
        return size();
    }

    bool connected_component::empty() const {
        return size() == 0;
    }

    const std::vector<point2i>& connected_component::points() const {
        if (_points.empty()) {
            rectangle2i bb = bounding_box();

            if (bb.width == -1 && bb.height == -1) {
                throw exception("Need to extract at least bounding box to infer point set");
            }

            for (std::size_t y = bb.y; y < bb.bottom(); ++y) {
                for (std::size_t x = bb.x; x < bb.right(); ++x) {
                    //if (view(y, x) == _label) {
                    //    _points.emplace_back(x, y);
                    //}
                }
            }
        }

        return _points;
    }

    const std::vector<point2i>& connected_component::contour() const {
        if (_contour.empty()) {
            //const auto& temp = points();
            //detail::trace_contour(_view, temp[0], std::back_inserter(_contour));
        }

        return _contour;
    }

    bool connected_component::solid() const noexcept {
        return false;
       //return _inner_contours.empty();
    }

    //const std::vector<std::vector<point2i>>& connected_component::inner_contours() const {
    //    //if (_inner_contours.empty()) {
    //        //const auto& temp = points();
    //        //detail::trace_contour(_view, temp[0], std::back_inserter(_inner_contours));
    //    //}

    //    //return _inner_contours;
    //}

    std::vector<point2i> connected_component::convex_hull() const {
        if (_hull.empty()) {
            //convex_hull(cbegin(), cend(), std::back_inserter(_hull));
        }

        return _hull;
    }

    //array_view<T> connected_component::view() {
        //return view->subview(_bounding_box);
    //}

    rectangle2i connected_component::bounding_box() const {
        if (_bounding_box.area() == 0) {
            if (_points.empty()) {
                throw exception("Need at least point set to infer bounding box");
            }

            for (auto& p : _points) {
                _bounding_box.adjust(p);
            }
        }

        return _bounding_box;
    }

    //unsigned char connected_component::max_intensity() const {
    //    if (_max_intensity == -1) {
    //        //for (auto& p : _points) {
    //            //_max_intensity = std::max(_max_intensity, view(p.y, p.x));
    //        //}
    //    }

    //    return static_cast<unsigned char>(_max_intensity);
    //}

    //unsigned char connected_component::min_intensity() const {
    //    if (_max_intensity == 256) {
    //        //for (auto& p : _points) {
    //            //_min_intensity = std::min(_min_intensity, view(p.y, p.x));
    //        //}
    //    }

    //    return static_cast<unsigned char>(_min_intensity);
    //}

    void connected_component::merge(const connected_component& other) {
        if (other.size() > 0) {
            //_points.insert(cend(), other.cbegin(), other.cend());
            
            // TODO: Update all internal component properties
            //_label = std::min(_label, other.label());
            //_color = ((_color.r + other.color().r) / 255.f);
            //invalidate();
        }
    }

    void connected_component::fill() {
        //if (!solid()) {
        //    if (points().empty()) {
        //        throw exception("Need at least point set to fill component");
        //    }

        //    if (inner_contours().empty()) {
        //        // No inner contours, fill using the component's points
        //        //auto point_iter = cbegin();
        //        //int cy = point_iter++->y;

        //        //while (true) {
        //        //    while (cy == point_iter->y) {
        //        //        if (point_iter->x != x) {
        //        //            _points.insert();
        //        //        }

        //        //        ++point_iter;
        //        //    }

        //        //    cy = point_iter->y;
        //        //}
        //   } else {
        //       // Use the inner contours to fill the component

        //       //_inner_contours.clear();
        //    }
        //}
    }

    connected_component::const_iterator connected_component::cbegin() const noexcept {
        return points().cbegin();
    }

    connected_component::const_iterator connected_component::cend() const noexcept {
        return points().cend();
    }

    connected_component::const_iterator connected_component::contour_cbegin() const noexcept {
        return contour().begin();
    }

    connected_component::const_iterator connected_component::contour_cend() const noexcept {
        return contour().end();
    }

    connected_component::const_iterator connected_component::hull_cbegin() const noexcept {
        return convex_hull().cbegin();
    }

    connected_component::const_iterator connected_component::hull_cend() const noexcept {
        return convex_hull().cend();
    }
} // cvx
