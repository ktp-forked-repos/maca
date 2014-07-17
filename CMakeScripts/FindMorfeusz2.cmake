FIND_PATH(MORFEUSZ2_INCLUDE_DIR morfeusz2.h /usr/include /usr/local/include)
MARK_AS_ADVANCED(MORFEUSZ2_INCLUDE_DIR)

FIND_LIBRARY(MORFEUSZ2_LIBRARY NAMES morfeusz2 PATH /usr/lib /usr/local/lib)
MARK_AS_ADVANCED(MORFEUSZ2_LIBRARY)

IF (MORFEUSZ2_INCLUDE_DIR AND MORFEUSZ2_LIBRARY)
   SET(MORFEUSZ2_FOUND TRUE)
ENDIF (MORFEUSZ2_INCLUDE_DIR AND MORFEUSZ2_LIBRARY)


IF (MORFEUSZ2_FOUND)
   IF (NOT Morfeusz2_FIND_QUIETLY)
      MESSAGE(STATUS "Found Morfeusz2: ${MORFEUSZ2_LIBRARY}")
   ENDIF (NOT Morfeusz2_FIND_QUIETLY)
ELSE (MORFEUSZ2_FOUND)
   IF (Morfeusz2_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Morfeusz2")
   ELSE (Morfeusz2_FIND_REQUIRED)
      IF (NOT Morfeusz2_FIND_QUIETLY)
         MESSAGE("Morfeusz2 NOT found")
      ENDIF (NOT Morfeusz2_FIND_QUIETLY)
    ENDIF (Morfeusz2_FIND_REQUIRED)
ENDIF (MORFEUSZ2_FOUND)
