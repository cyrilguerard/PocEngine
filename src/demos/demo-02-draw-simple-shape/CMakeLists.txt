set(DEMO_NAME demo-02-draw-simple-shape)

file(GLOB_RECURSE DEMO_SRC_DIR
	${PROJECT_SOURCE_DIR}/demos/${DEMO_NAME}/*.h
	${PROJECT_SOURCE_DIR}/demos/${DEMO_NAME}/*.cpp)

add_executable(${DEMO_NAME} ${DEMO_SRC_DIR})

# PoC Engine
target_include_directories(${DEMO_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/poc-engine)
target_link_libraries(${DEMO_NAME} "poc-engine")

if(PLATFORM EQUAL 64)
	install(TARGETS ${DEMO_NAME} CONFIGURATIONS Debug DESTINATION ${CMAKE_SOURCE_DIR}/bin/debug)
	install(TARGETS ${DEMO_NAME} CONFIGURATIONS Release DESTINATION ${CMAKE_SOURCE_DIR}/bin/release)
elseif(PLATFORM EQUAL 32)
	install(TARGETS ${DEMO_NAME} CONFIGURATIONS Debug DESTINATION ${CMAKE_SOURCE_DIR}/bin32/debug)
	install(TARGETS ${DEMO_NAME} CONFIGURATIONS Release DESTINATION ${CMAKE_SOURCE_DIR}/bin32/release)
endif()
