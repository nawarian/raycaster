#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "globals.h"
#include "raymath.h"
#include "world.h"

world_t world;

void _load_level(const char *filename)
{
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    TraceLog(LOG_FATAL, "Failed to load level '%s'", filename);
  }

  int width, height;
  fscanf(fp, "%d %d", &width, &height);

  world = (world_t) { 0 };
  world.dim = (Vector2) { width, height };

  world.walls = malloc(sizeof(WallKind *) * height);
  for (int y = 0; y < height; ++y) {
    world.walls[y] = (WallKind *) malloc(sizeof(WallKind) * width);
  }

  int y = 0;
  int x = 0;
  char buff[2];
  do {
    x = 0;
    do {
      fread(buff, 2, 1, fp);
      world.walls[y][x++] = atoi(buff);
    } while (x < width);

    ++y;
  } while (y < height);

  fclose(fp);
}

void world_create(void)
{
  _load_level("resources/level01.txt");
}

void world_update(void)
{
}

void _draw2D(void)
{
  WallKind wall;
  Color color;

  // Draw walls
  for (int y = 0; y < world.dim.y; ++y) {
    for (int x = 0; x < world.dim.x; ++x) {
      wall = world.walls[y][x];
      Rectangle rec = (Rectangle) {
        x * wall_size,
        y * wall_size,
        wall_size,
        wall_size
      };

      if (wall == WALL_NONE) {
        DrawRectangleLinesEx(rec, 0.5f, ColorAlpha(GRAY, 0.4f));
        continue;
      }

      switch (wall) {
        case WALL_GRAY:
          color = LIGHTGRAY;
          break;
        case WALL_RED:
          color = RED;
          break;
        default:
          break;
      }

      DrawRectangleRec(rec, color);
      DrawRectangleLinesEx(rec, 0.5f, ColorAlpha(GRAY, 0.4f));
    }
  }
}

void _draw3D(void)
{
  WallKind wall;
  Color colors[] = {
    WHITE,
    RED,
    GRAY,
  };

  float half_screen = screen_plane_h / 2.0f;
  float player_angle = Wrap(player.angle, 0.0f, 2 * PI);
  float fov_unit = player.fov / screen_plane_w;
  float half_fov = player.fov / 2.0f;

  // Iterate over all screen pixels
  for (int x = 0; x < screen_plane_w; ++x) {
    float angle = Wrap((fov_unit * x) + player_angle - half_fov, 0.0f, 2 * PI);
    Vector2 map = (Vector2) { (int) player.pos.x, (int) player.pos.y };

    Vector2 dir = (Vector2) {
      cosf(angle),
      sinf(angle),
    };

    Vector2 delta_dist = (Vector2) {
      dir.x == 0.0f ? 1e30 : fabs(1.0f / dir.x),
      dir.y == 0.0f ? 1e30 : fabs(1.0f / dir.y),
    };

    Vector2 ray_length = Vector2Zero();
    Vector2 step = Vector2Zero();

    if (dir.x < 0) {
      step.x = -1;
      ray_length.x = (player.pos.x - map.x) * delta_dist.x;
    } else {
      step.x = 1;
      ray_length.x = ((map.x + 1) - player.pos.x) * delta_dist.x;
    }

    if (dir.y < 0) {
      step.y = -1;
      ray_length.y = (player.pos.y - map.y) * delta_dist.y;
    } else {
      step.y = 1;
      ray_length.y = ((map.y + 1) - player.pos.y) * delta_dist.y;
    }

    bool wall_found = false;
    float dist = 0.0f;
    while (!wall_found && dist < MAX_RENDER_DISTANCE) {
      // Walk
      if (ray_length.x < ray_length.y) {
        map.x += step.x;
        dist = ray_length.x;
        ray_length.x += delta_dist.x;
      } else {
        map.y += step.y;
        dist = ray_length.y;
        ray_length.y += delta_dist.y;
      }

      // Check wall
      if (map.x >= 0 && map.x < world.dim.x && map.y >= 0 && map.y < world.dim.y) {
        wall = world.walls[(int) map.y][(int) map.x];

        if (wall != WALL_NONE) {
          wall_found = true;
        }
      }
    }

    if (wall_found) {
      Vector2 end = Vector2Add(
        player.pos,
        Vector2Scale(dir, dist)
      );

      dist = Vector2Distance(player.pos, end);
      // Fix fisheye effect
      dist *= cosf(angle - player_angle);

      float wall_height = screen_plane_h / dist;

      DrawLine(
        x,
        half_screen - wall_height * 0.4f,
        x,
        half_screen + wall_height * 0.4f,
        ColorAlpha(colors[wall], 1/dist)
      );
    }
  }
}

void world_draw(void)
{
  if (render_mode == RENDER2D) {
    _draw2D();
  } else {
    _draw3D();
  }
}

void world_destroy(void)
{
  for (int y = 0; y < world.dim.y; ++y) {
    free(world.walls[y]);
  }

  free(world.walls);
}

