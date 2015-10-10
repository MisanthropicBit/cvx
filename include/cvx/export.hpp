#ifndef CVX_EXPORT_HPP
#define CVX_EXPORT_HPP

#ifdef CVX_SHARED_LIBRARY
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) 
        #ifdef CVX_BUILING_SHARED_LIBRARY
            #define CVX_EXPORT __declspec(dllexport)
        #else
            #define CVX_EXPORT __declspec(dllimport)
        #endif
    #else
        // No definition needed for non-Windows platforms
        #define CVX_EXPORT
    #endif
#else
    // No definition needed for static library builds
    #define CVX_EXPORT
#endif

#endif // CVX_EXPORT_HPP
