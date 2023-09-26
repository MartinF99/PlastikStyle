#
# spec file for package plastikstyle
#
# Copyright (c) 2023 SUSE LLC
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via https://bugs.opensuse.org/
#


Name:           PlastikStyle
Version:        1.0.3
Release:        0
Summary:        QStyle imitating KDE 3's Plastik
License:        LGPL-2.1-only
URL:            https://github.com/MartinF99/PlastikStyle
Source0:        https://github.com/MartinF99/PlastikStyle/archive/refs/tags/%{version}.tar.gz
BuildRequires: 	cmake >= 3.16
BuildRequires: 	libqt5-qtbase-common-devel >= 5.15
BuildRequires: 	qt6-base-common-devel >= 6.2
BuildRequires:	cmake(Qt5Widgets)
BuildRequires: 	cmake(Qt6Widgets)
%description
Plastikstyle is a qstyle for both qt5 and qt6 imitating the look and feel from KDE3's plastikstyle.
It is forked from qtstyleplugins

%package        qt5
Summary:        QStyle imitating KDE 3's Plastik (qt5)

%description    qt5
The %{name}-qt5 package contains the files for the Qt5 qstyle of PlastikStyle.

%package	qt6
Summary:	QStyle imitating KDE 3's Plastik (qt6)

%description 	qt6
The %{name}-qt6 package contains the files for the qt6 variant of PlastikStyle

%prep
%setup -q

%build
%cmake -DCMAKE_INSTALL_PREFIX="%_prefix"
%cmake_build %{?_smp_mflags}

%install
%cmake_install
find %{buildroot} -type f -name "*.la" -delete -print

%files qt5
%dir %_libdir/qt5/plugins/styles
%_libdir/qt5/plugins/styles/libplastikstyle5.so

%files qt6
%dir %_libdir/qt6/plugins/styles
%_libdir/qt6/plugins/styles/libplastikstyle6.so

%changelog

