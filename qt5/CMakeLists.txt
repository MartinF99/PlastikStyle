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
get_target_property(REAL_QMAKE_EXECUTABLE ${Qt5Core_QMAKE_EXECUTABLE} IMPORTED_LOCATION)
execute_process(COMMAND "${REAL_QMAKE_EXECUTABLE}" -query QT_INSTALL_PLUGINS
  OUTPUT_VARIABLE _Qt5_PLUGIN_INSTALL_DIR
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
# Install the executable
install(TARGETS plastikstyle5 DESTINATION ${_Qt5_PLUGIN_INSTALL_DIR}/styles/)
