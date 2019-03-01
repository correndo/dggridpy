cmake_minimum_required(VERSION 3.8)

project(dggridpy)

add_subdirectory(../pybind11 build)

#find_package(pybind11 REQUIRED)
#message(STATUS "Found pybind11 v${pybind11_VERSION}: ${pybind11_INCLUDE_DIR}")

include_directories( ./src)


file(GLOB SOURCES
    dggridpy.cpp
    ./src/*.*

)

list(FILTER SOURCES EXCLUDE REGEX ".*RcppExports.*$")
list(FILTER SOURCES EXCLUDE REGEX ".*Rwrapper.*$")

pybind11_add_module(dggridpy ${SOURCES})
