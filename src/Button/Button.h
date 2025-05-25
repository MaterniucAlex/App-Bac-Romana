#include "raylib.h"

#ifndef BUTTON
#define BUTTON

typedef struct {
  int x;
  int y;
  int w;
  int h;
  char *text;
} Button;

Button createButton(int x, int y, int w, int h);
int isButtonPressed(Button btn);
void drawButton(Button btn, Color color);
void drawButtonTexture(Button btn, Texture2D *texture);

#endif
