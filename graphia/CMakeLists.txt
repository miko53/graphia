
set(GRAPHIA_SOURCE_DIR ${PROJECT_SOURCE_DIR}/graphia)

include_directories(${GRAPHIA_SOURCE_DIR})
include_directories(${GRAPHIA_SOURCE_DIR}/include)

if(WIN32)
SET(GUI_TYPE WIN32)
endif()

add_subdirectory(widgets)
add_subdirectory(property)
add_subdirectory(toolbox)
add_subdirectory(diagrams)
add_subdirectory(core)
