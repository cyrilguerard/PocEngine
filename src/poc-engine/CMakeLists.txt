#find_package(glfw3 3.3 REQUIRED)

file(GLOB_RECURSE POC_ENGINE_SRC_DIR
	${PROJECT_SOURCE_DIR}/poc-engine/*.h
	${PROJECT_SOURCE_DIR}/poc-engine/*.cpp)
	
add_library(poc-engine STATIC ${POC_ENGINE_SRC_DIR})

message(WARN ${POC_ENGINE_SRC_DIR})

# Vulkan
find_package(Vulkan REQUIRED)
target_include_directories(poc-engine PUBLIC ${VULKAN_INC})
#target_link_libraries(poc-engine ${VULKAN_LIB})

# GLFW3
find_package(GLFW3 REQUIRED)
target_include_directories(poc-engine PUBLIC ${GLFW3_INC})
target_link_libraries(poc-engine ${GLFW3_LIB})

if(PLATFORM EQUAL 64)
	install(TARGETS poc-engine CONFIGURATIONS Debug DESTINATION ${CMAKE_SOURCE_DIR}/lib/debug)
	install(TARGETS poc-engine CONFIGURATIONS Release DESTINATION ${CMAKE_SOURCE_DIR}/lib/release)
elseif(PLATFORM EQUAL 32)
	install(TARGETS poc-engine CONFIGURATIONS Debug DESTINATION ${CMAKE_SOURCE_DIR}/lib32/debug)
	install(TARGETS poc-engine CONFIGURATIONS Release DESTINATION ${CMAKE_SOURCE_DIR}/lib32/release)
endif()


