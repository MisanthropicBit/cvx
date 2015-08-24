add_executable(opencv_example opencv_example.cpp ${CVX_SOURCES})
set_target_properties(opencv_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/examples/bin)

add_executable(pitched_array pitched_array.cpp ${CVX_SOURCES})
set_target_properties(pitched_array PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/examples/bin)

add_executable(feature_extraction feature_extraction.cpp ${CVX_SOURCES})
set_target_properties(feature_extraction PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/examples/bin)

add_executable(find_contours find_contours.cpp ${CVX_SOURCES})
set_target_properties(find_contours PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/examples/bin)

# Link the OpenCV libraries to the appropriate examples
if(OpenCV_FOUND)
    target_link_libraries(opencv_example     ${OpenCV_LIBS})
    target_link_libraries(pitched_array      ${OpenCV_LIBS})
    target_link_libraries(feature_extraction ${OpenCV_LIBS})
    target_link_libraries(find_contours      ${OpenCV_LIBS})
endif()
