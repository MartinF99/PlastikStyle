function(build_qt6)

set(KF6_MIN_VERSION 6.10.0)


find_package(Qt6 CONFIG REQUIRED Core Widgets)

include(KDEInstallDirs6)

set(widgets_SRC controls/frame.cpp
    controls/button.cpp
    controls/dockwidget.cpp
    controls/frame.cpp
    controls/itemview.cpp
    controls/lineedit.cpp
    controls/mainwindow.cpp
    controls/tabbar.cpp
    controls/toolbar.cpp
    controls/widget.cpp
    controls/window.cpp
    controls/header.cpp
    controls/menu.cpp
    )

# Seperate the actual source files for the Style into its own thing
set(style_SRC
    plastikstyle.h
    plastikstyle.cpp
    plastikstylehelper.h
    plastikstylehelper.cpp
    ${widgets_SRC}
)


set(plastikstyle6_SRC
  plastikstyle.cpp
  qhexstring.h
  qstylecache.h
  qstylehelper.cpp
  plugin.cpp
  ${style_SRC}
)

# Tell CMake to create the helloworld executable
add_library(plastikstyle6 MODULE ${plastikstyle6_SRC})

# Use the Widgets module from Qt 6.
target_link_libraries(plastikstyle6 Qt6::Widgets)

set(_Qt6_STYLE_PLUGIN_INSTALL_DIR ${KDE_INSTALL_QTPLUGINDIR}/styles/)

install(TARGETS plastikstyle6 DESTINATION ${_Qt6_STYLE_PLUGIN_INSTALL_DIR})

unset(QUERY_EXECUTABLE CACHE)

endfunction()

build_qt6()
