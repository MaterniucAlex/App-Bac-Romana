#include "Button.h"

Button createButton(int x, int y, int w, int h)
{
  Button btn;
  btn.x = x;
  btn.y = y;
  btn.w = w;
  btn.h = h;

  return btn;
}

int isButtonPressed(Button btn)
{
  int touchX = GetTouchX();
  int touchY = GetTouchY();
  
  if (touchX > btn.x && touchX < btn.x + btn.w &&
      touchY > btn.y && touchY < btn.y + btn.h)
    return 1;
  return 0;
}

void drawButton(Button btn, Color color)
{
  DrawRectangle(btn.x, btn.y, btn.w, btn.h, color);
}
