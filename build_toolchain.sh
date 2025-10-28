#!/usr/bin/bash

TARGET="i686-elf"
PREFIX="$HOME/opt/$TARGET"
PATH="$PREFIX/bin:$PATH"
CFLAGS="-march=native -O3 -pipe"
CXXFLAGS="-march=native -O3 -pipe"
CONFFLAGS="--disable-lto --disable-bootstrap --disable-gprofng --disable-libquadmath --disable-libquadmath-support --disable-libstdcxx --disable-libada --disable-libgm2 --disable-ssp --disable-pgo-build --disable-gold"


MIRROR="https://mirrors.ustc.edu.cn/gnu"
BINUTILS="$MIRROR/binutils/binutils-2.45.tar.xz"
GDB="$MIRROR/gdb/gdb-16.3.tar.xz"
GCC="$MIRROR/gcc/gcc-15.2.0/gcc-15.2.0.tar.xz"


cd /tmp

if [ ! -e `basename $BINUTILS` ]; then
    curl -fLo `basename $BINUTILS` $BINUTILS
fi

if [ ! -e `basename $GDB` ]; then
    curl -fLo `basename $GDB` $GDB
fi

if [ ! -e `basename $GCC` ]; then
    curl -fLo `basename $GCC` $GCC
fi


tar xf `basename $BINUTILS`
mkdir build && cd build
../`basename $BINUTILS .tar.xz`/configure CFLAGS="$CFLAGS" CXXFLAGS="$CXXFLAGS" $CONFFLAGS --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j8
make -j8 install-strip
cd .. && rm -rf build `basename $BINUTILS .tar.xz`

tar xf `basename $GDB`
mkdir build && cd build
../`basename $GDB .tar.xz`/configure CFLAGS="$CFLAGS" CXXFLAGS="$CXXFLAGS" $CONFFLAGS --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j8 all-gdb
make -j8 install-strip-gdb
cd .. && rm -rf build `basename $GDB .tar.xz`

which -- $TARGET-as || echo $TARGET-as is not in the PATH
tar xf `basename $GCC`
mkdir build && cd build
../`basename $GCC .tar.xz`/configure CFLAGS="$CFLAGS" CXXFLAGS="$CXXFLAGS" $CONFFLAGS --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers --disable-hosted-libstdcxx
make -j8 all-{gcc,target-libgcc}
make -j8 install-strip-{gcc,target-libgcc}
cd .. && rm -rf build `basename $GCC .tar.xz`
