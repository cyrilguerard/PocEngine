file(GLOB_RECURSE DEMO_01_SRC_DIR
	${PROJECT_SOURCE_DIR}/demos/demo-01-simple-window/*.h
	${PROJECT_SOURCE_DIR}/demos/demo-01-simple-window/*.cpp)

add_executable(demo-01-simple-window ${DEMO_01_SRC_DIR})

# PoC Engine
target_include_directories(demo-01-simple-window PUBLIC ${PROJECT_SOURCE_DIR}/poc-engine)
target_link_libraries(demo-01-simple-window "poc-engine")

if(PLATFORM EQUAL 64)
	install(TARGETS demo-01-simple-window CONFIGURATIONS Debug DESTINATION ${CMAKE_SOURCE_DIR}/bin/debug)
	install(TARGETS demo-01-simple-window CONFIGURATIONS Release DESTINATION ${CMAKE_SOURCE_DIR}/bin/release)
elseif(PLATFORM EQUAL 32)
	install(TARGETS demo-01-simple-window CONFIGURATIONS Debug DESTINATION ${CMAKE_SOURCE_DIR}/bin32/debug)
	install(TARGETS demo-01-simple-window CONFIGURATIONS Release DESTINATION ${CMAKE_SOURCE_DIR}/bin32/release)
endif()
