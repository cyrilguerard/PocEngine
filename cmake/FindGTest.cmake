if(WIN32)
  
	find_path(GTEST_INC NAMES gtest/gtest.h PATHS "${PROJECT_3RD_PARTY_DIR}/include")
	  
	if(PLATFORM EQUAL 64)
		find_library(GTEST_LIB NAMES gtest PATHS "${PROJECT_3RD_PARTY_DIR}/lib/win64")
		find_library(GTEST_DBG_LIB NAMES gtestd PATHS "${PROJECT_3RD_PARTY_DIR}/lib/win64")
	
	elseif(PLATFORM EQUAL 32)
		find_library(GTEST_LIB NAMES gtest PATHS "${PROJECT_3RD_PARTY_DIR}/lib/win32")
		find_library(GTEST_DBG_LIB NAMES gtestd PATHS "${PROJECT_3RD_PARTY_DIR}/lib/win32")
	
	endif()

endif()
