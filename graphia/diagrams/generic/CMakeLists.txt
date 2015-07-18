

include (../../../cmake/global-set.cmake)

include_directories(${PROJECT_SOURCE_DIR})

set(diagram_generic_SRCS
    graphicsconfiguration.cpp
    graphicsitem.cpp
    toolitemmodel.cpp
    xmlbasicitem.cpp
    )

add_library(diagram_generic STATIC ${diagram_generic_SRCS})
target_link_libraries(diagram_generic toolbox)
qt5_use_modules(diagram_generic Widgets Xml)

