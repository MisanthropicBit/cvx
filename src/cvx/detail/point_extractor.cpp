#include "cvx/connected_component.hpp"
#include "cvx/detail/point_extractor.hpp"

namespace cvx {
    namespace detail {
        void point_extractor::initialise(connected_component& component) {}

        void point_extractor::update(std::size_t x, std::size_t y, connected_component& component) {
            //component.points().emplace_back(x, y);
            component._points.emplace_back(x, y);
        }

        void point_extractor::finalise(connected_component& component) {
            //component.area() = component.points().size();
            component._area = component._points.size();
        }
    }
}
