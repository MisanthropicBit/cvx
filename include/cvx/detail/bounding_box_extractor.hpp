#ifndef CVX_BOUNDING_BOX_EXTRACTOR_HPP
#define CVX_BOUNDING_BOX_EXTRACTOR_HPP

#include "cvx/detail/extractor.hpp"

namespace cvx {
    namespace detail {
        class bounding_box_extractor final : public extractor {
            public:
                void initialise() override;
                void update(std::size_t x, std::size_t y, connected_component& cc) override;
                void finalise(connected_component& cc) override;
        };
    } // detail
} // cvx

#endif // CVX_BOUNDING_BOX_EXTRACTOR_HPP
