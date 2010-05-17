set ( ri_src
      ${RICPP_SOURCE_DIR}/ri/ri.cpp
)

add_library ( ri STATIC ${ri_src} )
set_target_properties(ri PROPERTIES OUTPUT_NAME ric)
