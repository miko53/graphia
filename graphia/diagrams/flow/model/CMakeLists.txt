
include (../../../../cmake/global-set.cmake)

include_directories(${PROJECT_SOURCE_DIR})

set(flow_model_SRCS
    basictypes.cpp
    class.cpp
    data.cpp
    datafactory.cpp
    flow.cpp
    flowfactory.cpp
    input.cpp
    inputfactory.cpp
    item.cpp
    itemcontainer.cpp
    itemfactory.cpp
    output.cpp
    outputfactory.cpp
    processcontext.cpp
    processcontextfactory.cpp
    processimplementation.cpp
    processimplfactory.cpp
    processinstance.cpp
    processinstancefactory.cpp
    store.cpp
    storefactory.cpp
    terminator.cpp
    terminatorfactory.cpp
    typefactory.cpp
    )

add_library(flow_model STATIC ${flow_model_SRCS})
target_link_libraries(flow_model)
qt5_use_modules(flow_model Widgets)

