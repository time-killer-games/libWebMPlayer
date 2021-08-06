#include "libwebmplayer.h"

#ifdef _WIN32
#define EXPORTED_FUNCTION extern "C" __declspec(dllexport)
#else /* macOS, Linux, and BSD */
#define EXPORTED_FUNCTION extern "C" __attribute__((visibility("default")))
#endif

EXPORTED_FUNCTION double video_exists(double ind) {
  return libwebmplayer::video_exists((int)ind);
}

EXPORTED_FUNCTION double video_add(char *fname) {
  return libwebmplayer::video_add(fname);
}

EXPORTED_FUNCTION double video_delete(double ind) {
  return libwebmplayer::video_delete((int)ind);
}

EXPORTED_FUNCTION double video_is_playing(double ind) {
  return libwebmplayer::video_is_playing((int)ind);
}

EXPORTED_FUNCTION double video_play(double ind) {
  return libwebmplayer::video_play((int)ind);
}

EXPORTED_FUNCTION double video_is_paused(double ind) {
  return libwebmplayer::video_is_paused((int)ind);
}

EXPORTED_FUNCTION double video_pause(double ind) {
  return libwebmplayer::video_pause((int)ind);
}

EXPORTED_FUNCTION double video_is_stopped(double ind) {
  return libwebmplayer::video_is_stopped((int)ind);
}

EXPORTED_FUNCTION double video_stop(double ind) {
  return libwebmplayer::video_stop((int)ind);
}

EXPORTED_FUNCTION double video_get_property(double ind, double prop) {
  return libwebmplayer::video_get_property((int)ind, (int)prop);
}

EXPORTED_FUNCTION double video_get_width(double ind) {
  return libwebmplayer::video_get_width((int)ind);
}

EXPORTED_FUNCTION double video_get_height(double ind) {
  return libwebmplayer::video_get_height((int)ind);
}

EXPORTED_FUNCTION double video_get_playtime(double ind) {
  return libwebmplayer::video_get_playtime((int)ind);
}

EXPORTED_FUNCTION double video_get_duration(double ind) {
  return libwebmplayer::video_get_duration((int)ind);
}

EXPORTED_FUNCTION double video_grab_frame_image(double ind, char *fname) {
  return libwebmplayer::video_grab_frame_image((int)ind, fname);
}

EXPORTED_FUNCTION double video_grab_frame_buffer(double ind, char *buffer) {
  return libwebmplayer::video_grab_frame_buffer((int)ind, (unsigned char *buffer));
}
