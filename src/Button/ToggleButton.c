#include "ToggleButton.h"
#include "raylib.h"
#include <stdio.h>

ToggleButton createToggleButton(int x, int y, int w, int h)
{
  ToggleButton btn;
  btn.x = x;
  btn.y = y;
  btn.w = w;
  btn.h = h;
  btn.isToggled = 0;
  btn.isHeld = 0;

  return btn;
}

int isScreenTouched();

int isToggleButtonPressed(ToggleButton *btn)
{
  int touchX = GetTouchX();
  int touchY = GetTouchY();

  if (!(touchX > btn->x && touchX < btn->x + btn->w && touchY > btn->y && touchY < btn->y + btn->h)) //if touch is outside button
  {
    btn->isHeld = 0;
    return 0;
  }

  if (isScreenTouched() && btn->isHeld == 0) //if button was not touched before
  {
    btn->isHeld = 1;
    return 0;
  }

  if (!isScreenTouched() && btn->isHeld == 1) //if button was released
  {
    btn->isHeld = 0;
    btn->isToggled = !btn->isToggled;
  }

  return 0;
}

void drawToggleButton(ToggleButton btn, Color color)
{
  if (btn.isToggled)
    DrawRectangle(btn.x, btn.y, btn.w, btn.h, color);
  else
    DrawRectangle(btn.x, btn.y, btn.w, btn.h, (Color){color.r - 10, color.g - 10, color.b - 10, color.a});
}

void drawToggleButtonTexture(ToggleButton btn, Texture2D *texture)
{
  Rectangle sourceRec = {0, 0, texture->width, texture->height};
  Rectangle destRec   = {btn.x, btn.y, btn.w, btn.h};
  if (btn.isToggled)
    DrawTexturePro(*texture, sourceRec, destRec, (Vector2){0, 0}, 0, WHITE);
  else
    DrawTexturePro(*texture, sourceRec, destRec, (Vector2){0, 0}, 0, (Color){180, 180, 180, 255});
}
