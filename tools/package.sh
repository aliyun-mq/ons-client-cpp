#!/usr/bin/env bash

if [ "$#" -ne 1 ] ; then
  echo "Usage: $0 v3.0.8" >&2
  exit 1
fi

TOOLS_DIR=$(dirname "$0")
WORKSPACE=$(cd -- "$TOOLS_DIR/.." && pwd)
BUILD_DIR=$WORKSPACE/_build
if [ -d $BUILD_DIR ]; then
    rm -fr $BUILD_DIR
fi

mkdir -p "$BUILD_DIR"

cd $BUILD_DIR

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  # Linux
  cmake -DCMAKE_BUILD_TYPE=Release ..
  make -j $(nproc)
elif [[ "$OSTYPE" == "darwin"* ]]; then
  # Mac OSX
  cmake -DCMAKE_BUILD_TYPE=Release -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl ..
  make
fi

VERSION="$1"
DIST_DIR="$WORKSPACE/$VERSION/"
if [ -d "$DIST_DIR" ]; then
    rm -fr $DIST_DIR
fi

mkdir -p "$DIST_DIR/include"
mkdir -p "$DIST_DIR/lib"
mkdir -p "$DIST_DIR/examples"

cp -r $WORKSPACE/api/ons $DIST_DIR/include/
cp -r $WORKSPACE/api/rocketmq $DIST_DIR/include/
cp $WORKSPACE/examples/ons/* $DIST_DIR/examples/

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  cp $BUILD_DIR/libons.so $DIST_DIR/lib/
elif [[ "$OSTYPE" == "darwin"* ]]; then
  cp $BUILD_DIR/libons.dylib $DIST_DIR/lib/
fi

echo "Create Package"
cd "$WORKSPACE"
tar -czvf "$VERSION.tar.gz" "$VERSION"
