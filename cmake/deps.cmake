macro(DEPS)

	foreach(dep "${ARGN}")
		string(CONCAT __dep_path "${CMAKE_SOURCE_DIR}" "/" "${dep}")
		
		STORE_DEP(${__current_target_name} ${__dep_path})
		
		unset(__dep_path)
	endforeach()

endmacro()