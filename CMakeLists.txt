cmake_minimum_required(VERSION 2.8)

# Disregard setting PROJECT_VERSION etc. through the 'project' command
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 OLD)
endif()

# Disregard setting @rpath on Mac OSX
if(POLICY CMP0042)
    cmake_policy(SET CMP0042 OLD)
endif()

# Set up some useful aliases
set(BASE_NAME               cvx)
set(PROJECT_VERSION         0.1.0)
set(CVX_FULL_NAME           ${BASE_NAME}-${PROJECT_VERSION})
project(${CVX_FULL_NAME})

# Add all project sources
set(CVX_SOURCE_PREFIX ${PROJECT_SOURCE_DIR}/src/cvx)
set(CVX_SOURCES       ${CVX_SOURCE_PREFIX}/color.cpp
                      ${CVX_SOURCE_PREFIX}/connected_component.cpp
                      ${CVX_SOURCE_PREFIX}/draw.cpp
                      ${CVX_SOURCE_PREFIX}/exception.cpp
                      #${CVX_SOURCE_PREFIX}/detail/contour.cpp
                      ${CVX_SOURCE_PREFIX}/detail/extractor.cpp
                      ${CVX_SOURCE_PREFIX}/detail/area_extractor.cpp
                      ${CVX_SOURCE_PREFIX}/detail/bounding_box_extractor.cpp
                      ${CVX_SOURCE_PREFIX}/detail/centroid_extractor.cpp
                      ${CVX_SOURCE_PREFIX}/detail/extent_extractor.cpp
                      ${CVX_SOURCE_PREFIX}/detail/point_extractor.cpp)

# Set up options
option(CVX_STATIC_LIBRARY "Build cvx as a static library" OFF)
option(CVX_SHARED_LIBRARY "Build cvx as a shared library" ON)
option(CVX_WITH_OPENCV    "Build cvx with OpenCV support" OFF)
option(CVX_GEN_DOCS       "Generate offline documention"  OFF)
option(CVX_BUILD_EXAMPLES "Build all examples"            ON)
option(CVX_BUILD_TESTS    "Build all tests"               ON)

# Modify path to locate cmake Find* modules and custom functions
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(${PROJECT_SOURCE_DIR}/cmake/Functions.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Enable appropriate compiler flags
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    list(APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    list(APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif()

add_definitions(${CMAKE_CXX_FLAGS})

#--------------------------------------------------------------------
# Generate docs
#--------------------------------------------------------------------
if(CVX_GEN_DOCS)
    find_package(Doxygen QUIET)

    if(DOXYGEN_FOUND)
        set(CVX_DOCS_DIR "${PROJECT_SOURCE_DIR}/docs")
        file(MAKE_DIRECTORY ${CVX_DOCS_DIR})
        configure_file()

        # Run doxygen on the project
        add_custom_target(docs
                          COMMAND           ${DOXYGEN_EXECUTABLE} ${CVX_DOXYGEN_FILE}
                          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    else()
        message(WARNING "Doxygen was not found, cannot generate documentation")
    endif()
endif()

#--------------------------------------------------------------------
# Build examples
#--------------------------------------------------------------------
# First build examples that do not require OpenCV
if(CVX_BUILD_EXAMPLES)
    if(CVX_WITH_OPENCV)
        find_package(OpenCV COMPONENTS opencv_core opencv_imgproc opencv_highgui)
        include_directories(${OpenCV_INCLUDE_DIRS})

        if(OpenCV_FOUND)
            add_definitions(-DCVX_WITH_OPENCV)
        else()
            message(WARNING "Unable to find OpenCV, some examples will not be built")
        endif()
    endif()
    
    add_subdirectory(${PROJECT_SOURCE_DIR}/examples)
endif()

#--------------------------------------------------------------------
# Build tests
#--------------------------------------------------------------------
if(CVX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()

#--------------------------------------------------------------------
# Build static/shared libraries
#--------------------------------------------------------------------
if(CVX_STATIC_LIBRARY OR CVX_SHARED_LIBRARY)
    set(CVX_LIB_DIR "${PROJECT_SOURCE_DIR}/lib")
    file(MAKE_DIRECTORY ${CVX_LIB_DIR})

    if(CVX_STATIC_LIBRARY)
        add_library(${CVX_FULL_NAME}_static STATIC ${CVX_SOURCES})
        set_target_properties(${CVX_FULL_NAME}_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CVX_LIB_DIR})
    endif()

    if(CVX_SHARED_LIBRARY)
        add_library(${CVX_FULL_NAME} SHARED ${CVX_SOURCES})
        set_target_properties(${CVX_FULL_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CVX_LIB_DIR})
        add_definitions(-DCVX_SHARED_LIBRARY)
    endif()

    if(CVX_WITH_OPENCV AND OpenCV_FOUND)
        target_link_libraries(${CVX_FULL_NAME} ${OpenCV_LIBRARIES})
    endif()
endif()
