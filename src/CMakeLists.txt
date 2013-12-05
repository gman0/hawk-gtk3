cmake_minimum_required(VERSION 2.8)
project(hawk-gtk3-test)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic -g -Og")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS system filesystem)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories("include" ${Boost_INCLUDE_DIRS} "/usr/local/include/hawk" ${GTKMM_INCLUDE_DIRS})

file(GLOB_RECURSE srcs "./*.cpp")
add_executable(hawk-gtk3-test ${srcs})
target_link_libraries(hawk-gtk3-test ${Boost_LIBRARIES} hawk magic ${GTKMM_LIBRARIES})

