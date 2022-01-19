cmake_minimum_required(VERSION 3.10)

#Set project name
project(rti1516e_lv_proj VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(lvrt "/usr/local/natinst/labview/")
message("hi there")
link_directories(${lvrt})

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
option(BUILD_LIBRARY "Build shared library" ON)
option(BUILD_TEST_APP "Build test application" ON)

# variables for source and headers
set (LIB_RTI1516ELV_SOURCES
    rti1516eLvError.cpp
    rti1516eLv.cpp
)

if(BUILD_LIBRARY)
    message("configure shared lib")
    add_library(rti1516elv SHARED ${LIB_RTI1516ELV_SOURCES})
    target_include_directories(shlib PUBLIC "${PROJECT_BINARY_DIR}/cintools")

    set_target_properties(rti1516elv PROPERTIES VERSION ${PROJECT_VERSION})
    set_target_properties(rti1516elv PROPERTIES SOVERSION 1)

    target_link_libraries(rti1516elv rti1516e fedtime1516e OpenRTI pthread lvrt)
endif()

if(BUILD_TEST_APP)
    message("configure test app for shared lib")
    # add executable
    add_executable(testapp testapp.cpp)
    target_link_libraries(testapp rti1516elv rti1516e fedtime1516e OpenRTI pthread lvrt)
    #add path to search for header files
    #target_include_directories(hlalvapp PUBLIC "${PROJECT_BINARY_DIR}")
    
endif()



