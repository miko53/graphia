
include (../../../../cmake/global-set.cmake)

include_directories(${PROJECT_SOURCE_DIR})

set(flow_storage_SRCS
    decodeflowcontext.cpp
    decodeflowdiagram.cpp
    decodeflowmodel.cpp
    decodegraphics.cpp
    documentreader.cpp
    documentwriter.cpp
    )

add_library(flow_storage STATIC ${flow_storage_SRCS})
target_link_libraries(flow_storage diagram_generic)
qt5_use_modules(flow_storage Core Xml Widgets)

