#include <raylib.h>
#include <raymath.h>

#include "globals.h"
#include "player.h"

player_t player;

void player_create(void)
{
  player.pos.x = 5.5f;
  player.pos.y = 2.5f;
}

void player_update(void)
{
  Vector2 delta = (Vector2) {
    IsKeyDown(KEY_D) - IsKeyDown(KEY_A),
    IsKeyDown(KEY_S) - IsKeyDown(KEY_W)
  };
  delta = Vector2Scale(delta, 0.1f);

  player.pos = Vector2Add(player.pos, delta);
}

void player_draw(void)
{
  if (render_mode != RENDER2D) return;

  DrawCircleV(Vector2Scale(player.pos, wall_size), 4.0f, RED);
}

void player_destroy(void)
{
  player = (player_t) { 0 };
}

