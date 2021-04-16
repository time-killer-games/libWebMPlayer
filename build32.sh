cd "${0%/*}"

g++ "src/*" -I./include/ -o libWebMPlayer.dll -shared -static-libgcc -static-libstdc++ -static -lpng -lvorbis -logg -lwebm -lvpx -lz -m32 -Wno-narrowing