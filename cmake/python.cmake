MACRO(PYTHON PROGRAM)
	execute_process(COMMAND "python" "-d" "-c" "${PROGRAM}" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDMACRO()

MACRO(WRAPPER)
	execute_process(COMMAND "python" "${CMAKE_MODULE_PATH}/wrapper.py" ${ARGN} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDMACRO()

MACRO(RESET_DEPS)

	WRAPPER("reset_deps" "--path" "${CMAKE_BINARY_DIR}")

ENDMACRO()

MACRO(STORE_DEP CURRENT_TARGET DEP_ON_PATH)

	WRAPPER("store_dep" "--path" "${CMAKE_BINARY_DIR}" "--current-target" "${CURRENT_TARGET}" "--dep-on-path" "${DEP_ON_PATH}")

ENDMACRO()

MACRO(STORE_TARGET_PATH TARGET_NAME TARGET_PATH)

	WRAPPER("store_target_path" "--path" "${CMAKE_BINARY_DIR}" "--target-name" "${TARGET_NAME}" "--target-path" "${TARGET_PATH}")

ENDMACRO()

MACRO(BUILD_DEPS)

	execute_process(COMMAND "python" "${CMAKE_MODULE_PATH}/wrapper.py" "list_all_targets" "--path" "${CMAKE_BINARY_DIR}" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	OUTPUT_VARIABLE __all_targets)
	
	STRING(REGEX REPLACE "\n" ";" __all_targets "${__all_targets}")
	
	FOREACH(LINE ${__all_targets})
	
		execute_process(COMMAND "python" "${CMAKE_MODULE_PATH}/wrapper.py" "list_all_deps_for_target" "--path" "${CMAKE_BINARY_DIR}" "--target" "${LINE}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE __deps)
		
		STRING(REGEX REPLACE "\n" ";" __deps "${__deps}")
		
		FOREACH(DEP ${__deps})
			message("Adding dependency ${LINE} ON ${DEP}")
			add_dependencies(${LINE} ${DEP})
			
			get_property(__dep_type TARGET ${DEP} PROPERTY TYPE)
			IF (NOT ("${__dep_type}" EQUAL "EXECUTABLE"))
				get_property(__link_libs TARGET ${LINE} PROPERTY LINK_LIBRARIES)
				LIST(APPEND __link_libs ${DEP})
				set_property(TARGET ${LINE} PROPERTY LINK_LIBRARIES ${__link_libs})
			endif()
			
		endforeach()
		
	endforeach()
	unset(__link_libs)
	unset(__deps)
	unset(__dep_type)
	unset(__all_targets)
	
ENDMACRO()