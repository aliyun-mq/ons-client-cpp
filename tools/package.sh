#!/usr/bin/env bash
TOOLS_DIR=$(dirname "$0")
WORKSPACE=$(cd -- "$TOOLS_DIR/.." && pwd)
BUILD_DIR=$WORKSPACE/_build
if [ -d $BUILD_DIR ]; then
    rm -fr $BUILD_DIR
fi

mkdir -p "$BUILD_DIR"

cd $BUILD_DIR
cmake -DgRPC_SSL_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package -DCMAKE_BUILD_TYPE=Release ..
make -j $(nproc)
VERSION="v3.0.7"
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
