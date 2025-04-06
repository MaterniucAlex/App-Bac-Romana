#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "Button/Button.h"

#ifndef PHONE 
#define PHONE_WIDTH  (1080 * 0.3)
#define PHONE_HEIGHT (2412 * 0.3)
#endif

#ifdef PHONE
#define PHONE_WIDTH  1080
#define PHONE_HEIGHT 2412
#endif

int lastTouchX = 0;
int lastTouchY = 0;

Button buttonList[6];
Button fallingButtonList[3];

int selectedID = -1;
Color scoreColor = BLACK;

enum GameState {
  MAIN_MENU,
  GAME,
  PAUSED
};

enum GameState currentState;

void init();
void update();
void render();
void quit();
void selectScene(int stateID);
void setGamestate(enum GameState gs);
void respawn(Button *btn);

int score;
int highScore;

int main()
{
  init();

  while (!WindowShouldClose())
  {
    update();
    render();
  }

  quit();
  CloseWindow();

  return 0;
}

void init()
{
  srand(time(NULL));

  InitWindow(PHONE_WIDTH, PHONE_HEIGHT, "Bac Romana");
  SetTargetFPS(60);
  currentState = MAIN_MENU;

  int buttonWidth = PHONE_WIDTH / 7;

  //bar buttons
  buttonList[0] = createButton(1 * buttonWidth, PHONE_HEIGHT / 20 * 19 - buttonWidth / 2, buttonWidth, buttonWidth, selectScene);
  buttonList[1] = createButton(3 * buttonWidth, PHONE_HEIGHT / 20 * 19 - buttonWidth / 2, buttonWidth, buttonWidth, selectScene);
  buttonList[2] = createButton(5 * buttonWidth, PHONE_HEIGHT / 20 * 19 - buttonWidth / 2, buttonWidth, buttonWidth, selectScene);
  //play button
  buttonList[3] = createButton(PHONE_WIDTH / 4, PHONE_HEIGHT / 2 - PHONE_WIDTH / 8, PHONE_WIDTH / 2, PHONE_WIDTH / 4, setGamestate);
  //pause button
  buttonList[4] = createButton(PHONE_WIDTH / 12 * 11, 0, PHONE_WIDTH / 12, PHONE_WIDTH / 12, setGamestate);
  fallingButtonList[0] = createButton(0, 0, buttonWidth, buttonWidth, respawn);
  fallingButtonList[1] = createButton(0, 0, buttonWidth, buttonWidth, respawn);
  fallingButtonList[2] = createButton(0, 0, buttonWidth, buttonWidth, respawn);

  setButtonTag(&buttonList[0], 0);
  setButtonTag(&buttonList[1], 1);
  setButtonTag(&buttonList[2], 2);
  respawn(&fallingButtonList[0]);
  respawn(&fallingButtonList[1]);
  respawn(&fallingButtonList[2]);

  fallingButtonList[1].y -= buttonWidth * 2;
  fallingButtonList[2].y -= buttonWidth * 4;

  char *fileData = LoadFileText("assets/savedata/highScore.sv");
  highScore = atoi(fileData);
  score = 0;

}

void update()
{

  if (currentState == GAME)
    for(int i = 0; i < 3; i++)
    {
      fallingButtonList[i].y += PHONE_HEIGHT / 60 / 4;
      if (fallingButtonList[i].y >= PHONE_HEIGHT)
      {
        respawn(&fallingButtonList[i]);
        score -= 300;
      }
    }

  //touch protection
  if (lastTouchX == GetTouchX() && lastTouchY == GetTouchY()) return;

  if (currentState == PAUSED) currentState = GAME;

  if (currentState == MAIN_MENU)
  {
    if (isButtonPressed(buttonList[3])) currentState = GAME;
    return;
  }

  lastTouchX = GetTouchX();
  lastTouchY = GetTouchY();

  for(int i = 0; i < 3; i++)
  {
    if (isButtonPressed(buttonList[i]))
      buttonList[i].action(buttonList[i].tag);
    if (isButtonPressed(fallingButtonList[i]))
    {
      int pieceScore = 0;

      if (fallingButtonList[i].tag == selectedID)
      {
        pieceScore = 100 - fallingButtonList[i].y / (int)(PHONE_HEIGHT / 100);
      }
      else 
      {
        pieceScore = -300;
      }

      score += pieceScore;
      fallingButtonList[i].action(&fallingButtonList[i]);
    }
  }
  if (isButtonPressed(buttonList[4])) buttonList[4].action(PAUSED);

  if (score > highScore) 
    scoreColor = GREEN;
  else 
    scoreColor = BLACK;

}

void render()
{
  BeginDrawing();

  if (currentState == MAIN_MENU)
  {
    ClearBackground(RAYWHITE);
    drawButton(buttonList[3], GRAY);
    DrawText("Play", buttonList[3].x, buttonList[3].y, 24, BLACK);
    EndDrawing();
    return;
  }

  switch (selectedID)
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

  for(int i = 0; i < 3; i++)
  {
    switch (fallingButtonList[i].tag) {
      case 0:
        drawButton(fallingButtonList[i], PINK);
        break;
      case 1:
        drawButton(fallingButtonList[i], DARKBLUE);
        break;
      case 2:
        drawButton(fallingButtonList[i], DARKGREEN);
        break;
    }
    int pieceScore = 100 - fallingButtonList[i].y / (int)(PHONE_HEIGHT / 100);
    DrawText(TextFormat("%d", pieceScore),
             fallingButtonList[i].x, fallingButtonList[i].y - fallingButtonList[i].h / 3 , fallingButtonList[i].h / 3, BLACK);
  }

  DrawText(TextFormat("%d", score), 0, 0, PHONE_WIDTH / 8, scoreColor);

  //bottom bar
  DrawRectangle(0, PHONE_HEIGHT / 10 * 9, PHONE_WIDTH, PHONE_HEIGHT / 9.5, LIGHTGRAY);

  //State buttons
  drawButton(buttonList[0], RED);
  drawButton(buttonList[1], BLUE);
  drawButton(buttonList[2], GREEN);

  drawButton(buttonList[4], YELLOW);

  if (currentState == PAUSED)
    DrawText("Paused", PHONE_WIDTH / 4, PHONE_HEIGHT / 2 - PHONE_WIDTH / 24, PHONE_WIDTH / 12, BLACK);

  EndDrawing();
}

void quit()
{
  char* scoreStr;
  sprintf(scoreStr, "%d", score);
  SaveFileText("assets/savedata/highScore.sv", scoreStr);
}

void selectScene(int stateID)
{
  selectedID = stateID;
}

void setGamestate(enum GameState gs)
{
  currentState = gs;
}

int lastSpawnX = -1;
void respawn(Button *btn)
{
  int row = rand() % 7;
  while (lastSpawnX == row) row = rand() % 7;
  lastSpawnX = row;
  btn->x = row * PHONE_WIDTH / 7;
  btn->y = -PHONE_WIDTH / 7;
  setButtonTag(btn, buttonList[rand() % 3].tag);
}
