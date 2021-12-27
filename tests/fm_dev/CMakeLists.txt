# The current standard fdd used for testing
#set(RTI1516E_FDD_FILE ${CMAKE_SOURCE_DIR}/share/rti1516e/HLAstandardMIM.xml)

include_directories(${CMAKE_SOURCE_DIR}/include/rti1516e)
include_directories(${CMAKE_BINARY_DIR}/src/OpenRTI)
include_directories(${CMAKE_SOURCE_DIR}/src/OpenRTI)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# if(NOT OPENRTI_BUILD_SHARED)
#   add_definitions(-DSTATIC_RTI)
#   add_definitions(-DSTATIC_FEDTIME)
# endif()

# Just for proper recursion
add_subdirectory(chat-cpp-fm)