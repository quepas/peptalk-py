cmake_minimum_required( VERSION 3.0 )

project( peptalk VERSION 1.0 )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_BUILD_TYPE Release )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib )

add_subdirectory( third-party/peptalk-core )
add_subdirectory( third-party/pybind11 )

pybind11_add_module( peptalk main.cpp )
target_link_libraries( peptalk PRIVATE peptalk_core )
