#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "Button/Button.h"
#include "Opere/opere.h"

int PHONE_WIDTH  = 1080 / 2;
int PHONE_HEIGHT = 2412 / 2;

#define COLUMN_NR 5
#define SECONDS_TO_FALL 6.5
#define NR_OPERE 3

int lastTouchX = 0;
int lastTouchY = 0;

Button buttonList[6];
Button fallingButtonList[3];

int selectedID = -1;
Color scoreColor = BLACK;

int score;
int highScore;

int FallingButton0TextId = 0;
int FallingButton1TextId = 0;
int FallingButton2TextId = 0;

enum GameState {
  MAIN_MENU,
  GAME,
  PAUSED
};

enum GameState currentState;
Sound goodSound;

Opera listOpere[15];

void init();
void update();
void render();
void quit();
void selectScene(int stateID);
void setGamestate(enum GameState gs);
void respawn(Button *btn, int i);
void drawCustomText(char *text, int x, int y, int fontSize);

int main()
{
  init();

  while (!WindowShouldClose())
  {
    update();
    render();
  }

  quit();
  CloseAudioDevice();
  CloseWindow();

  return 0;
}

void init()
{
  srand(time(NULL));

  InitWindow(PHONE_WIDTH, PHONE_HEIGHT, "Bac Romana");

  #ifdef PHONE
  MaximizeWindow();
  ToggleBorderlessWindowed();
  PHONE_WIDTH  = GetScreenWidth ();
  PHONE_HEIGHT = GetScreenHeight();
  #endif

  InitAudioDevice();
  SetTargetFPS(60);
  currentState = MAIN_MENU;


  int buttonWidth = PHONE_WIDTH / COLUMN_NR;

  initOpere(listOpere);

  //bar buttons
  int gap = (PHONE_WIDTH - (buttonWidth * 3)) / 4;
  buttonList[0] = createButton(1 * gap + 0 * buttonWidth, PHONE_HEIGHT / 20 * 19 - buttonWidth / 2, buttonWidth, buttonWidth, selectScene);
  buttonList[1] = createButton(2 * gap + 1 * buttonWidth, PHONE_HEIGHT / 20 * 19 - buttonWidth / 2, buttonWidth, buttonWidth, selectScene);
  buttonList[2] = createButton(3 * gap + 2 * buttonWidth, PHONE_HEIGHT / 20 * 19 - buttonWidth / 2, buttonWidth, buttonWidth, selectScene);
  //play button
  buttonList[3] = createButton(PHONE_WIDTH / 4, PHONE_HEIGHT / 2 - PHONE_WIDTH / 8, PHONE_WIDTH / 2, PHONE_WIDTH / 4, setGamestate);
  //pause button
  buttonList[4] = createButton(PHONE_WIDTH / 12 * 11, 0, PHONE_WIDTH / 12, PHONE_WIDTH / 12, setGamestate);
  fallingButtonList[0] = createButton(0, 0, buttonWidth, buttonWidth, respawn);
  fallingButtonList[1] = createButton(0, 0, buttonWidth, buttonWidth, respawn);
  fallingButtonList[2] = createButton(0, 0, buttonWidth, buttonWidth, respawn);
  setButtonTag(&buttonList[0], rand() % NR_OPERE);
  do {
    setButtonTag(&buttonList[1], rand() % NR_OPERE);
  } while(buttonList[1].tag == buttonList[0].tag);
  
  do {
    setButtonTag(&buttonList[2], rand() % NR_OPERE);
  } while(buttonList[2].tag == buttonList[0].tag || buttonList[2].tag == buttonList[1].tag);
  respawn(&fallingButtonList[0], 0);
  respawn(&fallingButtonList[1], 1);
  respawn(&fallingButtonList[2], 2);

  fallingButtonList[1].y -= buttonWidth * 2;
  fallingButtonList[2].y -= buttonWidth * 4;

  ChangeDirectory("assets");
  char *fileData = LoadFileText("savedata/highScore.sv");
  highScore = atoi(fileData);
  score = 0;

  goodSound = LoadSound("good.wav");
}

