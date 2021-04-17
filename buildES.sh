#!/bin/sh
cd "${0%/*}"
cd "src"

if [ $(uname) = "Darwin" ]; then
  git clone https://github.com/webmproject/libwebm ~/webm
  clang++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.c file_reader.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp libpng-util.cpp player.cpp video_player.cpp -I$HOME/ -I$HOME/webm -I../include/ -o ../libWebMPlayer.dylib -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -static -lpng -lvorbis -logg -lvpx -lz -Wno-narrowing -fPIC
elif [ $(uname) = "Linux" ]; then
  git clone https://github.com/webmproject/libwebm ~/webm
  g++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.c file_reader.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp libpng-util.cpp player.cpp video_player.cpp -I$HOME/ -I$HOME/webm -I../include/ -o ../libWebMPlayer.so -std=c++17 -shared -static-libgcc -static-libstdc++ -lpng -lvorbis -logg -lvpx -lz -Wno-narrowing -fPIC
elif [ $(uname) = "FreeBSD" ]; then
  git clone https://github.com/webmproject/libwebm ~/webm
  clang++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.c file_reader.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp libpng-util.cpp player.cpp video_player.cpp -I$HOME/ -I$HOME/webm -I../include/ -o ../libWebMPlayer.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -static -lpng -lvorbis -logg -lvpx -lz -Wno-narrowing -fPIC
else
  git clone https://github.com/webmproject/libwebm /c/webm
  g++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.c file_reader.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp libpng-util.cpp player.cpp video_player.cpp -I/c/ -I../include/ -o ../libWebMPlayer.dll -std=c++17 -shared -static-libgcc -static-libstdc++ -static -lpng -lvorbis -logg -lwebm -lvpx -lz -Wno-narrowing
fi
