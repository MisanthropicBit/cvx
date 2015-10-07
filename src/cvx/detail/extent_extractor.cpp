#include "cvx/detail/extent_extractor.hpp"

namespace cvx {
    namespace detail {
        void extent_extractor::initialise(connected_component& component) {}

        void extent_extractor::update(std::size_t x, std::size_t y, connected_component& component) {}

        void extent_extractor::finalise(connected_component& component) {
            //for (auto component : components) {
            //    component._extent = component._bounding_box.area();
            //}
        }
    }
}
