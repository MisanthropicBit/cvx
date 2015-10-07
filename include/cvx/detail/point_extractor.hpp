#ifndef CVX_POINT_EXTRACTOR_HPP
#define CVX_POINT_EXTRACTOR_HPP

#include "cvx/detail/extractor.hpp"

namespace cvx {
    namespace detail {
        class point_extractor final : public extractor {
            public:
                void initialise(connected_component& component) override;
                void update(std::size_t x, std::size_t y, connected_component& component) override;
                void finalise(connected_component& component) override;
        };
    } // detail
} // cvx

#endif // CVX_POINT_EXTRACTOR_HPP
