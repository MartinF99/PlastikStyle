# Plastik style

This is a fork of QPlastiqueStyle from qt5-styleplugins and a port to qt6.

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

e.g:o

    cmake -DENABLE_ALL=off -DENABLE_QT5 => builds only qt5 variant
    cmake -DENABLE_ALL=off -DENABLE_QT6 => builds only qt6 variant
    cmake => without any options both are build

### For normal build
    
    cd into cloned repo (usually PlastikStyle)
    
    mkdir build

    cd build

    cmake -DCMAKE_INSTALL_PREFIX=/usr ../

    make

    make install

now select "Plastik" for both Qt5 and Qt6


currently this does not support DCMAKE_INSTALL_PREFIX at all

### How to apply

#### Qt5: 
    - as normal via your qt5 supporting settings
    - via QT_STYLE_OVERRIDE=plastik

#### Qt6:
    - QT_STYLE_OVERRIDE=plastik  
    - via qt6ct by setting QT_QPA_PLATFORMTHEME=qt6ct and choosing plastik in qt6ct
    - apply through settings that support qt6 style

#### For both Qt5 and Qt6:
    - You can simply set QT_QPA_PLATFORMTHEME=qt5ct 
    - Setting QT_STYLE_OVERRIDE to plastik globally should work as well

## Installing
Currently installing with your package manager is only possible on opensuse
### OpenSUSE
add the zypper repository with :
        
        sudo zypper ar https://download.opensuse.org/repositories/home:/MartinF99/openSUSE_Tumbleweed/ plastikstyle

then install the packages
PlastikStyle-qt5 and PlastikStyle-qt6
