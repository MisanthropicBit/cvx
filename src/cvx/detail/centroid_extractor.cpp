#include "cvx/detail/centroid_extractor.hpp"
#include "cvx/connected_component.hpp"

namespace cvx {
    namespace detail {
        void centroid_extractor::initialise() {}

        void centroid_extractor::update(std::size_t x, std::size_t y, connected_component& cc) {
            //cc.centroid().x += x;
            //cc.centroid().y += y;
            cc._centroid.x += x;
            cc._centroid.y += y;
        }

        void centroid_extractor::finalise(connected_component& cc) {
            const auto area = cc.area();
            cc._centroid.x /= area;
            cc._centroid.y /= area;
            //cc.centroid().x /= area;
            //cc.centroid().y /= area;
        }
    }
}
