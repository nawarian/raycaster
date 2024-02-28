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
  player.angle = PI;
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
  player.pos = Vector2Add(player.pos, Vector2Scale(sight_dir, -delta.y));
}

void player_draw(void)
{
  if (render_mode != RENDER2D) return;

  Vector2 sight_dir, sight_end;
  float player_angle = Wrap(player.angle - PI, 0.0f, 2 * PI);

  // Draw FOV
  for (int x = 0; x < screen_plane_w; ++x) {
    float angle = player_angle - player.fov / screen_plane_w * x;
    // Rotate by half player FOV, so line of sight is centered
    angle = Wrap(angle + player.fov / 2.0f, 0.0f, 2 * PI);
    sight_dir = (Vector2) {
      cosf(angle),
      sinf(angle)
    };
    sight_end = Vector2Add(Vector2Scale(sight_dir, 2.0f), player.pos);
    DrawLineV(
      Vector2Scale(player.pos, wall_size),
      Vector2Scale(sight_end, wall_size),
      ColorAlpha(GREEN, 0.1f)
    );
  }

  // Draw sight direction
  sight_dir = (Vector2) {
    cosf(player_angle),
    sinf(player_angle)
  };
  sight_end = Vector2Add(Vector2Scale(sight_dir, 0.5f), player.pos);

  DrawLineV(
    Vector2Scale(player.pos, wall_size),
    Vector2Scale(sight_end, wall_size),
    GOLD
  );

  // Draw player
  DrawCircleV(Vector2Scale(player.pos, wall_size), 4.0f, RED);
}

void player_destroy(void)
{
  player = (player_t) { 0 };
}

