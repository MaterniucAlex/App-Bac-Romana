#include "Button.h"

Button createButton(int x, int y, int w, int h, void (*action)())
{
  Button btn;
  btn.x = x;
  btn.y = y;
  btn.w = w;
  btn.h = h;
  btn.action = action;
  btn.tag = -1;

  return btn;
}

void setButtonTag(Button *btn, int tag)
{
  btn->tag = tag;
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
