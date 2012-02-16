set ( tools_src
      ${RICPP_SOURCE_DIR}/tools/stringlist.cpp
      ${RICPP_SOURCE_DIR}/tools/stringpattern.cpp
      ${RICPP_SOURCE_DIR}/tools/win32path.cpp
      ${RICPP_SOURCE_DIR}/tools/win32inc.cpp
      ${RICPP_SOURCE_DIR}/tools/macenv.cpp
      ${RICPP_SOURCE_DIR}/tools/macpath.cpp
      ${RICPP_SOURCE_DIR}/tools/linuxenv.cpp
      ${RICPP_SOURCE_DIR}/tools/linuxpath.cpp
      ${RICPP_SOURCE_DIR}/tools/maclinuxenv.cpp
      ${RICPP_SOURCE_DIR}/tools/maclinuxpath.cpp
)

add_library ( tools STATIC ${tools_src} )

