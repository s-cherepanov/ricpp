set ( tools_src
      ${RICPP_SOURCE_DIR}/tools/stringlist.cpp
      ${RICPP_SOURCE_DIR}/tools/stringpattern.cpp
      ${RICPP_SOURCE_DIR}/tools/win32env.cpp
      ${RICPP_SOURCE_DIR}/tools/win32filepath.cpp
      ${RICPP_SOURCE_DIR}/tools/macenv.cpp
      ${RICPP_SOURCE_DIR}/tools/macfilepath.cpp
      ${RICPP_SOURCE_DIR}/tools/linuxenv.cpp
      ${RICPP_SOURCE_DIR}/tools/linuxfilepath.cpp
      ${RICPP_SOURCE_DIR}/tools/maclinuxenv.cpp
      ${RICPP_SOURCE_DIR}/tools/maclinuxfilepath.cpp
)

add_library ( tools STATIC ${tools_src} )

