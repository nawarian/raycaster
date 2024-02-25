#include <raylib.h>

int main(void)
{
  InitWindow(800, 600, "Raycaster");
  SetTargetFPS(120);
  SetWindowMonitor(GetCurrentMonitor());

  while (!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(BLACK);
      DrawFPS(0, 0);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

