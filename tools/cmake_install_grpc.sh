#!/usr/bin/env bash
TOOLS_DIR=$(dirname "$0")
WORKSPACE=$(cd -- "$TOOLS_DIR/.." && pwd)
cd $WORKSPACE/third_party
wget -q https://shutian.oss-cn-hangzhou.aliyuncs.com/cdn/grpc/grpc_v1.48.0.tar.gz
tar -xzf grpc_v1.48.0.tar.gz
mkdir grpc_build
cd grpc_build
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    cmake -DCMAKE_BUILD_TYPE=Release         \
    -DgRPC_SSL_PROVIDER=package              \
    -DgRPC_ZLIB_PROVIDER=package             \
    -DCMAKE_INSTALL_PREFIX=$HOME/grpc        \
    ../grpc_v1.48.0
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # Mac OSX
    cmake -DCMAKE_BUILD_TYPE=Release          \
    -DgRPC_SSL_PROVIDER=package               \
    -DgRPC_ZLIB_PROVIDER=package              \
    -DCMAKE_INSTALL_PREFIX=$HOME/grpc         \
    -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl \
    ../grpc_v1.48.0
fi

make -j $(nproc)
make install