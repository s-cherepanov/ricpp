set ( ribwriterdll_src
      ${RICPP_SOURCE_DIR}/ribwriterdll/ribwriterdll.cpp
)

add_library ( ribwriterdll MODULE ${ribwriterdll_src})
target_link_libraries ( ribwriterdll ${ribwriter_libs})
set_target_properties(ribwriterdll PROPERTIES OUTPUT_NAME ribwriter.1)

