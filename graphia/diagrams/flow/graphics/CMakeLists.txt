
include (../../../../cmake/global-set.cmake)

include_directories(${PROJECT_SOURCE_DIR})

set(flow_graphics_SRCS
    graphicsanchor.cpp
    graphicsarrow.cpp
    graphicscircle.cpp
    graphicsdoubleline.cpp
    graphicsinput.cpp
    graphicsoutput.cpp
    graphicsrect.cpp
    graphicsrectselection.cpp
    graphicsscene.cpp
    graphicstextitem.cpp
    graphicstextitemprivate.cpp
    graphicswriter.cpp
    )

add_library(flow_graphics STATIC ${flow_graphics_SRCS})
target_link_libraries(flow_graphics)
qt5_use_modules(flow_graphics Widgets Xml)

