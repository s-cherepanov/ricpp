set ( passthrough_src
      ${RICPP_SOURCE_DIR}/ribfilter/passthrough.cpp
)

add_library ( passthroughdll MODULE ${passthrough_src})
add_library ( passthrough STATIC ${passthrough_src})
target_link_libraries ( passthroughdll ${ribfilter_libs})
target_link_libraries ( passthrough ${ribfilter_libs})
set_target_properties(passthroughdll PROPERTIES OUTPUT_NAME passthrough.1)

