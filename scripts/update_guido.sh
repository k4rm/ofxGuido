#! /bin/bash
#
# builds GUDIO Engine for your platform and updates headers,
# this should work for macOS and Linux however recommends building
# with Visual Studio on Windows
# 
# requires cmake
#

SRC=guidolib
DEST=../lib/guidolib
VERSION=dev
PLATFORM=""

###

cd $(dirname $0)
WD=$(pwd -P)
PLATFORM=$(./ostype.sh)

# get latest source 
git clone https://github.com/grame-cncm/guidolib.git
cd $SRC
git checkout $VERSION

# build
mkdir -p build/platform
cd build/platform
cmake ../ -DSTATICLIB=yes
make
cd "$WD"

# copy static lib
mkdir -p $DEST/lib/$PLATFORM
cp -v $SRC/build/lib/libSGUIDOEngine.a $DEST/lib/$PLATFORM/

# copy headers
mkdir -p $DEST/include
cp -v $SRC/src/engine/include/* $DEST/include

# cleanup
rm -rf $SRC
