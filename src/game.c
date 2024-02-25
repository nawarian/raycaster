#include <raylib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define screen_plane_w 320
#define screen_plane_h 240

RenderTexture2D framebuff;

Camera2D cam = { 0 };
float zoom = 4.0f;

void UpdateDraw(void)
{
  cam.zoom = zoom;

  BeginDrawing();

    BeginTextureMode(framebuff);
      ClearBackground(BLACK);

      DrawCircle(screen_plane_w / 2, screen_plane_h / 2, 5.0f, RED);
    EndTextureMode();

    BeginMode2D(cam);
      DrawTextureRec(
        framebuff.texture,
        (Rectangle) { 0, 0, screen_plane_w, -screen_plane_h },
        (Vector2) { 0 },
        WHITE
      );
    EndMode2D();

    DrawFPS(0, 0);
  EndDrawing();
}

int main(void)
{
  InitWindow(screen_plane_w * zoom, screen_plane_h * zoom, "Raycaster");

  framebuff = LoadRenderTexture(screen_plane_w, screen_plane_h);
  while (!IsRenderTextureReady(framebuff));

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDraw, 120, 1);
#else
  SetTargetFPS(120);
  SetWindowMonitor(GetCurrentMonitor());

  while (!WindowShouldClose()) {
    UpdateDraw();
  }
#endif

  CloseWindow();

  return 0;
}

