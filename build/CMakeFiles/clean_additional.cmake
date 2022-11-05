# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "qt5/CMakeFiles/plastikstyle5_autogen.dir/AutogenUsed.txt"
  "qt5/CMakeFiles/plastikstyle5_autogen.dir/ParseCache.txt"
  "qt5/plastikstyle5_autogen"
  "qt6/CMakeFiles/plastikstyle6_autogen.dir/AutogenUsed.txt"
  "qt6/CMakeFiles/plastikstyle6_autogen.dir/ParseCache.txt"
  "qt6/plastikstyle6_autogen"
  )
endif()
