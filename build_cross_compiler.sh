#!/bin/bash
set -e

# Setup the environment
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

mkdir -p $HOME/src
cd $HOME/src

# Download binutils and gcc (adjust versions if newer ones exist)
curl -O https://ftp.gnu.org/gnu/binutils/binutils-2.40.tar.gz
curl -O https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz

tar -xf binutils-2.40.tar.gz
tar -xf gcc-13.2.0.tar.gz

# Build Binutils
mkdir -p build-binutils && cd build-binutils
../binutils-2.40/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make && make install
cd ..

# Build GCC
mkdir -p build-gcc && cd build-gcc
../gcc-13.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc all-target-libgcc
make install-gcc install-target-libgcc
