function(install_colors)

include(KDEInstallDirs6)

install(FILES Plastik.colors DESTINATION ${KDE_INSTALL_DATADIR}/color-schemes/)

endfunction()

install_colors()
