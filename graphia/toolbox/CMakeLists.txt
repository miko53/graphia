
include (../../cmake/global-set.cmake)

include_directories(${PROJECT_SOURCE_DIR})

set(toolbox_SRCS
    toolbox.cpp
    toolgroup.cpp
    toolitem.cpp
    )

add_library(toolbox STATIC ${toolbox_SRCS})
target_link_libraries(toolbox widgets)
qt5_use_modules(toolbox Widgets)

