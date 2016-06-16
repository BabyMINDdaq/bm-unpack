message(STATUS "src ... ")

AUX_SOURCE_DIRECTORY(.                 SOURCE_FILES)
#message(STATUS "${SOURCE_FILES}")

ADD_LIBRARY(BabyMINDUnpack      SHARED            ${SOURCE_FILES})

