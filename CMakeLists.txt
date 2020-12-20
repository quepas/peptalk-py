cmake_minimum_required( VERSION 3.0 )

project( peptalk VERSION 1.0 )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_BUILD_TYPE Release )

add_subdirectory( third-party/peptalk-core )
add_subdirectory( third-party/pybind11 )

pybind11_add_module( peptalk_core_impl main.cpp )
target_link_libraries( peptalk_core_impl PRIVATE peptalk_core_static )
