#include "cvx/connected_component.hpp"
#include "cvx/detail/bounding_box_extractor.hpp"
#include <algorithm>

namespace cvx {
    namespace detail {
        void bounding_box_extractor::initialise() {}

        void bounding_box_extractor::update(std::size_t x, std::size_t y, connected_component& cc) {
            //cc._bounding_box.adjust(x, y); // TODO: Why does this not work?
            //cc.bounding_box().x = std::min(cc.bounding_box().x, static_cast<int>(x));
            //cc.bounding_box().y = std::min(cc.bounding_box().y, static_cast<int>(y));
            //cc.bounding_box().width = std::max(cc.bounding_box().width, static_cast<int>(x));
            //cc.bounding_box().height = std::max(cc.bounding_box().height, static_cast<int>(y));
            cc._bounding_box.x = std::min(cc._bounding_box.x, static_cast<int>(x));
            cc._bounding_box.y = std::min(cc._bounding_box.y, static_cast<int>(y));
            cc._bounding_box.width = std::max(cc._bounding_box.width, static_cast<int>(x));
            cc._bounding_box.height = std::max(cc._bounding_box.height, static_cast<int>(y));
        }

        void bounding_box_extractor::finalise(connected_component& cc) {
            //cc.bounding_box().width  = cc.bounding_box().width - cc.bounding_box().x;
            //cc.bounding_box().height = cc.bounding_box().height - cc.bounding_box().y;
            cc._bounding_box.width = cc._bounding_box.width - cc._bounding_box.x;
            cc._bounding_box.height = cc._bounding_box.height - cc._bounding_box.y;
        }
    }
}
