#ifndef CVX_FEATURE_FLAG_HPP
#define CVX_FEATURE_FLAG_HPP

#include <type_traits>

namespace cvx {
    enum class feature_flag : unsigned int {
        none            = 0x000, /// Retrieves no features (default)
        area            = 0x001, /// Gets component areas
        centroid        = 0x002, /// Computes each component centroid
        points          = 0x004, /// Gets point set of each component
        bounding_box    = 0x008, /// Gets the bounding box of each component
        extent          = 0x018, /// The size ratio between the component and its bounding box (also extract bounding box)
        outer_contours  = 0x020, /// Extract all outer contours
        inner_contours  = 0x040, /// Extract all inner contours (each component may have mulitple)
        all_contours    = 0x060, /// Extract all outer and inner contours
        convex_hull     = 0x080, /// The clock-wise convex hull of the component
        convex_hull_cw  = 0x100, /// See 'ConvexHull'
        convex_hull_ccw = 0x200, /// The counter-clock-wise convex hull 
        // max_intesity      = 0x020,
        // min_intensity     = 0x040,
        // mean_intensity    = 0x080,
        // stddev_intensity  = 0x180, // Also extracts mean intensity
        all              = 0x3ff
    };

    using U = typename std::underlying_type<feature_flag>::type;

    // Strongly typed enums need these bitwise operators defined to function like bitflags
    inline feature_flag operator&(const feature_flag& a, const feature_flag& b) {
        return feature_flag(static_cast<U>(a) & static_cast<U>(b));
    }

    inline feature_flag operator|(const feature_flag& a, const feature_flag& b) {
        return feature_flag(static_cast<U>(a) | static_cast<U>(b));
    }

    inline feature_flag operator^(const feature_flag& a, const feature_flag& b) {
        return feature_flag(static_cast<U>(a) ^ static_cast<U>(b));
    }

    inline feature_flag operator~(const feature_flag& f) {
        return feature_flag(~static_cast<U>(f));
    }

    inline bool operator!(const feature_flag& f) {
        return !static_cast<U>(f);
    }

    //inline U operator&=(const feature_flag& a, const feature_flag& b) {
    //    a = a & b;
    //    return a;
    //}

    //inline U operator|=(const feature_flag& a, const feature_flag& b) {
    //    a = a | b;
    //    return a;
    //}
    
    //inline U operator^=(const feature_flag& a, const feature_flag& b) {
    //    a = a ^ b;
    //    return a;
    //}

    // "Hack" to make feature_flag work in a boolean context
    inline bool any_flags(const feature_flag& flag) {
        return flag != feature_flag::none;
    }
} // cvx

#endif // CVX_FEATURE_FLAG_HPP
