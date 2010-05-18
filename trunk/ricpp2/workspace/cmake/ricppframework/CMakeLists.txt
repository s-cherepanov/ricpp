cmake_minimum_required (VERSION 2.8)

project(RICPPFRAMEWORK)

set ( RICPP_SOURCE_DIR
      ${RICPPFRAMEWORK_SOURCE_DIR}/../../../source
)

set ( RICPP_INCLUDE_DIR
      ${RICPPFRAMEWORK_SOURCE_DIR}/../../../source/include
)

# ADD_DEFINITIONS (-Wall)

include_directories (
	${RICPP_INCLUDE_DIR}
)
link_directories (
	${RICPPFRAMEWORK_BINARY_DIR}/tools
	${RICPPFRAMEWORK_BINARY_DIR}/ribase
	${RICPPFRAMEWORK_BINARY_DIR}/ricpp
	${RICPPFRAMEWORK_BINARY_DIR}/gendynlib
	${RICPPFRAMEWORK_BINARY_DIR}/pluginhandler
	${RICPPFRAMEWORK_BINARY_DIR}/streams
	${RICPPFRAMEWORK_BINARY_DIR}/declaration
	${RICPPFRAMEWORK_BINARY_DIR}/ricontext
	${RICPPFRAMEWORK_BINARY_DIR}/ribfilter
	${RICPPFRAMEWORK_BINARY_DIR}/ribparser
	${RICPPFRAMEWORK_BINARY_DIR}/rendererloader
	${RICPPFRAMEWORK_BINARY_DIR}/ricppbridge
	${RICPPFRAMEWORK_BINARY_DIR}/baserenderer
	${RICPPFRAMEWORK_BINARY_DIR}/glrenderer
	${RICPPFRAMEWORK_BINARY_DIR}/ribwriter
	${RICPPFRAMEWORK_BINARY_DIR}/ri
	${RICPPFRAMEWORK_BINARY_DIR}/ridynload
	${RICPPFRAMEWORK_BINARY_DIR}/riprog
)

# *** static libraries
add_subdirectory (tools)
add_subdirectory (ribase)
add_subdirectory (ricpp)
add_subdirectory (gendynlib)
add_subdirectory (pluginhandler)
add_subdirectory (streams)
add_subdirectory (declaration)
add_subdirectory (ricontext)
add_subdirectory (ribfilter)
add_subdirectory (ribparser)
add_subdirectory (rendererloader)
add_subdirectory (ricppbridge)
add_subdirectory (baserenderer)
add_subdirectory (glrenderer)
add_subdirectory (ribwriter)
add_subdirectory (ri)
add_subdirectory (ridynload)

set (ricpp_libs ricpp ribase tools)
set (ridynload_libs ridynload ri ${ricpp_libs})
set (pluginhandler_libs pluginhandler gendynlib ${ricpp_libs} dl)
set (ribfilter_libs ribfilter ${pluginhandler_libs})
set (ricontext_libs ricontext declaration streams ${pluginhandler_libs} z)
set (ribparser_libs ribparser ribfilter ${ricontext_libs})
set (ricppbridge_libs ricppbridge rendererloader ${ribparser_libs})
set (baserenderer_libs baserenderer ${ribparser_libs})
set (glrenderer_libs glrenderer ${baserenderer_libs} GL)
set (ribwriter_libs ribwriter ${baserenderer_libs})

# *** dynamic libraries 
add_subdirectory (riprog)

# *** dynamic libraries (plugin modules)
add_subdirectory (glrendererdll)
add_subdirectory (ribwriterdll)
add_subdirectory (passthrough)

# *** programs
add_subdirectory (glutrib)
add_subdirectory (ribtool)

# add_subdirectory (test)
# add_subdirectory (testpoly)
# add_subdirectory (testribind)

# *** Dependencies between targets
# add_dependencies(glutrib riprog)
# add_dependencies(ribtool riprog)
# add_dependencies(glrendererdll glrenderer)
# add_dependencies(ribwriterdll ribwriter)
# add_dependencies(passthrough ribfilter)
# add_dependencies(riprog ri ricppbridge)
# add_dependencies(ridynload ri)
# add_dependencies(ri ricpp)
# add_dependencies(ricppbridge rendererloader ribparser)
# add_dependencies(glrenderer baserenderer)
# add_dependencies(ribwriter baserenderer)
# add_dependencies(baserenderer ribparser)
# add_dependencies(rendererloader ricontext)
# add_dependencies(ribparser ricontext ribfilter)
# add_dependencies(ricontext declaration streams)
# add_dependencies(ribfilter pluginhandler)
# add_dependencies(declaration ricpp)
# add_dependencies(streams pluginhandler)
# add_dependencies(pluginhandler gendynlib)
# add_dependencies(gendynlib ricpp)
# add_dependencies(ricpp ribase)
# add_dependencies(ribase tools)

