cmake_minimum_required(VERSION 3.10)

#Set project name
project(hlalvapp VERSION 1.0)

configure_file(HeaderConfig.h.in HeaderConfig.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable

option(BUILD_SHARED_LIBS "Build shared library" ON)


add_library(shlib SHARED shlib.cpp)
set_target_properties(shlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(shlib PROPERTIES SOVERSION 1)

# add executable
add_executable(hlalvapp app.cpp)

include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(shlib           # generates the export header shared_EXPORTS.h automatically
    BASE_NAME shlib
    EXPORT_MACRO_NAME shlib_EXPORTS
    EXPORT_FILE_NAME shlib_EXPORTS.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

target_link_libraries(hlalvapp shlib)

#add path to search for header files
#target_include_directories(hlalvapp PUBLIC "${PROJECT_BINARY_DIR}")
#target_include_directories(shlib PUBLIC "${PROJECT_BINARY_DIR}")
