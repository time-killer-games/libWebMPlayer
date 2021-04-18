#include "player.hpp"
#ifdef _WIN32
#include "libpng-util.h"
#include "widen_narrow.h"
#else
#include "lodepng.h"
#endif
#include "yuv_rgb.h"

#include <string>
#include <vector>
#include <unordered_map>

#include <png.h>
#ifdef _WIN32
#include <windows.h>
#define EXPORTED_FUNCTION extern "C" __declspec(dllexport)
#else /* macOS, Linux, and BSD */
#define EXPORTED_FUNCTION extern "C" __attribute__((visibility("default")))
#endif

static int id = -1;
static std::unordered_map<int, uvpx::Player *> videos;
static std::unordered_map<int, float> videos_time;
static float gamespeed = 60.0f;
static float gametimer = 0.0f;

static void convert_rgb_to_rgba(const uint8_t *RGB, uint32_t width, uint32_t height, uint8_t **RGBA, uint32_t dispWidth, uint32_t dispHeight) {
  if ((*RGBA) == nullptr) {
    printf("RGBA buffer given is nullptr, allocating a new one.\n");
    *RGBA = (uint8_t *)malloc(4 * dispWidth * dispHeight);
  }
  
  //printf("RGBA=%p\n",*RGBA);
  for(uint32_t y = 0; y < height; ++y) {
    if (y > dispHeight) break;
    for(uint32_t x = 0; x < width; ++x) {
      if (x > dispWidth) break;
      (*RGBA)[(y * dispWidth + x) * 4] = RGB[(y * width + x) * 3];
      (*RGBA)[(y * dispWidth + x) * 4 + 1] = RGB[(y * width + x) * 3 + 1];
      (*RGBA)[(y * dispWidth + x) * 4 + 2] = RGB[(y * width + x) * 3 + 2];
      (*RGBA)[(y * dispWidth + x) * 4 + 3] = 255;
    }
  }
}

EXPORTED_FUNCTION double video_set_gamespeed(double spd) {
  double oldgamespeed = gamespeed;
  gamespeed = (float)spd;
  return oldgamespeed;
}

EXPORTED_FUNCTION double video_set_gametimer(double timer) {
  double oldgametimer = gametimer;
  gametimer = (float)timer;
  return oldgametimer;
}

EXPORTED_FUNCTION double video_advance_time(double ind) { // TODO: properly finish timing stuff
  //videos[(int)ind]->update(videos_time[(int)ind]);
  videos[(int)ind]->update(0);
  float endTime = gametimer / 1000.0f;
  return 1.0;
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
    id++; videos.insert(std::make_pair(id, player)); videos_time[id] = 1.0f / gamespeed;
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

// Also mirror this into the extension constants:
const int WEBM_INVALID = 0; // reserved.
const int WEBM_WIDTH   = 1;
const int WEBM_HEIGHT  = 2;
const int WEBM_TIME    = 3;

EXPORTED_FUNCTION double video_get_property(double ind, double propertyId) {
  if (video_exists(ind)) {
    uvpx::Frame *yuv = nullptr;
    yuv = videos[(int)ind]->lockRead();
    if (yuv) {
      double ret;
	    switch ((int)propertyId) {
	      case WEBM_WIDTH:  ret = yuv->displayWidth();  break;
	      case WEBM_HEIGHT: ret = yuv->displayHeight(); break;
	      case WEBM_TIME:   ret = yuv->time();          break;
	      default:          ret = -1.0;                 break;
	    }
	    videos[(int)ind]->unlockRead();
	    
	    return ret;
    }
    
    return -2.0;
  }
  
  return -3.0;
  // -1 - unknown property
  // -2 - unable to lock yuv video
  // -3 - video does not exist.
}

EXPORTED_FUNCTION double video_get_width(double ind) {
  return video_get_property(ind, WEBM_WIDTH);
}

EXPORTED_FUNCTION double video_get_height(double ind) {
  return video_get_property(ind, WEBM_HEIGHT);
}

EXPORTED_FUNCTION double video_get_time(double ind) {
  return video_get_property(ind, WEBM_TIME);
}

EXPORTED_FUNCTION double video_grab_frame(double ind, char *fname) {
  if (video_exists(ind)) {
    video_advance_time(ind);
    uvpx::Frame *yuv = nullptr;
    yuv = videos[(int)ind]->lockRead();
    if (yuv) {
      unsigned char *rgb = (unsigned char *)malloc(3 * yuv->width() * yuv->height());
      yuv420_rgb24_std(yuv->width(), yuv->height(), yuv->y(), yuv->u(), yuv->v(), 
      yuv->yPitch(), yuv->uvPitch(), rgb, yuv->width() * 3, YCBCR_JPEG);
      if (rgb) {
        unsigned char *rgba = nullptr;
        convert_rgb_to_rgba(rgb, yuv->width(), yuv->height(), &rgba, yuv->displayWidth(), yuv->displayHeight());
        free(rgb);
        videos[(int)ind]->unlockRead();
        
        if (rgba) {
          #if defined(_WIN32)
          std::wstring wstr = widen(fname);
          libpng_encode32_file(rgba, yuv->displayWidth(), yuv->displayHeight(), wstr.c_str());
          #else
          lodepng_encode32_file(fname, rgba, yuv->displayWidth(), yuv->displayHeight());
          #endif
          free(rgba);
          
          return 1.0;
        }
        
        return 0.0;
      }
      
      return -1.0;
    }
    
    return -2.0;
  }
  
  return -3.0;
}

EXPORTED_FUNCTION double video_grab_frame_buffer(double ind, char *buffer) {
  if (video_exists(ind)) {
    video_advance_time(ind);
    uvpx::Frame *yuv = nullptr;
    yuv = videos[(int)ind]->lockRead();
    if (yuv) {
      unsigned char *rgb = (unsigned char *)malloc(3 * yuv->width() * yuv->height());
      yuv420_rgb24_std(yuv->width(), yuv->height(), yuv->y(), yuv->u(), yuv->v(), 
      yuv->yPitch(), yuv->uvPitch(), rgb, yuv->width() * 3, YCBCR_JPEG);
      if (rgb) {
        unsigned char *rgba = (unsigned char *)buffer;
        convert_rgb_to_rgba(rgb, yuv->width(), yuv->height(), &rgba, yuv->displayWidth(), yuv->displayHeight());
        free(rgb);
        videos[(int)ind]->unlockRead();
        
        return 1.0;
      }
      
      return -1.0;
    }
    
    return -2.0;
  }
  
  return -3.0;
}
