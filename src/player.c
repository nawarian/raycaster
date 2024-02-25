#include <raylib.h>
#include <raymath.h>
#include <math.h>

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
  delta = Vector2Scale(delta, 0.03f);

  player.angle = Wrap(player.angle + delta.x, 0.0f, PI * 2);

  // Thrust player
  Vector2 sight_dir = (Vector2) {
    cosf(player.angle),
    sinf(player.angle)
  };
  player.pos = Vector2Add(player.pos, Vector2Scale(sight_dir, -delta.y));
}

void player_draw(void)
{
  if (render_mode != RENDER2D) return;

  DrawCircleV(Vector2Scale(player.pos, wall_size), 4.0f, RED);

  Vector2 sight_dir = (Vector2) {
    cosf(player.angle),
    sinf(player.angle)
  };
  Vector2 sight_end = Vector2Add(Vector2Scale(sight_dir, 0.5f), player.pos);

  DrawLineV(
    Vector2Scale(player.pos, wall_size),
    Vector2Scale(sight_end, wall_size),
    GOLD
  );
}

void player_destroy(void)
{
  player = (player_t) { 0 };
}

