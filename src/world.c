#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
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

// TODO: refactor to use DDA instead of a 1/64.0f factor
void _draw3D(void)
{
  float player_angle = Wrap(player.angle, 0.0f, 2 * PI);
  float fov_unit = player.fov / screen_plane_w;
  float half_screen = screen_plane_h / 2;

  Color colors[] = {
    WHITE,
    RED,
    GRAY,
  };

  for (int x = 0; x < screen_plane_w; ++x) {
    // Calculate coords of next wall
    float angle = Wrap((fov_unit * x) + player_angle - (player.fov / 2.0f), 0.0f, 2 * PI);
    Vector2 dir = (Vector2) {
      cosf(angle),
      sinf(angle),
    };

    Vector2 end = player.pos;
    WallKind wall;
    while (true) {
      wall = world.walls[(int) end.y][(int) end.x];
      if (wall != WALL_NONE) {
        break;
      }

      end = Vector2Add(end, Vector2Scale(dir, 1/64.0f));
    }

    float dist = Vector2Distance(player.pos, end);
    dist *= cosf(angle - player_angle);

    float wall_height = screen_plane_h / dist;

    DrawLine(
      x,
      half_screen - wall_height * 0.4,
      x,
      half_screen + wall_height * 0.4,
      ColorAlpha(colors[wall], 1/dist)
    );
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

