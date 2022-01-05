cmake_minimum_required(VERSION 3.10)

#Set project name
project(rti1516e_lv_proj VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(lvrt "/usr/local/natinst/labview/")

link_directories(${lvrt})

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable

option(BUILD_TEST_APP "Build test application" OFF)

if(BUILD_TEST_APP)
    # add executable
    add_executable(testapp app.cpp)
    target_link_libraries(hlalvapp shlib)
    #add path to search for header files
    #target_include_directories(hlalvapp PUBLIC "${PROJECT_BINARY_DIR}")
    
endif()

add_library(rti1516elv SHARED rti1516eLv.cpp)

target_include_directories(shlib PUBLIC "${PROJECT_BINARY_DIR}/cintools")

set_target_properties(rti1516elv PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(rti1516elv PROPERTIES SOVERSION 1)

target_link_libraries(rti1516elv rti1516e fedtime1516e OpenRTI pthread lvrt)




