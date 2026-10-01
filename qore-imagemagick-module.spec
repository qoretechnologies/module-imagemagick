# Copyright (C) 2026 Qore Technologies, s.r.o.
# SPDX-License-Identifier: MIT
# Use the pinned source epoch for RPM headers and installed file timestamps.
%global source_date_epoch_from_changelog 1
%global use_source_date_epoch_as_buildtime 1
%if v"%{rpmversion}" >= v"4.20"
%global build_mtime_policy clamp_to_source_date_epoch
%else
%global clamp_mtime_to_source_date_epoch 1
%endif
%bcond_without tests
%bcond_without docs
Name: qore-imagemagick-module
Version: 1.0.0
Release: 1%{?dist}
Summary: Image processing and conversion providers for Qore
License: MIT
URL: https://github.com/qoretechnologies/module-imagemagick
Source0: %{name}-%{version}.tar.xz
%global _find_debuginfo_dwz_opts %{nil}
BuildRequires: cmake >= 3.21
BuildRequires: make
BuildRequires: gcc-c++
BuildRequires: pkgconfig(MagickWand) >= 7.0
BuildRequires: urw-base35-fonts
Requires: urw-base35-fonts
%if 0%{?suse_version}
BuildRequires: dejavu-fonts
Requires: dejavu-fonts
%else
BuildRequires: dejavu-sans-fonts
Requires: dejavu-sans-fonts
%endif
%if %{with tests}
BuildRequires: python3
BuildRequires: diffutils
BuildRequires: qore-misc-tools >= 3.0.0~
%endif
BuildRequires: qore-devel >= 3.0.0~
BuildRequires: qore-rpm-macros >= 3.0.0~
%if %{with docs}
BuildRequires: doxygen
%if 0%{?suse_version}
BuildRequires: util-linux
%else
BuildRequires: util-linux-core
%endif
%endif
%{?qore_enable_aot_post}

%description
ImageMagick 7 bindings for resizing, image effects, drawing, profiles and
metadata. Includes the ImageMagickDataProvider module, its resources and
translations, and a command-line tool.

%if %{with docs}
%package doc
Summary: ImageMagick module reference documentation
BuildArch: noarch
%description doc
API reference and examples for Qore's ImageMagick module.
%endif

%prep
%autosetup
%build
%{?set_build_flags}
. %{_rpmconfigdir}/qore/module-env.sh
qore_set_source_prefix_maps "%{qore_debug_source_dir}"
cmake -S . -B build -G 'Unix Makefiles' \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE=-DNDEBUG \
  -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_INSTALL_LIBDIR=%{_lib} \
  -DCMAKE_SKIP_RPATH=ON -DCMAKE_IGNORE_PREFIX_PATH=/usr/local \
  -DQore_DIR=%{_libdir}/cmake/Qore -DQORE_EXECUTABLE=/usr/bin/qore \
  -DQORE_QPP_EXECUTABLE=/usr/bin/qpp -DQORE_QCC_EXECUTABLE=/usr/bin/qcc \
  -DQORE_BUILD_AOT_MODULES=ON -DQORE_AOT_LINK_SOURCE_MODULES=OFF \
  -DQORE_QM_METADATA_ENV:STRING="QORE_MODULE_DIR=$QORE_MODULE_DIR:$PWD/qlib;QORE_MODULE_DIR_ONLY=1;QORE_INCLUDE_DIR=;LD_LIBRARY_PATH=" \
  -DCMAKE_DISABLE_FIND_PACKAGE_Doxygen=%{!?with_docs:ON}%{?with_docs:OFF}
cmake --build build -- %{?_smp_mflags}
%if %{with docs}
for config in build/Doxyfile build/doxygen/Doxyfile.*; do
    printf "\nWARN_AS_ERROR = FAIL_ON_WARNINGS\n" >> "$config"
done
cmake --build build --target docs -- %{?_smp_mflags}
%endif
%install
DESTDIR=%{buildroot} cmake --install build
sed -i '1s|.*|#!/usr/bin/qore|' %{buildroot}%{_bindir}/qimage
install -Dm644 debian/qimage.1 %{buildroot}%{_mandir}/man1/qimage.1
%qore_install_aot_sources qlib
find %{buildroot}%{_libdir}/qore-modules -type f -name '*.qmod' -exec chmod 755 {} +
%if %{with docs}
install -d %{buildroot}%{_docdir}/%{name}-doc
cp -a build/docs %{buildroot}%{_docdir}/%{name}-doc/
hardlink -t -O %{buildroot}%{_docdir}/%{name}-doc
%endif
%check
%if %{with tests}
. %{_rpmconfigdir}/qore/module-env.sh
python3 -B -W error test/test_uninstall.py -v
%if %{with docs}
python3 -B -W error test/test_docs.py build -v
%endif
for test in test/*.qtest; do
  timeout 180 /usr/bin/qore -b --enable-debug \
    -l "$PWD/build/imagemagick-api-$(/usr/bin/qore --latest-module-api).qmod" \
    -l "$PWD/build/qlib-qmod/ImageMagickDataProvider/ImageMagickDataProvider.qmod" "$test" -v
done
QORE_QIMAGE_BINARY="$PWD/bin/qimage" debian/tests/cli
qore-data-provider-i18n --no-color --check-source-tree --require-standard-locales \
  --require-complete-locales --output "$PWD/qlib"
%endif
%files
%license LICENSE
%doc README.md
%{_bindir}/qimage
%{_mandir}/man1/qimage.1*
%{_libdir}/qore-modules/imagemagick-api-*.qmod
%{_libdir}/qore-modules/ImageMagickDataProvider/
%{_datadir}/qore-modules/ImageMagickDataProvider/
%dir %{_datadir}/qore/metadata/imagemagick
%{_datadir}/qore/metadata/imagemagick/*.meta.json
%{_datadir}/qore/i18n/
%if %{with docs}
%files doc
%license LICENSE
%doc %{_docdir}/%{name}-doc/
%endif
%changelog
* Thu Oct 01 2026 David Nichols <david@qore.org> - 1.0.0-1
- Package native and AOT modules, resources, documentation and offline tests.
