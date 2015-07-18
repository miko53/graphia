project(graphia)
cmake_minimum_required(VERSION 2.8.8)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${graphia_SOURCE_DIR}/cmake/modules )

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(qt-solutions-qtpropertybrowser REQUIRED)

add_definitions(-DGRAPHIA_VERSION="0.0.0")

if(WIN32)
SET(GUI_TYPE WIN32)
endif()

add_subdirectory(graphia)
