message(STATUS "examples ... ")

ADD_EXECUTABLE(dump         babyMindDataDump.cpp)
TARGET_LINK_LIBRARIES(dump  BabyMINDUnpack)

if    (ROOT_FOUND)
  ADD_EXECUTABLE(unpack         babyMindUnpack.cpp)
  TARGET_LINK_LIBRARIES(unpack  BabyMINDUnpack ${ROOT_LIBRARIES})

  ADD_EXECUTABLE(analysis         babyMindAnalysis.cpp)
  TARGET_LINK_LIBRARIES(analysis  BabyMINDUnpack ${ROOT_LIBRARIES})

  ADD_EXECUTABLE(analysis2         babyMindAnalysis2.cpp)
  TARGET_LINK_LIBRARIES(analysis2  BabyMINDUnpack ${ROOT_LIBRARIES})

  ADD_EXECUTABLE(newMulti         newMulti.cpp)
  TARGET_LINK_LIBRARIES(newMulti  BabyMINDUnpack ${ROOT_LIBRARIES})

endif (ROOT_FOUND)