void update()
{

  if (currentState == GAME)
    for(int i = 0; i < 3; i++)
    {
      fallingButtonList[i].y += PHONE_HEIGHT / GetFPS() / SECONDS_TO_FALL;
      if (fallingButtonList[i].y >= PHONE_HEIGHT / 10 * 9)
      {
        respawn(&fallingButtonList[i], i);
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
        PlaySound(goodSound);
      }
      else 
      {
        pieceScore = -300;
      }

      score += pieceScore;
      fallingButtonList[i].action(&fallingButtonList[i], i);
    }
  }
  if (isButtonPressed(buttonList[4])){
    buttonList[4].action(PAUSED);

    if (score > highScore)
    {
      char scoreStr[6];
      snprintf(scoreStr, 6, "%d", score);
      SaveFileText("savedata/highScore.sv", scoreStr);
    }
  }

  if (score > highScore) 
    scoreColor = DARKGREEN;
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
    DrawText("Play", buttonList[3].x, buttonList[3].y, buttonList[3].h / 3, BLACK);
    EndDrawing();
    return;
  }


  if (selectedID == -1)
    ClearBackground(RAYWHITE);
  if (selectedID == buttonList[0].tag)
      ClearBackground(RED);
  if (selectedID == buttonList[1].tag)
      ClearBackground(BLUE);
  if (selectedID == buttonList[2].tag)
      ClearBackground(GREEN);

  for(int i = 0; i < 3; i++)
  {
    int pieceScore = 100 - fallingButtonList[i].y / (int)(PHONE_HEIGHT / 100);
    DrawText(TextFormat("%d", pieceScore),
             fallingButtonList[i].x, fallingButtonList[i].y - fallingButtonList[i].h / 3 , fallingButtonList[i].h / 4, BLACK);
    drawCustomText(fallingButtonList[i].text, fallingButtonList[i].x, fallingButtonList[i].y, fallingButtonList[i].h / 4);
  }

  DrawText(TextFormat("%d", score), 0, 0, PHONE_WIDTH / 8, scoreColor);

  //bottom bar
  DrawRectangle(0, PHONE_HEIGHT / 10 * 9, PHONE_WIDTH, PHONE_HEIGHT / 9.5, LIGHTGRAY);

  //State buttons
  drawButton(buttonList[0], RED);
  drawCustomText(listOpere[buttonList[0].tag].titlu, buttonList[0].x, buttonList[0].y, buttonList[0].h / 4);
  drawButton(buttonList[1], BLUE);
  drawCustomText(listOpere[buttonList[1].tag].titlu, buttonList[1].x, buttonList[1].y, buttonList[1].h / 4);
  drawButton(buttonList[2], GREEN);
  drawCustomText(listOpere[buttonList[2].tag].titlu, buttonList[2].x, buttonList[2].y, buttonList[2].h / 4);

  drawButton(buttonList[4], YELLOW);

  if (currentState == PAUSED)
    DrawText("Paused", PHONE_WIDTH / 4, PHONE_HEIGHT / 2 - PHONE_WIDTH / 24, PHONE_WIDTH / 12, BLACK);

  EndDrawing();
}

void quit()
{
  UnloadSound(goodSound);
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
void respawn(Button *btn, int i)
{
  int column = rand() % COLUMN_NR;
  while (lastSpawnX == column) column = rand() % COLUMN_NR;
  lastSpawnX = column;
  btn->x = column * PHONE_WIDTH / COLUMN_NR;
  btn->y = -PHONE_WIDTH / COLUMN_NR;
  setButtonTag(btn, buttonList[rand() % 3].tag);

  int state = -1;
  switch (i) {
    case 0:
      FallingButton0TextId += FallingButton0TextId > 6 ? -7 : 1;
      state = FallingButton0TextId;
      break;
    case 1:
      FallingButton1TextId += FallingButton1TextId > 6 ? -7 : 1;
      state = FallingButton1TextId;
      break;
    case 2:
      FallingButton2TextId += FallingButton2TextId > 6 ? -7 : 1;
      state = FallingButton2TextId;
      break;
  }
  switch (state) {
    case 0:
      btn->text = listOpere[btn->tag].anAparitie;
      break;
    case 1:
      btn->text = listOpere[btn->tag].autor;
      break;
    case 2:
      btn->text = listOpere[btn->tag].curentLiterar;
      break;
    case 3:
      btn->text = listOpere[btn->tag].temaOperei;
      break;
    case 4:
      btn->text = listOpere[btn->tag].elemStr;
      break;
    case 5:
      btn->text = listOpere[btn->tag].scena1;
      break;
    case 6:
      btn->text = listOpere[btn->tag].scena2;
      break;
  }
}

void drawCustomText(char *text, int x, int y, int fontSize)
{
  char textCopy[120];
  strcpy(textCopy, text);

  char *token = strtok(textCopy, " ");
  int row = 0;
  while (token)
  {
    DrawText(token, x, y + row * fontSize, fontSize, BLACK);
    int length = strlen(token);
    if (length < 4)
    {
      token = strtok(NULL, " ");
      int charWidth = fontSize / 9 * 7;
      DrawText(token, x + charWidth * (length + 1), y + row * fontSize, fontSize, BLACK);
    }
    token = strtok(NULL, " ");
    row++;
  }
}
