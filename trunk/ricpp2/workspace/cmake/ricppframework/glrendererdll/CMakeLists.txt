set ( glrendererdll_src
      ${RICPP_SOURCE_DIR}/glrendererdll/glrendererdll.cpp
)

add_library ( glrendererdll MODULE ${glrendererdll_src})
target_link_libraries ( glrendererdll ${glrenderer_libs})
set_target_properties(glrendererdll PROPERTIES OUTPUT_NAME glrenderer.1)

