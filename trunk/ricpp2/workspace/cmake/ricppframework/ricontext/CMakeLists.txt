set ( ricontext_src
      ${RICPP_SOURCE_DIR}/ricontext/attributes.cpp
      ${RICPP_SOURCE_DIR}/ricontext/blend.cpp
      ${RICPP_SOURCE_DIR}/ricontext/contextcreator.cpp
      ${RICPP_SOURCE_DIR}/ricontext/displaydesc.cpp
      ${RICPP_SOURCE_DIR}/ricontext/grid.cpp
      ${RICPP_SOURCE_DIR}/ricontext/inputstate.cpp
      ${RICPP_SOURCE_DIR}/ricontext/lights.cpp
      ${RICPP_SOURCE_DIR}/ricontext/modestack.cpp
      ${RICPP_SOURCE_DIR}/ricontext/motionstate.cpp
      ${RICPP_SOURCE_DIR}/ricontext/optionsbase.cpp
      ${RICPP_SOURCE_DIR}/ricontext/options.cpp
      ${RICPP_SOURCE_DIR}/ricontext/parameter.cpp
      ${RICPP_SOURCE_DIR}/ricontext/polygon.cpp
      ${RICPP_SOURCE_DIR}/ricontext/renderstate.cpp
      ${RICPP_SOURCE_DIR}/ricontext/resource.cpp
      ${RICPP_SOURCE_DIR}/ricontext/ribelements.cpp
      ${RICPP_SOURCE_DIR}/ricontext/ricontext.cpp
      ${RICPP_SOURCE_DIR}/ricontext/rimacro.cpp
      ${RICPP_SOURCE_DIR}/ricontext/subdivision.cpp
      ${RICPP_SOURCE_DIR}/ricontext/surface.cpp
      ${RICPP_SOURCE_DIR}/ricontext/transformation.cpp
      ${RICPP_SOURCE_DIR}/ricontext/triangulation.cpp
)

add_library ( ricontext ${ricontext_src} )

