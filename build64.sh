#!/bin/sh
cd "${0%/*}"
cd "src"

if [ $(uname) = "Darwin" ]; then
  git clone https://github.com/time-killer-games/libwebm ~/webm;cmake ~/webm -DCMAKE_BUILD_TYPE=release;make
  clang++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp file_reader.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp libpng-util.cpp player.cpp video_player.cpp webm_player.cpp libwebm.a /usr/local/lib/libogg.a /usr/local/lib/libvorbis.a /usr/local/lib/libvorbisenc.a /usr/local/lib/libvorbisfile.a /usr/local/lib/libvpx.a /usr/local/opt/zlib/lib/libz.a /usr/local/lib/libSDL2.a -I$HOME/ -I$HOME/webm -I../include/ -o ../libWebMPlayer.dylib -std=c++17 -shared -I/usr/local/include -lm -liconv -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-weak_framework,CoreHaptics -Wl,-weak_framework,GameController -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal -m64 -Wno-narrowing -fPIC
elif [ $(uname) = "Linux" ]; then
  git clone https://github.com/time-killer-games/libwebm ~/webm;cmake ~/webm -DCMAKE_BUILD_TYPE=release;make
  g++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.cpp file_reader.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp libpng-util.cpp player.cpp video_player.cpp webm_player.cpp libwebm.a -I$HOME/ -I$HOME/webm -I../include/ -o ../libWebMPlayer.so -std=c++17 -shared -static-libgcc -static-libstdc++ -lvorbis -logg -lvpx -lz `pkg-config --cflags --libs sdl2 --static` -m64 -Wno-narrowing -fPIC
elif [ $(uname) = "FreeBSD" ]; then
  git clone https://github.com/time-killer-games/libwebm ~/webm;cmake ~/webm -DCMAKE_BUILD_TYPE=release;make
  clang++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.cpp file_reader.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp libpng-util.cpp player.cpp video_player.cpp webm_player.cpp libwebm.a -I$HOME/ -I$HOME/webm -I../include/ -o ../libWebMPlayer.so -std=c++17 -shared -I/usr/local/include -L/usr/local/lib -lvorbis -logg -lvpx -lz -lc `pkg-config --cflags --libs sdl2 --static` -lpthread -lc -m64 -Wno-narrowing -fPIC
else
  g++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp file_reader.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp libpng-util.cpp player.cpp video_player.cpp webm_player.cpp widen_narrow.cpp -I../include/ -o ../libWebMPlayer.dll -std=c++17 -shared -static-libgcc -static-libstdc++ -static -lpng -lvorbis -logg -lwebm -lvpx -lz `pkg-config --cflags --libs sdl2 --static` -m64 -Wno-narrowing
fi
