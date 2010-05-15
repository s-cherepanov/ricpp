cmake_minimum_required (VERSION 2.8)
project(RICPPFRAMEWORK)

set ( RICPP_SOURCE_DIR ${RICPPFRAMEWORK_SOURCE_DIR}/../../../source )
set ( RICPP_INCLUDE_DIR ${RICPPFRAMEWORK_SOURCE_DIR}/../../../source/include )

include_directories ( ${RICPP_INCLUDE_DIR} )

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
add_subdirectory (streams)
add_subdirectory (tools)

# *** dynamic libraries
# add_subdirectory (glrendererdll)
# add_subdirectory (passthrough)
# add_subdirectory (ribwriterdll)
# add_subdirectory (ridynload)
# add_subdirectory (riprog)

# *** programs
# add_subdirectory (glutrib)
# add_subdirectory (ribtool)
# add_subdirectory (test)
# add_subdirectory (testpoly)
# add_subdirectory (testribind)

