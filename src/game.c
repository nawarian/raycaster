#include <raylib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "globals.h"
#include "player.h"
#include "world.h"

RenderTexture2D framebuff;

Camera2D cam = { 0 };
float zoom = 4.0f;

void Create(void)
{
  InitWindow(screen_plane_w * zoom, screen_plane_h * zoom, "Raycaster");

  framebuff = LoadRenderTexture(screen_plane_w, screen_plane_h);
  while (!IsRenderTextureReady(framebuff));

  world_create();
  player_create();
}

void Update(void)
{
  cam.zoom = zoom;

  player_update();
  world_update();
}

void Draw(void)
{
  BeginTextureMode(framebuff);
    ClearBackground(BLACK);

    world_draw();
    player_draw();
  EndTextureMode();

  BeginMode2D(cam);
    ClearBackground(BLACK);

    DrawTextureRec(
      framebuff.texture,
      (Rectangle) { 0, 0, screen_plane_w, -screen_plane_h },
      (Vector2) { 0 },
      WHITE
    );
  EndMode2D();

  DrawFPS(0, 0);
}

void UpdateDraw(void)
{
  Update();
  BeginDrawing();
    Draw();
  EndDrawing();
}

void Destroy(void)
{
  player_destroy();
  world_destroy();

  CloseWindow();
}

int main(void)
{
  Create();

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDraw, 120, 1);
#else
  SetTargetFPS(120);
  SetWindowMonitor(GetCurrentMonitor());

  while (!WindowShouldClose()) {
    UpdateDraw();
  }
#endif

  Destroy();

  return 0;
}

