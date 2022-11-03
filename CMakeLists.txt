cmake_minimum_required(VERSION 3.16)
project(PlastikStyle)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABlE_QT5 "build with QT5" ON)

option(ENABLE_QT6 "build with QT6" ON)

#if(ENABLE_QT5)
	add_subdirectory(qt5)
#endif()

if(ENABLE_QT6)
	add_subdirectory(qt6)
endif()
