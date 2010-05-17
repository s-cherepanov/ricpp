cmake_minimum_required (VERSION 2.8)

project(RICPPFRAMEWORK)

set ( RICPP_SOURCE_DIR
      ${RICPPFRAMEWORK_SOURCE_DIR}/../../../source
)

set ( RICPP_INCLUDE_DIR
      ${RICPPFRAMEWORK_SOURCE_DIR}/../../../source/include
)

include_directories ( ${RICPP_INCLUDE_DIR} )
# link_directories ()

# *** Dependencies between targets
# add_dependencies(riprog ri ricppbridge)
# add_dependencies(glrendererdll glrenderer)
# add_dependencies(ribwriterdll ribwriter)
# add_dependencies(passtrough ribfilter)
# add_dependencies(ridynload ri)
# add_dependencies(ri ricpp)
# add_dependencies(ricppbridge rendererloader ribparser)
# add_dependencies(glrenderer baserenderer)
# add_dependencies(ribwriter baserenderer)
# add_dependencies(baserenderer ribparser)
# add_dependencies(rendererloder ricontext)
# add_dependencies(ribparser ricontext ribfilter)
# add_dependencies(ricontext declaration streams)
# add_dependencies(ribfilter pluginhandler)
# add_dependencies(declaration ricpp)
# add_dependencies(streams pluginhandler)
# add_dependencies(pluginhandler gendynlib)
# add_dependencies(gendynlib ricpp)
# add_dependencies(ricpp ribase)
# add_dependencies(ribase tools)

# *** static libraries
add_subdirectory (baserenderer)
add_subdirectory (declaration)
add_subdirectory (gendynlib)
add_subdirectory (glrenderer)
add_subdirectory (pluginhandler)
add_subdirectory (rendererloader)
add_subdirectory (ri)
add_subdirectory (ribase)
add_subdirectory (ribfilter)
add_subdirectory (ribparser)
add_subdirectory (ribwriter)
add_subdirectory (ricontext)
add_subdirectory (ricpp)
add_subdirectory (ricppbridge)
add_subdirectory (ridynload)
add_subdirectory (streams)
add_subdirectory (tools)

# *** dynamic libraries 
# add_subdirectory (riprog)

# *** dynamic libraries (plugin modules)
# add_subdirectory (glrendererdll)
# add_subdirectory (passthrough)
# add_subdirectory (ribwriterdll)

# *** programs
# add_subdirectory (glutrib)
# add_subdirectory (ribtool)
# add_subdirectory (test)
# add_subdirectory (testpoly)
# add_subdirectory (testribind)

