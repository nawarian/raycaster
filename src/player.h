#ifndef _player_h_
#define _player_h_

#include <raylib.h>

typedef struct {
  Vector2 pos;
  float angle;
} player_t;

extern player_t player;

void player_create(void);
void player_update(void);
void player_draw(void);
void player_destroy(void);

#endif

