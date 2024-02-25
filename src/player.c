#include <raylib.h>
#include <raymath.h>

#include "globals.h"
#include "player.h"

Vector2 coords = { 0 };

void player_create(void)
{
  coords.x = screen_plane_w / 2;
  coords.y = screen_plane_h / 2;
}

void player_update(void)
{
  Vector2 delta = (Vector2) {
    IsKeyDown(KEY_D) - IsKeyDown(KEY_A),
    IsKeyDown(KEY_S) - IsKeyDown(KEY_W)
  };

  coords = Vector2Add(coords, delta);
}

void player_draw(void)
{
  DrawCircleV(coords, 5.0f, RED);
}

void player_destroy(void)
{
}

