#!/bin/sh

echo $1
if [ $1 = "debug" ] || [ $1 = "all" ]; then
  echo "build project in debug mode"
  mkdir build-amd64-linux-debug
  cd build-amd64-linux-debug
  cmake -DCMAKE_BUILD_TYPE=Debug ../.
  cd ..
fi
if [ $1 = "release" ] || [ $1 = "all" ]; then
  echo "build project in release mode"
  mkdir build-amd64-linux-release
  cd build-amd64-linux-release
  cmake -DCMAKE_BUILD_TYPE=Release ../.
  cd ..
fi
