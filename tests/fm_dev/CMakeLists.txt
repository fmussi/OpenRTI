# The current standard fdd used for testing
#set(RTI1516E_FDD_FILE ${CMAKE_SOURCE_DIR}/share/rti1516e/HLAstandardMIM.xml)
set(OPENRTI_RTI1516ELV_INCLUDE_SUBDIR "rti1516eLv" CACHE PATH "Install the RTI1516ELV include files into \${CMAKE_INSTALL_INCLUDEDIR}/\${OPENRTI_RTI1516ELV_INCLUDE_SUBDIR}.")


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
#add_subdirectory(multithread)
add_subdirectory(shlib)
# add_subdirectory(rti1516eLv)
# Temp subdir for testing
#add_subdirectory(testWinCmake)
#add_subdirectory(LabView_EventTest)
#defint src/OpenRTI headers to be exposed
# The directory containing the OPENRTI headers needed by 1516elv
# set(OPENRTI_SRC_HEADER_PATH ${CMAKE_SOURCE_DIR}/src/OpenRTI)
# set(OPENRTI_EXPORTED_HEADERS
#         ${OPENRTI_SRC_HEADER_PATH}/RTI/Enums.h

# )