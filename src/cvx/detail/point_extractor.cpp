#include "cvx/connected_component.hpp"
#include "cvx/detail/point_extractor.hpp"

namespace cvx {
    namespace detail {
        void point_extractor::initialise() {}

        void point_extractor::update(std::size_t x, std::size_t y, connected_component& cc) {
            //cc.points().emplace_back(x, y);
            cc._points.emplace_back(x, y);
        }

        void point_extractor::finalise(connected_component& cc) {
            //cc.area() = cc.points().size();
            cc._area = cc._points.size();
        }
    }
}
