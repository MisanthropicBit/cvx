#include "cvx/detail/extent_extractor.hpp"

namespace cvx {
    namespace detail {
        void extent_extractor::initialise() {}

        void extent_extractor::update(std::size_t x, std::size_t y, connected_component& cc) {}

        void extent_extractor::finalise(connected_component& cc) {
            //for (auto cc : components) {
            //    cc._extent = cc._bounding_box.area();
            //}
        }
    }
}
