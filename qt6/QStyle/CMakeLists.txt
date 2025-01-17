function(build_qt6)

set(KF6_MIN_VERSION 6.10.0)


find_package(Qt6 CONFIG REQUIRED Core Widgets)

include(KDEInstallDirs6)

set(plastikstyle6_SRC
  plastikstyle.cpp
  qhexstring.h
  qstylecache.h
  qstylehelper.cpp
  plugin.cpp
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
