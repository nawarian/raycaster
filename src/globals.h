#ifndef _globals_h_
#define _globals_h_

#include <raylib.h>
#include "player.h"
#include "world.h"

#define screen_plane_w 320
#define screen_plane_h 240
#define wall_size 32

typedef enum {
  RENDER2D = 0,
  RENDER3D = 1
} RenderMode;

extern RenderMode render_mode;
extern Camera2D cam;
extern Camera2D tex_cam;

#endif

