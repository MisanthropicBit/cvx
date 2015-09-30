# Enable C++11 for different CMake versions
#
# Inspired by the script from Kevin M. Godby (kevin@godby.org)
# Note that Microsoft Visual C++ compiler enables C++11 by default

if (NOT (${CMAKE_VERSION} LESS 3.1))
    set(CMAKE_CXX_STANDARD 11)
else()
    include(CheckCXXCompilerFlag)

    check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
    check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

    if (COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif (COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        message(ERROR "The compiler ${CMAKE_CXX_COMPILER} does not support C++11 support.")
    endif()
#endif()
