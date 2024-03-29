[![Build
Status](https://app.travis-ci.com/sionescu/libfixposix.svg?branch=master)](https://app.travis-ci.com/sionescu/libfixposix)
[![Coverity Scan Build
Status](https://scan.coverity.com/projects/113/badge.svg)](https://scan.coverity.com/projects/sionescu-libfixposix)

The purpose of libfixposix is to offer replacements for parts of POSIX
whose behaviour is inconsistent across *NIX flavours.

* Compilation

If you've downloaded this library through git, you'll first need to
generate the ./configure script; in order to do that you need to have
at least autoconf-2.67, automake-1.10, libtool-2.2.6b, pkg-config and check (for
the test suite: see http://check.sf.net) then run this inside the top
directory:
> autoreconf -i -f

For best results use a dedicated build directory instead of running
the configure script directly in the source tree:
> mkdir build ; cd build
> ../libfixposix/configure
> make
> make install

On Linux, you might need to run «ldconfig» as superuser after
installing, otherwise the *nix linker won't be able to find the library

* Installation through distro repositories

For a certain number of Linux distributions, there are officially
supported repositories.
You can find the instructions to configure the binary repository at
https://software.opensuse.org/download.html?project=home:sionescu&package=libfixposix.
Just click on your distribution, if present.

http://article.gmane.org/gmane.lisp.iolib.devel/422 also has a set of
older instructions about installing on Debian-based distros.
