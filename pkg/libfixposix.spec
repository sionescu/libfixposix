URL:      http://common-lisp.net/project/iolib/

Name:     libfixposix
Summary:  Thin wrapper over POSIX syscalls
Version:  0.2.4
Release:  1
License:  Boost 1.0
Group:    Development/Libraries/C and C++
Source:   http://common-lisp.net/iolib/files/%{name}/%{name}_%{version}.orig.tar.gz

%define srcdir %{name}-%{version}

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}
BuildRequires: pkg-config check-devel

%description
Thin wrapper over POSIX syscalls.

%package -n libfixposix1
Summary: Shared object for libfixposix
Group: Development/Libraries/C and C++

%description -n libfixposix1
Thin wrapper over POSIX syscalls.

%package -n libfixposix-devel
Summary: Development headers and libraries for using libfixposix
Group: Development/Libraries/C and C++
Provides: %{name}-static = %{version}-%{release}
Requires: libfixposix1 = %{version}-%{release} glibc-devel

%description devel
Header files and API documentation for using libfixposix.

%prep
%setup -q -n %{srcdir}

%build
%configure --disable-static --with-pic --disable-silent-rules
make %{?_smp_mflags}

%install
%makeinstall
rm -f $RPM_BUILD_ROOT/%{_libdir}/*.la

%post -n libfixposix1 -p /sbin/ldconfig

%postun -n libfixposix1 -p /sbin/ldconfig

%files -n libfixposix1
%defattr(-,root,root)
%{_libdir}/lib*so.*

%files -n libfixposix-devel
%defattr(-,root,root)
%{_includedir}/*
%{_libdir}/lib*.so
%{_libdir}/pkgconfig/libfixposix.pc

%changelog
* Wed Jun 13 2012 Stelian Ionescu <sionescu@cddr.org> - 0.2.4-1
- Release 0.2.4
* Wed Jun 13 2012 Stelian Ionescu <sionescu@cddr.org> - 0.2.3-1
- Release 0.2.3
* Wed May 30 2012 Stelian Ionescu <sionescu@cddr.org> - 0.2.2-1
- Release 0.2.2
* Wed Apr 18 2012 Stelian Ionescu <sionescu@cddr.org> - 0.2.1-1
- First package.
