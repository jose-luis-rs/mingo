# Create a library called "libMingoData" which includes the source files given in
# the array. The extension is already found. Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES} )

set(INCLUDE_DIRECTORIES
${R3BROOT_SOURCE_DIR}/r3bdata
${MINGO_SOURCE_DIR}/data
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} )

link_directories( ${LINK_DIRECTORIES})

set(SRCS
MingoPoint.cxx
)


# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS} ${R3BROOT_SOURCE_DIR}/r3bdata/R3BDetectorList.h )

set(LINKDEF  DataLinkDef.h)
set(LIBRARY_NAME MingoData)
set(DEPENDENCIES
    Base FairTools R3BBase R3BData Core m)

GENERATE_LIBRARY()
