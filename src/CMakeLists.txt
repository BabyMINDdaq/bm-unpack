message(STATUS "src ... ")

AUX_SOURCE_DIRECTORY(.                 SOURCE_FILES)
#message(STATUS "${SOURCE_FILES}")

if (_STATIC)

  MESSAGE(STATUS "building static library\n")
  add_library(bmunpack        ${SOURCE_FILES})
else (_STATIC)

  MESSAGE(STATUS "building shered library\n")
  add_library(bmunpack SHARED ${SOURCE_FILES})
endif()


