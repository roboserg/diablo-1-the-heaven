macro (SRCS)
	if(NOT (${ARGC} GREATER 0))
		message(FATAL_ERROR "At least one source file should be specified")
	endif()
	
	foreach(__src "${ARGV}")
		set (__srcs ${__srcs} ${__src})
	endforeach()
	unset(__srcs_list)
	unset(__src)

endmacro()