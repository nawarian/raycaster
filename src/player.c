#include <raylib.h>
#include <raymath.h>
#include <math.h>

#include "globals.h"
#include "player.h"

player_t player;

void player_create(void)
{
  player.pos.x = 6.2f;
  player.pos.y = 2.5f;
  player.angle = 0.0f;
  player.fov = 90.0f * DEG2RAD;
}

void player_update(void)
{
  Vector2 delta = (Vector2) {
    IsKeyDown(KEY_D) - IsKeyDown(KEY_A),
    IsKeyDown(KEY_W) - IsKeyDown(KEY_S)
  };
  delta = Vector2Scale(delta, 0.03f);

  player.angle = Wrap(player.angle + delta.x, 0.0f, PI * 2);

  // Thrust player
  Vector2 sight_dir = (Vector2) {
    cosf(player.angle),
    sinf(player.angle)
  };
  player.pos = Vector2Add(player.pos, Vector2Scale(sight_dir, delta.y));
}

void player_draw(void)
{
  if (render_mode != RENDER2D) return;

  // Draw player
  DrawCircleV(Vector2Scale(player.pos, wall_size), 4.0f, RED);

  // Draw sight direction
  Vector2 sight_dir, sight_end;
  float player_angle = Wrap(player.angle, 0.0f, 2 * PI);

  sight_dir = (Vector2) {
    cosf(player_angle),
    sinf(player_angle)
  };
  sight_end = Vector2Add(Vector2Scale(sight_dir, 0.3f), player.pos);

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

