#ifndef CVX_EXPORT_HPP
#define CVX_EXPORT_HPP

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) 
    #ifdef CVX_SHARED_LIBRARY
        #define CVX_EXPORT __declspec(dllexport)
    #else
        #define CVX_EXPORT __declspec(dllimport)
    #endif
#else
    #define CVX_EXPORT
#endif

#endif // CVX_EXPORT_HPP
