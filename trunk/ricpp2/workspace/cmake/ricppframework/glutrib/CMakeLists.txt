set ( glutrib_src
      ${RICPP_SOURCE_DIR}/glutrib/glutrib.cpp
)

add_executable ( glutrib ${glutrib_src} )
target_link_libraries ( glutrib riprog glut )

