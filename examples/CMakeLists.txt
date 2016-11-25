message(STATUS "examples ... ")

ADD_EXECUTABLE(dump         babyMindDataDump.cpp)
TARGET_LINK_LIBRARIES(dump  BabyMINDUnpack)

if    (ROOT_FOUND)
  ADD_EXECUTABLE(unpack         babyMindUnpack.cpp)
  TARGET_LINK_LIBRARIES(unpack  BabyMINDUnpack ${ROOT_LIBRARIES})

  ADD_EXECUTABLE(ufe-monitor         ufe_monitor.cpp)
  TARGET_LINK_LIBRARIES(ufe-monitor  BabyMINDUnpack ${ROOT_LIBRARIES})

endif (ROOT_FOUND)
