find_package(Qt6 CONFIG REQUIRED Core Widgets)


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

#get qmake6
get_target_property(REAL_QMAKE_EXECUTABLE Qt6::qmake
                        IMPORTED_LOCATION)
#save output to _Qt6_PLUGIN_INSTALL_DIR
execute_process(COMMAND "${REAL_QMAKE_EXECUTABLE}" -query QT_INSTALL_PLUGINS
                    OUTPUT_VARIABLE _Qt6_PLUGIN_INSTALL_DIR
                    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)


set(_Qt6_STYLE_PLUGIN_INSTALL_DIR ${_Qt6_PLUGIN_INSTALL_DIR}/styles)

install(TARGETS plastikstyle6 DESTINATION ${_Qt6_STYLE_PLUGIN_INSTALL_DIR})
