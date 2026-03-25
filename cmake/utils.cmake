macro(SET_GLOBAL_DEFINITIONS DEFINITION_VAR)
  foreach(item ${${DEFINITION_VAR}})
	set(DEF_VAR -D${item})
    add_definitions(${DEF_VAR})
  endforeach()
endmacro()

macro(ADD_TGT_DEFINITIONS TARGET DEFINITION_VAR)
  #message("TARGET: ${TARGET}")
  #message("DEFINITION_VAR: ${${DEFINITION_VAR}}")
  foreach(item ${${DEFINITION_VAR}})
    #string(CONCAT DEF_VAR "-D" ${item})
    #message("DEF_VAR: ${DEF_VAR}")
    target_compile_definitions(${TARGET} PUBLIC ${item})
  endforeach()
endmacro()


MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
	message("PrecompiledBinary: ${PrecompiledBinary}")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

MACRO(PLATFORM_LIBS PLATFORM_NAME)
    if (${CMAKE_SYSTEM_NAME} STREQUAL ${PLATFORM_NAME})
        foreach(__lib ${ARGN})
            target_link_libraries(${__current_target_name} ${__lib})
        endforeach()
    endif()
ENDMACRO()

include("python")
include("library")
include("end")
include("srcs")
include("program")
include("deps")