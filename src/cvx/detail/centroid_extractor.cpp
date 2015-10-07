#include "cvx/detail/centroid_extractor.hpp"
#include "cvx/connected_component.hpp"

namespace cvx {
    namespace detail {
        void centroid_extractor::initialise(connected_component& component) {
            component._centroid.zero();
        }

        void centroid_extractor::update(std::size_t x, std::size_t y, connected_component& component) {
            //component.centroid().x += x;
            //component.centroid().y += y;
            component._centroid.x += x;
            component._centroid.y += y;
        }

        void centroid_extractor::finalise(connected_component& component) {
            const float area = static_cast<float>(component.area());
            component._centroid.x /= area;
            component._centroid.y /= area;

            //const auto area = component.area();
            //component._centroid.x /= area;
            //component._centroid.y /= area;

            //component.centroid().x /= area;
            //component.centroid().y /= area;
        }
    }
}
