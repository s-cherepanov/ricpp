set ( declaration_src
      ${RICPP_SOURCE_DIR}/declaration/declaration.cpp
      ${RICPP_SOURCE_DIR}/declaration/decldict.cpp
      ${RICPP_SOURCE_DIR}/declaration/tokenmap.cpp
)

add_library ( declaration STATIC ${declaration_src} )

