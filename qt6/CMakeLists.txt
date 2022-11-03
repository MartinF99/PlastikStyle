find_package(Qt6 CONFIG REQUIRED Core Widgets)

qt_standard_project_setup()

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
#evil might only work on SUSE
execute_process(COMMAND qmake6 -query QT_INSTALL_PLUGINS
  OUTPUT_VARIABLE _Qt6_PLUGIN_INSTALL_DIR
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
# Install the executable
install(TARGETS plastikstyle6 DESTINATION ${_Qt6_PLUGIN_INSTALL_DIR}/styles/)
