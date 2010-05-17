set ( riprog_src
      ${RICPP_SOURCE_DIR}/ri/riprog.cpp
)

add_library ( riprogdll SHARED ${riprog_src})
add_library ( riprog STATIC ${riprog_src})
target_link_libraries ( riprogdll ri ${ricppbridge_libs} z dl)
target_link_libraries ( riprog ri ${ricppbridge_libs} z dl)
set_target_properties(riprogdll PROPERTIES OUTPUT_NAME riprog.1)

