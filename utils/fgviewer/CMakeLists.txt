set(FGVIEWER_SOURCES
  fgviewer.cxx
  Drawable.cxx
  Renderer.cxx
  SlaveCamera.cxx
  Viewer.cxx
)

if(RTI_FOUND)
  set(FGVIEWER_RTI_SOURCES
    HLACamera.cxx
    HLACameraClass.cxx
    HLACameraManipulator.cxx
    HLADrawable.cxx
    HLADrawableClass.cxx
    HLAEyeTracker.cxx
    HLAEyeTrackerClass.cxx
    HLAMPAircraft.cxx
    HLAMPAircraftClass.cxx
    HLAOrthographicCamera.cxx
    HLAOrthographicCameraClass.cxx
    HLAPerspectiveCamera.cxx
    HLAPerspectiveCameraClass.cxx
    HLAPerspectiveViewer.cxx
    HLAPerspectiveViewerClass.cxx
    HLARenderer.cxx
    HLARendererClass.cxx
    HLASceneObject.cxx
    HLASceneObjectClass.cxx
    HLAView.cxx
    HLAViewClass.cxx
    HLAViewer.cxx
    HLAViewerClass.cxx
    HLAViewerFederate.cxx
    HLAWindowDrawable.cxx
    HLAWindowDrawableClass.cxx
  )
  set(FGVIEWER_RTI_LIBRARIES ${RTI_LIBRARIES})
else()
  set(FGVIEWER_RTI_LIBRARIES "")
  set(FGVIEWER_RTI_SOURCES "")
endif()

add_executable(fgviewer ${FGVIEWER_SOURCES} ${FGVIEWER_RTI_SOURCES})
if(X11_FOUND)
    target_link_libraries(fgviewer ${X11_LIBRARIES})
endif()

target_link_libraries(fgviewer
	${SIMGEAR_LIBRARIES}
	${OPENSCENEGRAPH_LIBRARIES}
	${OPENGL_LIBRARIES}
	${FGVIEWER_RTI_LIBRARIES}
        ${SIMGEAR_CORE_LIBRARY_DEPENDENCIES}
)
install(TARGETS fgviewer RUNTIME DESTINATION bin)
