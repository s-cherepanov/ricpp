set ( ri_src
      ${RICPP_SOURCE_DIR}/ri/ri.cpp
)

set_target_properties(ri PROPERTIES OUTPUT_NAME ric)
add_library ( ri STATIC ${ri_src} )
