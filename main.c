#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#include "tools.h"
#include "softwareAPI.h"
#include "renderingAPI.h"
#include "game.h"

int main(void)
{
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  InitScreenBuffer(screenWidth, screenWidth);
  LoadGameLevel("level-1");
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    ControlPlayer();
    // SOFTWARE Draw
    DrawColumns();
    DrawMinimap();
    UpdateScreenBuffer();

    // RAYLIB Draw
    BeginDrawing(); ClearBackground(BLUE);
      DrawScreenBuffer();
    EndDrawing();
  }

  // De-Initialization
  CloseWindow();
  DeInitScreenBuffer();
  free(GameLevel);
  return 0;
}