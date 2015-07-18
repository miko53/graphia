
include(FindPackageHandleStandardArgs)

if(WIN32)
  FIND_LIBRARY(QTPROPERTYBROWSER_LIBRARY QtSolutions_PropertyBrowser-head
  E:/Perso/lib_ext/qt-solutions/qtpropertybrowser/lib)

  FIND_LIBRARY(QTPROPERTYBROWSER_LIBRARY_DEBUG QtSolutions_PropertyBrowser-headd
  E:/Perso/lib_ext/qt-solutions/qtpropertybrowser/lib)

  set(QTPROPERTYBROWSER_INCLUDE_DIR E:/Perso/lib_ext/qt-solutions/qtpropertybrowser/src)
  
  find_package_handle_standard_args(qt-solutions-qtpropertybrowser DEFAULT_MSG QTPROPERTYBROWSER_LIBRARY QTPROPERTYBROWSER_LIBRARY_DEBUG QTPROPERTYBROWSER_INCLUDE_DIR)
  mark_as_advanced(QTPROPERTYBROWSER_INCLUDE_DIR QTPROPERTYBROWSER_LIBRARY QTPROPERTYBROWSER_LIBRARY_DEBUG)
endif()

if(UNIX)
  FIND_LIBRARY(QTPROPERTYBROWSER_LIBRARY QtSolutions_PropertyBrowser-head
  /media/extras/mickael/programmation/lib_ext/qt-solutions/qtpropertybrowser/lib)
  set(QTPROPERTYBROWSER_LIBRARY_DEBUG ${QTPROPERTYBROWSER_LIBRARY})
  
  set(QTPROPERTYBROWSER_INCLUDE_DIR /media/extras/mickael/programmation/lib_ext/qt-solutions/qtpropertybrowser/src)
  
  find_package_handle_standard_args(qt-solutions-qtpropertybrowser DEFAULT_MSG QTPROPERTYBROWSER_LIBRARY QTPROPERTYBROWSER_INCLUDE_DIR)
  mark_as_advanced(QTPROPERTYBROWSER_INCLUDE_DIR QTPROPERTYBROWSER_LIBRARY )
endif()
