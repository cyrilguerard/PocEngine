if(WIN32)

	if(PLATFORM EQUAL 64)
		find_path(GLFW3_INC NAMES GLFW/glfw3.h PATHS "${PROJECT_3RD_PARTY_DIR}/include/glfw3/win64")
		find_library(GLFW3_LIB NAMES glfw3 PATHS "${PROJECT_3RD_PARTY_DIR}/lib/win64")
	
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
		find_path(GLFW3_INC NAMES GLFW/glfw3.h PATHS "${PROJECT_3RD_PARTY_DIR}/include/glfw3/win32")
		find_library(GLFW3_LIB NAMES glfw3 PATHS "${PROJECT_3RD_PARTY_DIR}/lib/win32")
	
	endif()
	
endif()
