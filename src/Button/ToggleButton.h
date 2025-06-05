#include "raylib.h"

#ifndef TOGGLEBUTTON
#define TOGGLEBUTTON

typedef struct {
  int x;
  int y;
  int w;
  int h;
  int isHeld;
  int isToggled;
  char *text;
} ToggleButton;

ToggleButton createToggleButton(int x, int y, int w, int h);
int isToggleButtonPressed(ToggleButton *btn);
void drawToggleButton(ToggleButton btn, Color color);
void drawToggleButtonTexture(ToggleButton btn, Texture2D *texture);

#endif
