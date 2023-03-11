# The name of our project is “R3BSOF”.CMakeLists files in this project can
# refer to the R3BROOT root source directory of the project as ${R3BROOT_SOURCE_DIR},
# the SOFIA root source directory as ${MINGO_SOURCE_DIR}(for the mingo specifics) or
# as ${CMAKE_SOURCE_DIR} and to the root binary directory of the project as
# ${MINGO_SOURCE_DIR} or ${CMAKE_BINARY_DIR}.

# Recurse into the given subdirectories.This does not actually cause
# another cmake executable to run.The same process will walk through
# the project's entire directory structure.

Set(MINGO_SOURCE_DIR ${R3BROOT_SOURCE_DIR}/mingo)
message(STATUS "${BBlue}MINGO project was FOUND${CR} ")
message("    - ${Cyan}MINGO${CR} = ${BGreen}${MINGO_SOURCE_DIR}${CR}")

# MINGO specific
add_subdirectory(data) 
add_subdirectory(mingo)

