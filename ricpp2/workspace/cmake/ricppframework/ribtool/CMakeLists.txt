set ( ribtool_src
      ${RICPP_SOURCE_DIR}/ribtool/ribtool.cpp
)

add_executable ( ribtool ${ribtool_src} )
target_link_libraries ( ribtool ${ricppbridge_libs} )

