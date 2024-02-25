#ifndef _world_h_
#define _world_h_

#include <raylib.h>

typedef enum WallKind {
  WALL_NONE = 0,
  WALL_GRAY,
  WALL_RED
} WallKind;

typedef struct {
  Vector2 dim;
  WallKind **walls;
} world_t;

extern world_t world;

void world_create(void);
void world_update(void);
void world_draw(void);
void world_destroy(void);

#endif

