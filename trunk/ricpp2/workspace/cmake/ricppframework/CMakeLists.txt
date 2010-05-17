cmake_minimum_required (VERSION 2.8)

project(RICPPFRAMEWORK)

set ( RICPP_SOURCE_DIR
      ${RICPPFRAMEWORK_SOURCE_DIR}/../../../source
)

set ( RICPP_INCLUDE_DIR
      ${RICPPFRAMEWORK_SOURCE_DIR}/../../../source/include
)

include_directories ( ${RICPP_INCLUDE_DIR} )
link_directories (lib bin)

# *** Dependencies between targets
# add_dependencies(riprog ri ricppbridge)
# add_dependencies(glrendererdll glrenderer)
# add_dependencies(ribwriterdll ribwriter)
# add_dependencies(passtrough ribfilter)
add_dependencies(ridynload ri)
add_dependencies(ri ricpp)
add_dependencies(ricppbridge rendererloader ribparser)
add_dependencies(glrenderer baserenderer)
add_dependencies(ribwriter baserenderer)
add_dependencies(baserenderer ribparser)
add_dependencies(rendererloder ricontext)
add_dependencies(ribparser ricontext ribfilter)
add_dependencies(ricontext declaration streams)
add_dependencies(ribfilter pluginhandler)
add_dependencies(declaration ricpp)
add_dependencies(streams pluginhandler)
add_dependencies(pluginhandler gendynlib)
add_dependencies(gendynlib ricpp)
add_dependencies(ricpp ribase)
add_dependencies(ribase tools)

# *** static libraries
add_subdirectory (baserenderer lib)
add_subdirectory (declaration lib)
add_subdirectory (gendynlib lib)
add_subdirectory (glrenderer lib)
add_subdirectory (pluginhandler lib)
add_subdirectory (rendererloader lib)
add_subdirectory (ri lib)
add_subdirectory (ribase lib)
add_subdirectory (ribfilter lib)
add_subdirectory (ribparser lib)
add_subdirectory (ribwriter lib)
add_subdirectory (ricontext lib)
add_subdirectory (ricpp lib)
add_subdirectory (ricppbridge lib)
add_subdirectory (ridynload bin)
add_subdirectory (streams lib)
add_subdirectory (tools lib)

# *** dynamic libraries 
# add_subdirectory (riprog lib)

# *** dynamic libraries (plugin modules)
# add_subdirectory (glrendererdll bin)
# add_subdirectory (passthrough bin)
# add_subdirectory (ribwriterdll bin)

# *** programs
# add_subdirectory (glutrib bin)
# add_subdirectory (ribtool bin)
# add_subdirectory (test bin)
# add_subdirectory (testpoly bin)
# add_subdirectory (testribind bin)

