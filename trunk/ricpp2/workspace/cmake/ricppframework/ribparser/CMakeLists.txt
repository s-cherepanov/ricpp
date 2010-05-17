set ( ribparser_src
      ${RICPP_SOURCE_DIR}/ribparser/ribattributes.cpp
      ${RICPP_SOURCE_DIR}/ribparser/ribexternals.cpp
      ${RICPP_SOURCE_DIR}/ribparser/riblights.cpp
      ${RICPP_SOURCE_DIR}/ribparser/ribmisc.cpp
      ${RICPP_SOURCE_DIR}/ribparser/ribmodes.cpp
      ${RICPP_SOURCE_DIR}/ribparser/riboptions.cpp
      ${RICPP_SOURCE_DIR}/ribparser/ribparser.cpp
      ${RICPP_SOURCE_DIR}/ribparser/ribprims.cpp
      ${RICPP_SOURCE_DIR}/ribparser/ribtransforms.cpp
)

add_library ( ribparser STATIC ${ribparser_src} )

