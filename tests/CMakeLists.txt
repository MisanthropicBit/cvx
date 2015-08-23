add_executable(test_area_extraction test_area_extraction.cpp ${CVX_SOURCES})
set_target_properties(test_area_extraction PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/tests/bin)
add_test(test_area_extraction ${PROJECT_SOURCE_DIR}/tests/bin/test_area_extraction)

add_executable(test_bounding_box_extraction test_bounding_box_extraction.cpp ${CVX_SOURCES})
set_target_properties(test_bounding_box_extraction PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/tests/bin)
add_test(test_bounding_box_extraction ${PROJECT_SOURCE_DIR}/tests/bin/test_bounding_box_extraction)

#add_executable(test_centroid_extraction test_centroid_extraction.cpp ${CVX_SOURCES})
#add_test(test_centroid_extraction test_centroid_extraction)

#add_executable(test_point_extraction test_point_extraction.cpp ${CVX_SOURCES})
#add_test(test_point_extraction test_point_extraction)
