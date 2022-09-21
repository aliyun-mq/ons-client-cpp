#!/usr/bin/env bash
TOOLS_DIR=$(dirname "$0")
WORKSPACE=$(cd -- "$TOOLS_DIR/.." && pwd)
cd $WORKSPACE/third_party
wget https://shutian.oss-cn-hangzhou.aliyuncs.com/cdn/grpc/grpc_v1.48.0.tar.gz
tar -xzvf grpc_v1.48.0.tar.gz
mkdir grpc_build
cd grpc_build
cmake -DCMAKE_BUILD_TYPE=Release -DgRPC_SSL_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package -DCMAKE_INSTALL_PREFIX=$HOME/grpc ../grpc_v1.48.0
make -j $(nproc)
make install