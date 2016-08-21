cmake_minimum_required(VERSION 3.6)
project(BuildingEscapeProj)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Source/BuildingEscapeProj/BuildingEscapeProj.cpp
    Source/BuildingEscapeProj/BuildingEscapeProj.h
    Source/BuildingEscapeProj/BuildingEscapeProjGameMode.cpp
    Source/BuildingEscapeProj/BuildingEscapeProjGameMode.h
    Source/BuildingEscapeProj/PositionReport.cpp
    Source/BuildingEscapeProj/PositionReport.h)

add_executable(BuildingEscapeProj ${SOURCE_FILES})