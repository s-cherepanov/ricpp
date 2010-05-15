set ( ricpp_src
      ${RICPP_SOURCE_DIR}/ricpp/errorhandlers.cpp
      ${RICPP_SOURCE_DIR}/ricpp/filters.cpp
      ${RICPP_SOURCE_DIR}/ricpp/paramclasses.cpp
      ${RICPP_SOURCE_DIR}/ricpp/requests.cpp
      ${RICPP_SOURCE_DIR}/ricpp/ricpperror.cpp
      ${RICPP_SOURCE_DIR}/ricpp/riroot.cpp
      ${RICPP_SOURCE_DIR}/ricpp/subdivfunc.cpp
      ${RICPP_SOURCE_DIR}/ricpp/types.cpp
      ${RICPP_SOURCE_DIR}/ricpp/varsubst.cpp
)

add_library ( ricpp ${ricpp_src} )

