macro(PROGRAM NAME)

	set(__is_program TRUE)
	set(__current_target_name "${NAME}")
	
	add_executable(${__current_target_name} "")
	STORE_TARGET_PATH(${__current_target_name} ${CMAKE_CURRENT_SOURCE_DIR})
	
endmacro()