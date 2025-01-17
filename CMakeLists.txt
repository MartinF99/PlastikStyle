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

#extra cmake modules, to not directly rely on qmake

set(KF5_MIN_VERSION "5.102.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

option(ENABLE_ALL "build with QT5 and QT6" ON)

option(ENABlE_QT5 "build with QT5" OFF)

option(ENABLE_QT6 "build with QT6" OFF)

option(ENABLE_QSTYLE "build Qstyles" ON)

if(ENABLE_QT5 OR ENABLE_ALL)
		add_subdirectory(qt5)
endif()

if(ENABLE_QT6 OR ENABLE_ALL)
	add_subdirectory(qt6)
endif()
