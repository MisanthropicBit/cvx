#include "cvx/detail/area_extractor.hpp"
#include "cvx/connected_component.hpp"

namespace cvx {
    namespace detail {
        void area_extractor::initialise(connected_component& component) {
        }

        void area_extractor::update(std::size_t x, std::size_t y, connected_component& component) {
            //++component.area();
            ++component._area;
        }

        void area_extractor::finalise(connected_component& component) {
        }
    }
}
