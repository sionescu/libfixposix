URL:      http://common-lisp.net/project/iolib/

Name:     libfixposix
Summary:  Thin wrapper over POSIX syscalls
Version:  0.3.0
Release:  1
License:  Boost-1
Group:    Development/Libraries/C and C++
Source:   http://common-lisp.net/iolib/files/%{name}/%{name}_%{version}.orig.tar.gz

%define srcdir %{name}-%{version}

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}
BuildRequires: pkg-config check-devel

%description
Thin wrapper over POSIX syscalls.

%package -n libfixposix2
Summary: Shared object for libfixposix
Group: Development/Libraries/C and C++

%description -n libfixposix2
Thin wrapper over POSIX syscalls.

%package -n libfixposix-devel
Summary: Development headers and libraries for using libfixposix
Group: Development/Libraries/C and C++
Provides: %{name}-static = %{version}-%{release}
Requires: libfixposix2 = %{version}-%{release} glibc-devel

%description devel
Header files and API documentation for using libfixposix.

%prep
%setup -q -n %{srcdir}

%build
%configure --enable-static --with-pic --enable-tests
make %{?_smp_mflags}

%install
%makeinstall
rm -f $RPM_BUILD_ROOT/%{_libdir}/*.la

%post -n libfixposix2 -p /sbin/ldconfig

%postun -n libfixposix2 -p /sbin/ldconfig

%files -n libfixposix2
%defattr(-,root,root)
%{_libdir}/lib*so.*

%files -n libfixposix-devel
%defattr(-,root,root)
%{_includedir}/*
%{_libdir}/lib*.so
%{_libdir}/lib*.a
%{_libdir}/pkgconfig/libfixposix.pc

%changelog
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
