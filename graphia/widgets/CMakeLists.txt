
include (../../cmake/global-set.cmake)

set(widgets_SRCS
    buttongroup.cpp
    toolbutton.cpp
    )
    
add_library(widgets STATIC ${widgets_SRCS})
qt5_use_modules(widgets Widgets)

