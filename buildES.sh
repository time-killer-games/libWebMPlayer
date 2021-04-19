#!/bin/sh
cd "${0%/*}"
cd "src"

if [ $(uname) = "Linux" ]; then
  git clone https://github.com/time-killer-games/libwebm ~/webm;cmake ~/webm -DCMAKE_BUILD_TYPE=release;make
  g++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.cpp file_reader.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp libpng-util.cpp player.cpp video_player.cpp libwebm.a -I$HOME/ -I$HOME/webm -I../include/ -DBUILD_ES -o ../libWebMPlayer.so -std=c++17 -shared -static-libgcc -static-libstdc++ -lpng -lvorbis -logg -lvpx -lz -Wno-narrowing -fPIC
elif [ $(uname) = "FreeBSD" ]; then
  git clone https://github.com/time-killer-games/libwebm ~/webm;cmake ~/webm -DCMAKE_BUILD_TYPE=release;make
  clang++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.cpp file_reader.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp libpng-util.cpp player.cpp video_player.cpp libwebm.a -I$HOME/ -I$HOME/webm -I../include/ -DBUILD_ES -o ../libWebMPlayer.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -static -lpng -lvorbis -logg -lvpx -lz -Wno-narrowing -fPIC
fi
