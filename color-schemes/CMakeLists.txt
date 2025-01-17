function(install_colors)

include(KDEInstallDirs6)

install(FILES Plastik.colors DESTINATION ${KDE_INSTALL_DATADIR}/color-schemes/)

install(FILES plastik.conf DESTINATION ${KDE_INSTALL_DATADIR}/qt5ct/colors/)

install(FILES plastik.conf DESTINATION ${KDE_INSTALL_DATADIR}/qt6ct/colors/)

endfunction()

install_colors()
