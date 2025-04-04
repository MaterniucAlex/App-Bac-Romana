#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "Button/Button.h"

#ifndef SCALE
#define SCALE 0.3
#endif
#define PHONE_WIDTH  1080 * SCALE
#define PHONE_HEIGHT 2412 * SCALE

int lastTouchX = 0;
int lastTouchY = 0;

Button buttonList[6];

int stateNr = -1;

void update();
void changeState(int stateID);
void respawn(Button *btn);

int main()
{

  srand(time(NULL));

  InitWindow(PHONE_WIDTH, PHONE_HEIGHT, "Bac Romana");
  SetTargetFPS(60);

  int buttonWidth = PHONE_WIDTH / 7;

  buttonList[0] = createButton(1 * buttonWidth, PHONE_HEIGHT / 20 * 19 - buttonWidth / 2, buttonWidth, buttonWidth, changeState);
  buttonList[1] = createButton(3 * buttonWidth, PHONE_HEIGHT / 20 * 19 - buttonWidth / 2, buttonWidth, buttonWidth, changeState);
  buttonList[2] = createButton(5 * buttonWidth, PHONE_HEIGHT / 20 * 19 - buttonWidth / 2, buttonWidth, buttonWidth, changeState);
  buttonList[3] = createButton(0, 0, buttonWidth, buttonWidth, respawn);
  buttonList[4] = createButton(0, 0, buttonWidth, buttonWidth, respawn);
  buttonList[5] = createButton(0, 0, buttonWidth, buttonWidth, respawn);

  setButtonTag(&buttonList[0], 0);
  setButtonTag(&buttonList[1], 1);
  setButtonTag(&buttonList[2], 2);
  respawn(&buttonList[3]);
  respawn(&buttonList[4]);
  respawn(&buttonList[5]);

  while (!WindowShouldClose())
  {
    update();
  
    //Render
    BeginDrawing();
    
    switch (stateNr)
    {
      default:
      case -1:
        ClearBackground(RAYWHITE);
        break;
      case 0:
        ClearBackground(RED);
        break;
      case 1:
        ClearBackground(BLUE);
        break;
      case 2:
        ClearBackground(GREEN);
        break;
    }

    for(int i = 3; i < 6; i++)
      switch (buttonList[i].tag) {
        case 0:
          drawButton(buttonList[i], PINK);
          break;
        case 1:
          drawButton(buttonList[i], DARKBLUE);
          break;
        case 2:
          drawButton(buttonList[i], DARKGREEN);
          break;
      }

    //bottom bar
    DrawRectangle(0, PHONE_HEIGHT / 10 * 9, PHONE_WIDTH, PHONE_HEIGHT / 10, LIGHTGRAY);

    //State buttons
    drawButton(buttonList[0], RED);
    drawButton(buttonList[1], BLUE);
    drawButton(buttonList[2], GREEN);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

void update()
{
  for(int i = 3; i < 6; i++)
  {
    buttonList[i].y += 3;
    if (buttonList[i].y > PHONE_HEIGHT) respawn(&buttonList[i]);
  }

  if (lastTouchX != GetTouchX() || lastTouchY != GetTouchY())
  {
    lastTouchX = GetTouchX();
    lastTouchY = GetTouchY();

    for(int i = 0; i < 3; i++)
      if (isButtonPressed(buttonList[i]))
        buttonList[i].action(buttonList[i].tag);
    for(int i = 3; i < 6; i++)
      if (isButtonPressed(buttonList[i]) && buttonList[i].tag == stateNr)
        buttonList[i].action(&buttonList[i]);

  }
}

void changeState(int stateID)
{
  stateNr = stateID;
}

void respawn(Button *btn)
{
  btn->x = rand() % (int)(PHONE_WIDTH / 7 * 6);
  btn->y = -PHONE_WIDTH / 7;
  setButtonTag(btn, buttonList[rand() % 3].tag);
}

