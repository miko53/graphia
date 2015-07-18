
include (../../cmake/global-set.cmake)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${QTPROPERTYBROWSER_INCLUDE_DIR})

set( CORE_SRC
    graphia.cpp 
    application.cpp 
    configuration.cpp 
    dialognewdiagram.cpp 
    document.cpp 
    graphicsview.cpp 
    mainwindow.cpp 
    mainwindow_p.cpp 
    mainwidget.cpp 
    version.cpp
    )

qt5_add_resources(RESSOURCES ../resources/resources.qrc)

add_executable(
    ${PROJECT_NAME} ${GUI_TYPE}
    ${RESSOURCES}
    ${CORE_SRC}
    )

target_link_libraries(${PROJECT_NAME} widgets property toolbox diagram_generic flow_model flow_storage flow_graphics flow_dialog)
qt5_use_modules(${PROJECT_NAME} Widgets PrintSupport)


