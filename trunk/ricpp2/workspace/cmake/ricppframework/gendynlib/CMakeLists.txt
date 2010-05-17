set ( gendynlib_src ${RICPP_SOURCE_DIR}/gendynlib/dynlib.cpp )

if ( ${WIN32} )
	set ( gendynlib_src
              ${gendynlib_src}
              ${RICPP_SOURCE_DIR}/gendynlib/win32dynlib.cpp
	)
elseif ( ${APPLE} )
	set ( gendynlib_src
              ${gendynlib_src}
              ${RICPP_SOURCE_DIR}/tools/macdynlib.cpp
        )
else ()
	if ( NOT ${UNIX} )
		message ("Warning: Unknown architecture using same as Unix.")
	endif ()
	set ( gendynlib_src
              ${gendynlib_src}
              ${RICPP_SOURCE_DIR}/gendynlib/linuxdynlib.cpp
        )
endif ()

add_library ( gendynlib STATIC ${gendynlib_src} )

