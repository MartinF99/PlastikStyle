
function(build_qt5)

include(KDEInstallDirs5)

find_package(Qt5 CONFIG REQUIRED Core Widgets)


set(plastikstyle5_SRC
  plastikstyle.cpp
  qhexstring.h
  qstylecache.h
  qstylehelper.cpp
  plugin.cpp
)

# Tell CMake to create the helloworld executable
add_library(plastikstyle5 MODULE ${plastikstyle5_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(plastikstyle5 Qt5::Widgets)


# Install the executable
install(TARGETS plastikstyle5 DESTINATION  ${KDE_INSTALL_QTPLUGINDIR}/styles/)

unset(QUERY_EXECUTABLE CACHE)

endfunction()

build_qt5()
