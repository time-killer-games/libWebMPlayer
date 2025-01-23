#!/bin/sh
cd "${0%/*}"
cd "src"

if [ $(uname) = "Darwin" ]; then
  git clone https://github.com/time-killer-games/libwebm ~/webm;cmake ~/webm -DCMAKE_BUILD_TYPE=release;make
  clang++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp file_reader.cpp libwebmplayer.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp player.cpp video_player.cpp webm_player.cpp libwebm.a -liconv /opt/local/lib/libogg.a /opt/local/lib/libvorbis.a /opt/local/lib/libvorbisenc.a /opt/local/lib/libvorbisfile.a /opt/local/lib/libvpx.a /opt/local/lib/libz.a /opt/local/lib/libSDL2.a -I$HOME/ -I$HOME/webm -I../include/ -o ../libWebMPlayer.dylib -std=c++17 -shared -DUSE_BGRA_COLOR -I/opt/local/include -lm -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-weak_framework,CoreHaptics -Wl,-weak_framework,GameController -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal -Wno-narrowing -arch arm64 -arch x86_64 -fPIC
elif [ $(uname) = "Linux" ]; then
  git clone https://github.com/time-killer-games/libwebm ~/webm;cmake ~/webm -DCMAKE_BUILD_TYPE=release;make
  g++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.cpp file_reader.cpp libwebmplayer.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp player.cpp video_player.cpp webm_player.cpp libwebm.a -I$HOME/ -I$HOME/webm -I../include/ -o ../libWebMPlayer.so -std=c++17 -shared -DUSE_BGRA_COLOR -static-libgcc -static-libstdc++ -lpthread -lvorbis -logg -lvpx `pkg-config --cflags --libs sdl2 --static` -Wno-narrowing -fPIC -DBUILD_ES
elif [ $(uname) = "FreeBSD" ]; then
  git clone https://github.com/time-killer-games/libwebm ~/webm;cmake ~/webm -DCMAKE_BUILD_TYPE=release;make
  clang++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.cpp file_reader.cpp libwebmplayer.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp player.cpp video_player.cpp webm_player.cpp libwebm.a -I$HOME/ -I$HOME/webm -I../include/ -o ../libWebMPlayer.so -std=c++17 -shared -DUSE_BGRA_COLOR -I/usr/local/include -L/usr/local/lib -lpthread -lvorbis -logg -lvpx -lc `pkg-config --cflags --libs sdl2 --static` -Wno-narrowing -fPIC -DBUILD_ES
elif [ $(uname) = "DragonFly" ]; then
  git clone https://github.com/time-killer-games/libwebm ~/webm;cmake ~/webm -DCMAKE_BUILD_TYPE=release;make
  g++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.cpp file_reader.cpp libwebmplayer.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp player.cpp video_player.cpp webm_player.cpp libwebm.a -I$HOME/ -I$HOME/webm -I../include/ -o ../libWebMPlayer.so -std=c++17 -shared -DUSE_BGRA_COLOR -I/usr/local/include -L/usr/local/lib -static-libgcc -static-libstdc++ -lpthread -lvorbis -logg -lvpx -lc `pkg-config --cflags --libs sdl2 --static` -Wno-narrowing -fPIC
elif [ $(uname) = "OpenBSD" ]; then
  git clone https://github.com/time-killer-games/libwebm ~/webm;cmake ~/webm -DCMAKE_BUILD_TYPE=release;make
  clang++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp yuv_rgb.cpp file_reader.cpp libwebmplayer.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp player.cpp video_player.cpp webm_player.cpp libwebm.a -I$HOME/ -I$HOME/webm -I../include/ -o ../libWebMPlayer.so -std=c++17 -shared -DUSE_BGRA_COLOR -I/usr/local/include -L/usr/local/lib -lpthread -lvorbis -logg -lvpx -lc `pkg-config --cflags --libs sdl2 --static` -Wno-narrowing -fPIC -DBUILD_ES
else
  g++ audio_decoder.cpp frame_buffer.cpp lodepng.cpp timer.cpp file_reader.cpp libwebmplayer.cpp gamemaker.cpp packet.cpp utils.cpp frame.cpp player.cpp video_player.cpp webm_player.cpp widenarrow.cpp -I../include/ -o ../libWebMPlayer.dll -std=c++17 -shared -DUSE_BGRA_COLOR -static-libgcc -static-libstdc++ -static -lvorbis -logg -lwebm -lvpx `pkg-config --cflags --libs sdl2 --static` -Wno-narrowing
fi
