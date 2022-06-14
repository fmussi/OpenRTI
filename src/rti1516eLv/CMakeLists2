cmake_minimum_required(VERSION 3.10)

#Set project name
project(rti1516e_lv_proj VERSION 1.0)
# Commented below. Specify architecture with the generator from command line, for example: cmake -G "Visual Studio 16 2019" -A Win32 ..
# if(WIN32)
#     set(CMAKE_VS_PLATFORM_NAME Win32)
# endif()
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
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
link_directories(${lvrt})

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

if(BUILD_LIBRARY)
    message("configure shared lib")
    add_library(rti1516elv SHARED ${LIB_RTI1516ELV_SOURCES})
    if(WIN32)
        target_include_directories(rti1516elv PUBLIC ${lvrt})
    endif()
    if(UNIX AND NOT APPLE)
        target_include_directories(rti1516elv PUBLIC "${PROJECT_BINARY_DIR}/cintools")
    endif()
    set_target_properties(rti1516elv PROPERTIES VERSION ${PROJECT_VERSION})
    set_target_properties(rti1516elv PROPERTIES SOVERSION 1)
    if(WIN32)  
        target_link_libraries(rti1516elv rti1516e fedtime1516e OpenRTI labview.lib)
        # for Visual Studio, set SAFEESH to no
        target_link_options(rti1516elv PUBLIC "$<$<C_COMPILER_ID:MSVC>:-SAFESEH:NO>")
    endif()
    if (UNIX AND NOT APPLE)
        target_link_libraries(rti1516elv rti1516e fedtime1516e OpenRTI pthread lvrt)
    endif()
endif()

if(BUILD_TEST_APP)
    message("configure test app for shared lib")
    # add executable
    add_executable(testapp testapp.cpp)
    if(WIN32)
        target_link_libraries(testapp rti1516elv rti1516e fedtime1516e OpenRTI labview.lib)
        # for Visual Studio, set SAFEESH to no
        target_link_options(rti1516elv PUBLIC "$<$<C_COMPILER_ID:MSVC>:-SAFESEH:NO>")
    endif()
    if (UNIX AND NOT APPLE)
        target_link_libraries(testapp rti1516elv rti1516e fedtime1516e OpenRTI pthread lvrt)
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




