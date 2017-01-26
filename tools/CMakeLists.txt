message(STATUS "examples ... ")

ADD_EXECUTABLE(ufe-dump         ufe_dump.cpp)
TARGET_LINK_LIBRARIES(ufe-dump  bmunpack)

if    (ROOT_FOUND)
  ADD_EXECUTABLE(bm-unpack         bm_unpack.cpp)
  TARGET_LINK_LIBRARIES(bm-unpack  bmunpack ${ROOT_LIBRARIES})

  ADD_EXECUTABLE(ufe-monitor         ufe_monitor.cpp)
  TARGET_LINK_LIBRARIES(ufe-monitor  bmunpack ${ROOT_LIBRARIES})

endif (ROOT_FOUND)
