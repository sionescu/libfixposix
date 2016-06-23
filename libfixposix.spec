Name:     libfixposix
Summary:  POSIX syscall wrappers
Version:  0.4.1
Release:  0
License:  BSL-1.0
Group:    Development/Libraries/C and C++
URL:      https://github.com/sionescu/%{name}
Source:   https://github.com/sionescu/%{name}/releases/download/v%{version}/%{name}_%{version}.tar.gz

%define srcdir %{name}

BuildRequires:  check-devel
BuildRequires:  pkgconfig
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
Thin wrapper over POSIX syscalls and some replacement functionality, most notably lfp_spawn().

%package -n libfixposix3
Summary:        Shared library for libfixposix
Group:          Development/Libraries/C and C++

%description -n libfixposix3
Thin wrapper over POSIX syscalls.

%package -n libfixposix-devel
Summary:        Development files
Group:          Development/Libraries/C and C++
Requires:       glibc-devel
Requires:       libfixposix3 = %{version}-%{release}
Provides:       %{name}-static = %{version}-%{release}

%description devel
Header files, static libraries and API documentation for using libfixposix.

%prep
%setup -q -n %{srcdir}

%build
%configure --enable-static --with-pic --enable-tests
make %{?_smp_mflags}

%check
make %{?_smp_mflags} check

%install
make %{?_smp_mflags} DESTDIR=%{buildroot} install
find %{buildroot} -type f -name "*.la" -delete -print

%post -n libfixposix3 -p /sbin/ldconfig
%postun -n libfixposix3 -p /sbin/ldconfig

%files -n libfixposix3
%defattr(-,root,root)
%{_libdir}/lib*so.*

%files -n libfixposix-devel
%defattr(-,root,root)
%{_includedir}/*
%{_libdir}/lib*.so
%{_libdir}/lib*.a
%{_libdir}/pkgconfig/libfixposix.pc

%changelog
* Thu Jun 23 2016 Stelian Ionescu <sionescu@cddr.org> - 0.4.1
- Release 0.4.1
* Tue May 3 2016 Stelian Ionescu <sionescu@cddr.org> - 0.4.0
- Release 0.4.0
* Tue Mar 5 2013 Stelian Ionescu <sionescu@cddr.org> - 0.3.0-2
- Run tests
* Tue Mar 5 2013 Stelian Ionescu <sionescu@cddr.org> - 0.3.0-1
- Release 0.3.0
* Sun Jul 29 2012 Stelian Ionescu <sionescu@cddr.org> - 0.2.4-2
- Fix license: correct spelling is Boost-1 not "Boost 1.0"
* Wed Jun 13 2012 Stelian Ionescu <sionescu@cddr.org> - 0.2.4-1
- Release 0.2.4
* Wed Jun 13 2012 Stelian Ionescu <sionescu@cddr.org> - 0.2.3-1
- Release 0.2.3
* Wed May 30 2012 Stelian Ionescu <sionescu@cddr.org> - 0.2.2-1
- Release 0.2.2
* Wed Apr 18 2012 Stelian Ionescu <sionescu@cddr.org> - 0.2.1-1
- First package.
