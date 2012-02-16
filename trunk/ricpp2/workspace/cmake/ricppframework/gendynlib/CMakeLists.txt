set ( gendynlib_src
      ${RICPP_SOURCE_DIR}/gendynlib/dynlib.cpp
      ${RICPP_SOURCE_DIR}/gendynlibwin32/dynlib.cpp
      ${RICPP_SOURCE_DIR}/gendynlib/macdynlib.cpp
      ${RICPP_SOURCE_DIR}/gendynlib/linuxdynlib.cpp
)

add_library ( gendynlib STATIC ${gendynlib_src} )
