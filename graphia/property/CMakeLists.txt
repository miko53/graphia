
include (../../cmake/global-set.cmake)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${QTPROPERTYBROWSER_INCLUDE_DIR})

set(property_SRCS
    propertybrowser.cpp
    propertydefinition.cpp
    )

add_library(property STATIC ${property_SRCS})
target_link_libraries(property debug ${QTPROPERTYBROWSER_LIBRARY_DEBUG} optimized ${QTPROPERTYBROWSER_LIBRARY})
qt5_use_modules(property Core Widgets)

