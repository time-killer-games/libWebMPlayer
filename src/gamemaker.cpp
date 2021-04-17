#include "player.hpp"
#ifdef _WIN32
#include "libpng-util.h"
#else
#include "lodepng.h"
#endif
#include "yuv_rgb.h"

#include <string>
#include <vector>
#include <map>

#include <png.h>
#ifdef _WIN32
#include <windows.h>
#define EXPORTED_FUNCTION extern "C" __declspec(dllexport)
#else /* macOS, Linux, and BSD */
#define EXPORTED_FUNCTION extern "C" __attribute__((visibility("default")))
#endif

static int id = -1;
static std::map<int, uvpx::Player *> videos;

#if defined(_WIN32)
static std::wstring widen(std::string str) {
  std::size_t wchar_count = str.size() + 1;
  std::vector<wchar_t> buf(wchar_count);
  return std::wstring { buf.data(), 
    (std::size_t)MultiByteToWideChar(
    CP_UTF8, 0, str.c_str(), -1, 
    buf.data(), (int)wchar_count) };
}

static std::string narrow(std::wstring wstr) {
  int nbytes = WideCharToMultiByte(CP_UTF8,
    0, wstr.c_str(), (int)wstr.length(), 
    nullptr, 0, nullptr, nullptr);
  std::vector<char> buf(nbytes);
  return std::string { buf.data(), 
    (std::size_t)WideCharToMultiByte(
    CP_UTF8, 0, wstr.c_str(), 
    (int)wstr.length(), buf.data(), 
    nbytes, nullptr, nullptr) };
}
#endif

static void convert_rgb_to_rgba(const uint8_t *RGB, uint32_t width, uint32_t height, uint8_t **RGBA) {
  *RGBA = (uint8_t *)malloc(4 * width * height);
  for(uint32_t y = 0; y < height; ++y) {
    for(uint32_t x = 0; x < width; ++x) {
      (*RGBA)[(y * width + x) * 4] = RGB[(y * width + x) * 3];
      (*RGBA)[(y * width + x) * 4 + 1] = RGB[(y * width + x) * 3 + 1];
      (*RGBA)[(y * width + x) * 4 + 2] = RGB[(y * width + x) * 3 + 2];
      (*RGBA)[(y * width + x) * 4 + 3] = 255;
    }
  }
}

EXPORTED_FUNCTION double video_exists(double ind) {
  return (videos.find(ind) != videos.end());
}

EXPORTED_FUNCTION double video_add(char *fname) {
  uvpx::Player *player = new uvpx::Player(uvpx::Player::defaultConfig());
  uvpx::Player::LoadResult res = player->load(fname, 1, true);
  switch (res) {
   case uvpx::Player::LoadResult::FileNotExists:
    printf("Failed to open video file '%s'\n", fname);
   case uvpx::Player::LoadResult::UnsupportedVideoCodec:
    printf("Unsupported video codec\n");
   case uvpx::Player::LoadResult::NotInitialized:
    printf("Video player not initialized\n");
   case uvpx::Player::LoadResult::Success:
    id++; videos.insert(std::make_pair(id, player));
    printf("Video loaded successfully\n");	  
  }
  return id;
}

EXPORTED_FUNCTION double video_is_playing(double ind) {
  if (video_exists(ind)) {
    return videos[(int)ind]->isPlaying();
  }
  return false;
}

EXPORTED_FUNCTION double video_play(double ind) {
  if (video_exists(ind)) {
    videos[(int)ind]->play();
  }
  return video_is_playing(ind);
}

EXPORTED_FUNCTION double video_is_paused(double ind) {
  if (video_exists(ind)) {
    return videos[(int)ind]->isPaused();
  }
  return false;
}

EXPORTED_FUNCTION double video_pause(double ind) {
  if (video_exists(ind)) {
	videos[(int)ind]->pause();
  }
  return video_is_paused(ind);
}

EXPORTED_FUNCTION double video_is_stopped(double ind) {
  if (video_exists(ind)) {
    return videos[(int)ind]->isStopped();
  }
  return false;
}

EXPORTED_FUNCTION double video_stop(double ind) {
  if (video_exists(ind)) {
    videos[(int)ind]->stop();
  }
  return video_is_stopped(ind);
}

EXPORTED_FUNCTION double video_grab_frame(double ind, char *fname) {
  videos[(int)ind]->update(0);
  uvpx::Frame *yuv = nullptr;
  yuv = videos[(int)ind]->lockRead();
  if (yuv) {
    unsigned char *rgb = (unsigned char *)malloc(3 * yuv->displayWidth() * yuv->displayHeight());
    yuv420_rgb24_std(yuv->displayWidth(), yuv->displayHeight(), yuv->y(), yuv->u(), yuv->v(), 
    yuv->displayWidth(), (yuv->displayWidth() - 1) / 2, rgb, yuv->displayWidth() * 3, YCBCR_JPEG);
    if (rgb) {
      unsigned char *rgba = nullptr; 
      convert_rgb_to_rgba(rgb, yuv->displayWidth(), yuv->displayHeight(), &rgba);
      if (rgba) {
        #if defined(_WIN32)
        std::wstring wstr = widen(fname);
        libpng_encode32_file(rgba, yuv->displayWidth(), yuv->displayHeight(), wstr.c_str());
        #else
        lodepng_encode32_file(rgba, yuv->displayWidth(), yuv->displayHeight(), fname);
        #endif
        free(rgba);
      }
      videos[(int)ind]->unlockRead();
      free(rgb);
    }
  }
  return 0;
}
