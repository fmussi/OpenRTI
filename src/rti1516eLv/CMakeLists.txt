# Add custom symbol for OPENRTI
#Option - definitions
#Option - This is mak implementation
#add_definitions(-D_OPENRTI)
add_definitions(-D_OPENRTI)
# The current standard fdd used for testing
#set(RTI1516E_FDD_FILE ${CMAKE_SOURCE_DIR}/share/rti1516e/HLAstandardMIM.xml)
set(OPENRTI_RTI1516ELV_INCLUDE_SUBDIR "rti1516eLv" CACHE PATH "Install the RTI1516ELV include files into \${CMAKE_INSTALL_INCLUDEDIR}/\${OPENRTI_RTI1516ELV_INCLUDE_SUBDIR}.")


include_directories(${CMAKE_SOURCE_DIR}/include/rti1516e)
include_directories(${CMAKE_BINARY_DIR}/src/OpenRTI)
include_directories(${CMAKE_SOURCE_DIR}/src/OpenRTI)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# select the right location for cintools
if(WIN32)
    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        set(lvrt "C:\\Program Files\\National Instruments\\LabVIEW 2020\\cintools")
        message("Adding paths to LabVIEW 2020 cintools and runtime - Windows - x64")
    endif()

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "Win32")
        set(lvrt "C:\\Program Files (x86)\\National Instruments\\LabVIEW 2020\\cintools")
        message("Adding paths to LabVIEW 2020 cintools and runtime - Windows - x86")
    endif()
endif()
if(UNIX AND NOT APPLE)
    set(lvrt "/usr/local/natinst/labview/")
    message("Adding paths to LabVIEW runtime - NI Linux RT - x64")
endif()

# includes (-I option)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${OPENRTI_ROOTDIR}/include/HLA1516E ${lvrt} ${openrti_src_headers})
#include_directories(${OPENRTI_ROOTDIR}/include/HLA13)
#include_directories(${OPENRTI_ROOTDIR}/include/HLA1516)

# linker 
link_directories(${lvrt})
link_directories(${OPENRTI_ROOTDIR}/lib)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
option(BUILD_LIBRARY "Build shared library" ON)
#option(BUILD_TEST_APP "Build test application" OFF)

# variables for source and headers
set(RTI1516ELV_PUBLIC_HEADERS
    rti1516eLv.h
    rti1516eLv_helpers.h
    rti1516eLv_errors.h
)

set (LIB_RTI1516ELV_SOURCES
    rti1516eLv.cpp
    rti1516eLvFederate.cpp
    rti1516eLvError.cpp
    rti1516eLvHelpers.cpp
)

# list of libraries for the linker
if(WIN32)
    message("Windows")
    if(CMAKE_BUILD_TYPE MATCHES DEBUG)
        message("debug mode")
        set (LIB_RTI1516ELV_LINK
            rti1516e 
			fedtime1516e 
			OpenRTI 
			labview.lib
        )
    else()
        message("release mode")
        set (LIB_RTI1516ELV_LINK
            rti1516e 
			fedtime1516e 
			OpenRTI 
			labview.lib
        ) 
    endif()
endif()

if(UNIX)
    message("Linux")
    set (LIB_RTI1516ELV_LINK
    rti1516e 
	fedtime1516e
	OpenRTI
	pthread
	lvrt
)
endif()

if(BUILD_LIBRARY)
    message("configure shared lib")
    add_library(rti1516elv SHARED ${LIB_RTI1516ELV_SOURCES})
    if(WIN32)
        target_include_directories(rti1516elv PUBLIC ${lvrt} ${OPENRTI_ROOTDIR}/include/rti1516e)
    endif()
    if(UNIX AND NOT APPLE)
        target_include_directories(rti1516elv PUBLIC "${PROJECT_BINARY_DIR}/cintools" ${OPENRTI_ROOTDIR}/include/rti1516e)
    endif()

    if(WIN32)  
        target_link_libraries(rti1516elv ${LIB_RTI1516ELV_LINK})
        # for Visual Studio, set SAFEESH to no
        target_link_options(rti1516elv PUBLIC "$<$<C_COMPILER_ID:MSVC>:-SAFESEH:NO>")
    endif()
    if (UNIX AND NOT APPLE)
        target_link_libraries(rti1516elv ${LIB_RTI1516ELV_LINK})
    endif()
endif()

if(BUILD_TEST_APP)
    message("configure test app for shared lib")
    # add executable
    add_executable(testapp testapp.cpp)
    if(WIN32)
        target_link_libraries(testapp rti1516elv ${LIB_RTI1516ELV_LINK})
        # for Visual Studio, set SAFEESH to no
        target_link_options(rti1516elv PUBLIC "$<$<C_COMPILER_ID:MSVC>:-SAFESEH:NO>")
    endif()
    if (UNIX AND NOT APPLE)
        target_link_libraries(testapp rti1516elv ${LIB_RTI1516ELV_LINK})
    endif()
    #add path to search for header files
    #target_include_directories(hlalvapp PUBLIC "${PROJECT_BINARY_DIR}")
    
endif()
# Installation section
install(TARGETS rti1516elv
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

install(FILES ${RTI1516ELV_PUBLIC_HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${OPENRTI_RTI1516ELV_INCLUDE_SUBDIR}")




