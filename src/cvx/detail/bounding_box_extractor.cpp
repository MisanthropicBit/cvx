#include "cvx/connected_component.hpp"
#include "cvx/detail/bounding_box_extractor.hpp"
#include <algorithm>

namespace cvx {
    namespace detail {
        void bounding_box_extractor::initialise(connected_component& component) {}

        void bounding_box_extractor::update(std::size_t x, std::size_t y, connected_component& component) {
            //component._bounding_box.adjust(x, y); // TODO: Why does this not work?
            //component.bounding_box().x = std::min(component.bounding_box().x, static_cast<int>(x));
            //component.bounding_box().y = std::min(component.bounding_box().y, static_cast<int>(y));
            //component.bounding_box().width = std::max(component.bounding_box().width, static_cast<int>(x));
            //component.bounding_box().height = std::max(component.bounding_box().height, static_cast<int>(y));
            component._bounding_box.x = std::min(component._bounding_box.x, static_cast<int>(x));
            component._bounding_box.y = std::min(component._bounding_box.y, static_cast<int>(y));
            component._bounding_box.width = std::max(component._bounding_box.width, static_cast<int>(x));
            component._bounding_box.height = std::max(component._bounding_box.height, static_cast<int>(y));
        }

        void bounding_box_extractor::finalise(connected_component& component) {
            //component.bounding_box().width  = component.bounding_box().width - component.bounding_box().x;
            //component.bounding_box().height = component.bounding_box().height - component.bounding_box().y;
            component._bounding_box.width = component._bounding_box.width - component._bounding_box.x + 1;
            component._bounding_box.height = component._bounding_box.height - component._bounding_box.y + 1;
        }
    }
}
