
include (../../../../cmake/global-set.cmake)

include_directories(${PROJECT_SOURCE_DIR})

set(flow_dialog_SRCS
    dataflowmodel.cpp
    dialogchoosedata.cpp
    dialogcreatedata.cpp
    dialoglistdiagram.cpp
    dialoglisttypedico.cpp
    dialogsetname.cpp
    )

add_library(flow_dialog STATIC ${flow_dialog_SRCS})
target_link_libraries(flow_dialog)
qt5_use_modules(flow_dialog Widgets Gui)

