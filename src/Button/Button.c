#include "Button.h"
#include "raylib.h"
#include <stdio.h>

Button createButton(int x, int y, int w, int h)
{
  Button btn;
  btn.x = x;
  btn.y = y;
  btn.w = w;
  btn.h = h;
  btn.isHeld = 0;

  return btn;
}

int isScreenTouched();

int isButtonPressed(Button *btn)
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
    return 1;
  }

  return 0;
}

void drawButton(Button btn, Color color)
{
  DrawRectangle(btn.x, btn.y, btn.w, btn.h, color);
}

void drawButtonTexture(Button btn, Texture2D *texture)
{
  Rectangle sourceRec = {0, 0, texture->width, texture->height};
  Rectangle destRec   = {btn.x, btn.y, btn.w, btn.h};
  if (btn.isHeld)
    DrawTexturePro(*texture, sourceRec, destRec, (Vector2){0, 0}, 0, (Color){200, 200, 200, 255});
  else
    DrawTexturePro(*texture, sourceRec, destRec, (Vector2){0, 0}, 0, WHITE);
}
