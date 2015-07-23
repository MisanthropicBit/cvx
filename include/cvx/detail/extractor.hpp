#ifndef CVX_EXTRACTOR_HPP
#define CVX_EXTRACTOR_HPP

//#include "cvx/connected_component.hpp"
#include <cstdlib>

namespace cvx {
    class connected_component;
}

namespace cvx {
    namespace detail {
        class extractor {
            public:
                virtual void initialise();
                virtual void update(std::size_t x, std::size_t y, connected_component& cc);
                virtual void finalise(connected_component& cc);
        };
    } // detail
} // cvx

#endif // CVX_EXTRACTOR_HPP
