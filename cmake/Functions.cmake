# Function for building a cvx example
function(cvx_build_example target)
    add_executable(${target} ${target}.cpp ${CVX_SOURCES})
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/examples/bin)

    if(${CVX_WITH_OPENCV} AND OpenCV_FOUND)
        target_link_libraries(${target} ${OpenCV_LIBS})
    endif()
endfunction()

# Function for building a cvx test
function(cvx_build_test target)
    add_executable(${target} ${target}.cpp ${CVX_SOURCES})
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/tests/bin)

    if(${CVX_WITH_OPENCV} AND OpenCV_FOUND)
        target_link_libraries(${target} ${OpenCV_LIBS})
    endif()

    add_test(${target} ${PROJECT_SOURCE_DIR}/tests/bin/${target})
endfunction()
