set(TEST_NAME poc-tests)

file(GLOB_RECURSE TEST_SRC_DIR
	${PROJECT_SOURCE_DIR}/tests/*.hpp
	${PROJECT_SOURCE_DIR}/tests/*.cpp)

add_executable(${TEST_NAME} ${TEST_SRC_DIR})

# PoC Engine
target_include_directories(${TEST_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/poc-engine)
target_link_libraries(${TEST_NAME} "poc-engine")

# Gtest
find_package(GTest REQUIRED)
target_link_libraries(${TEST_NAME} debug ${GTEST_DBG_LIB})
target_link_libraries(${TEST_NAME} optimized ${GTEST_LIB})
