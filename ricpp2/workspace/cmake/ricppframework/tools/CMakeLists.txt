set ( tools_src
      ${RICPP_SOURCE_DIR}/tools/stringlist.cpp
      ${RICPP_SOURCE_DIR}/tools/stringpattern.cpp
)

if ( ${WIN32} )
	set ( tools_src
              ${tools_src}
              ${RICPP_SOURCE_DIR}/tools/win32filepath.cpp
              ${RICPP_SOURCE_DIR}/tools/win32env.cpp
        )
elseif ( ${APPLE} )
	set ( tools_src
              ${tools_src}
              ${RICPP_SOURCE_DIR}/tools/macfilepath.cpp
              ${RICPP_SOURCE_DIR}/tools/maclinuxfilepath.cpp
              ${RICPP_SOURCE_DIR}/tools/macenv.cpp
              ${RICPP_SOURCE_DIR}/tools/maclinuxenv.cpp
        )
else ()
	if ( NOT ${UNIX} )
		message ("Warning: Unknown architecture using same as Unix.")
	endif ()
	set ( tools_src
              ${tools_src}
              ${RICPP_SOURCE_DIR}/tools/linuxfilepath.cpp
              ${RICPP_SOURCE_DIR}/tools/maclinuxfilepath.cpp
              ${RICPP_SOURCE_DIR}/tools/linuxenv.cpp
              ${RICPP_SOURCE_DIR}/tools/maclinuxenv.cpp
        )
endif ()

add_library ( tools STATIC ${tools_src} )

