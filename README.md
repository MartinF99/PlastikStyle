# Plastik style (a fork of Qt5 styleplugins' plastique style
----------------------

## Requirements

### For QT5:
Qt 5.15 and CMake 3.16

### For Qt6:
Qt 6.2

## Build instructions
option ENABLE_ALL => enables Qt5 and Qt6 support *default on
option ENABLE_QT5 => enables Qt5 support (need to have -DENABLE_ALL=off)
option ENABLE_QT6 => enables Qt6 support (need to have -DENABLE_ALL=off)

e.g:

    cmake -DENABLE_ALL=off -DENABLE_QT5 => builds only qt5 variant
    cmake -DENABLE_ALL=off -DENABLE_QT6 => builds only qt6 variant
    cmake => without any options both are build

### For normal build

    mkdir PlastikStyle/build

    cd PlastikStyle/build

    cmake ../

    make

    make install

now select "Plastik" for both Qt5 and Qt6
