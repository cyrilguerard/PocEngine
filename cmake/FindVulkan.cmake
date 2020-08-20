if(WIN32)
  
	find_path(VULKAN_INC NAMES vulkan/vulkan.h PATHS "${PROJECT_3RD_PARTY_DIR}/include")
	  
	if(PLATFORM EQUAL 64)
		find_library(VULKAN_LIB NAMES vulkan-1 PATHS "${PROJECT_3RD_PARTY_DIR}/lib/win64")
	
	elseif(PLATFORM EQUAL 32)
		find_library(VULKAN_LIB NAMES vulkan-1 PATHS "${PROJECT_3RD_PARTY_DIR}/lib/win32")
	
	endif()

endif()
