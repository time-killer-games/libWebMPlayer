#!/bin/sh
cd "${0%/*}"

if [ $(uname) = "Darwin" ]; then
  clang++ "src/*" -I./include/ -o libWebMPlayer.dylib -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -static -lpng -lvorbis -logg -lwebm -lvpx -lz -m64 -Wno-narrowing -fPIC
elif [ $(uname) = "Linux" ]; then
  g++ "src/*" -I./include/ -o libWebMPlayer.so -std=c++17 -shared -static-libgcc -static-libstdc++ -static -lpng -lvorbis -logg -lwebm -lvpx -lz -m64 -Wno-narrowing
elif [ $(uname) = "FreeBSD" ]; then
  clang++ "src/*" -I./include/ -o libWebMPlayer.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -static -lpng -lvorbis -logg -lwebm -lvpx -lz -m64 -Wno-narrowing -fPIC
else
  g++ "src/*" -I./include/ -o libWebMPlayer.dll -std=c++17 -shared -static-libgcc -static-libstdc++ -static -lpng -lvorbis -logg -lwebm -lvpx -lz -m64 -Wno-narrowing
fi
