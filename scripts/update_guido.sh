#! /bin/bash
#
# builds GUDIO Engine for your platform and updates headers,
# this should work for macOS and Linux however recommends building
# with Visual Studio on Windows
# 
# requires cmake
#

WD=$(dirname $0)

SRC=guidolib
DEST=../lib/guidolib
VERSION=dev
PLATFORM=""

###

cd "$WD"
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
cp $SRC/build/lib/libSGUIDOEngine.a $DEST/lib/$PLATFORM/

# copy headers
mkdir -p $DEST/include
cp $SRC/src/engine/include/* $DEST/include

# cleanup
rm -rf $SRC
