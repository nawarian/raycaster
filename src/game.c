#include <raylib.h>
#include <raymath.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "globals.h"
#include "player.h"
#include "world.h"

RenderMode render_mode;
RenderTexture2D framebuff;

Camera2D cam = { 0 }; // game cam
Camera2D tex_cam = { 0 }; // render cam
float zoom = 3.0f;

void Create(void)
{
  InitWindow(screen_plane_w * zoom, screen_plane_h * zoom, "Raycaster");

  framebuff = LoadRenderTexture(screen_plane_w, screen_plane_h);
  while (!IsRenderTextureReady(framebuff));

  render_mode = RENDER2D;

  world_create();
  player_create();
}

void Update(void)
{
  if (IsKeyPressed(KEY_TAB)) {
    render_mode = !render_mode;
  }

  player_update();
  world_update();
}

void Draw(void)
{
  // Reset texture mode zoom
  tex_cam.zoom = zoom;
  tex_cam.target = Vector2Zero();
  tex_cam.offset = Vector2Zero();

  // Update in-game camera to follow player
  cam.zoom = 1.0f;
  cam.target = Vector2Zero();
  cam.offset = Vector2Zero();
  if (render_mode == RENDER2D) {
    cam.target = Vector2Scale(player.pos, wall_size);
    cam.offset = (Vector2) { (float) screen_plane_w / 2, (float) screen_plane_h / 2};
  }

  BeginTextureMode(framebuff);
    ClearBackground(BLACK);

    BeginMode2D(cam);
      world_draw();
      player_draw();
    EndMode2D();
  EndTextureMode();

  BeginMode2D(tex_cam);
    ClearBackground(BLACK);

    DrawTexturePro(
      framebuff.texture,
      (Rectangle) { 0, 0, screen_plane_w, -screen_plane_h },
      (Rectangle) { 0, 0, screen_plane_w, screen_plane_h },
      (Vector2) { 0, 0 },
      0.0f,
      WHITE
    );
  EndMode2D();

  DrawFPS(0, 0);
  DrawText(TextFormat("Press [TAB] to switch to %s", render_mode == RENDER2D ? "3D mode" : "2D mode"), 0, 20, 20, LIME);
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

