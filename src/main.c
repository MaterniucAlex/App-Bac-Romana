#include "raylib.h"

#define PHONE_WIDTH  1080
#define PHONE_HEIGHT 2412
#define SCALE 3

int main()
{
  InitWindow(PHONE_WIDTH / SCALE, PHONE_HEIGHT / SCALE, "Bac Romana");
  SetTargetFPS(60);

  int taps = 0;

  while (!WindowShouldClose())
  {
    //Update
    if (IsGestureDetected(GESTURE_TAP))
      taps++;
  
    //Render
    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    DrawText(TextFormat("Taps: %i", taps), 
             (PHONE_WIDTH  / SCALE) / 2, 
             (PHONE_HEIGHT / SCALE) / 2, 20, BLACK);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
