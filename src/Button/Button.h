#include "raylib.h"

#ifndef BUTTON
#define BUTTON

typedef struct {
  int x;
  int y;
  int w;
  int h;
  void (*action)();
  int tag;
  char *text;
} Button;

Button createButton(int x, int y, int w, int h, void (*action)());
int isButtonPressed(Button btn);
void drawButton(Button btn, Color color);
void setButtonTag(Button *btn, int tag);

#endif
