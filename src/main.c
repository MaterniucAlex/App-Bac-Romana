#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "Button/Button.h"
#include "Lectures/lectures.h"

int PHONE_WIDTH  = 1080 / 2.5;
int PHONE_HEIGHT = 2412 / 2.5;

Font textFont;

int lastTouchX = 0;
int lastTouchY = 0;

Button startButton;
Button pauseButton;

Color scoreColor = BLACK;

int score;
int highScore;

enum GameState {
  MAIN_MENU,
  GAME,
  PAUSED
};

enum GameState currentState;
Sound goodSound;

Lecture listLectures[15];

int firstLecture;
int secondLecture;
int thirdLecture;

void init();
void update();
void render();
void quit();
void drawWrapedText(char *text, int x, int y, int w, int fontSize);

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
  ChangeDirectory("assets");
  textFont = LoadFont("fonts/0xProtoNerdFontMono-Regular.ttf");

  #ifdef PHONE
  MaximizeWindow();
  ToggleBorderlessWindowed();
  PHONE_WIDTH  = GetScreenWidth ();
  PHONE_HEIGHT = GetScreenHeight();
  #endif

  InitAudioDevice();
  SetTargetFPS(60);
  currentState = MAIN_MENU;

  initLectures(listLectures);

   firstLecture = rand() % (15 / 3);
  secondLecture = rand() % (15 / 3 * 2 -  firstLecture) + firstLecture;
   thirdLecture = rand() % (15 + 1     - secondLecture) + secondLecture;

  startButton = createButton(PHONE_WIDTH / 4, PHONE_HEIGHT / 2 - PHONE_WIDTH / 8, PHONE_WIDTH / 2, PHONE_WIDTH / 4);
  pauseButton = createButton(PHONE_WIDTH / 12 * 11, 0, PHONE_WIDTH / 12, PHONE_WIDTH / 12);
  
  /*char *fileData = LoadFileText("savedata/highScore.sv");*/
  /*highScore = atoi(fileData);*/
  highScore = 0;
  score = 0;

  goodSound = LoadSound("good.wav");
}

void update()
{

  if (currentState == GAME)

  //touch protection
  if (lastTouchX == GetTouchX() && lastTouchY == GetTouchY()) return;

  if (currentState == PAUSED) currentState = GAME;

  if (currentState == MAIN_MENU)
  {
    if (isButtonPressed(startButton)) currentState = GAME;
    return;
  }

  lastTouchX = GetTouchX();
  lastTouchY = GetTouchY();

  if (isButtonPressed(pauseButton)){
    currentState = PAUSED;

    //Save the score when paused
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
  ClearBackground(RAYWHITE);

  if (currentState == MAIN_MENU)
  {
    drawButton(startButton, GRAY);
    DrawText("Play", startButton.x, startButton.y, startButton.h / 3, BLACK);
    EndDrawing();
    return;
  }

  drawWrapedText(listLectures[firstLecture] .titlu, PHONE_WIDTH / 100, PHONE_HEIGHT / 2, PHONE_WIDTH / 5, PHONE_HEIGHT / 50);
  drawWrapedText(listLectures[secondLecture].titlu, PHONE_WIDTH / 100 * 90, PHONE_HEIGHT / 2, PHONE_WIDTH / 5, PHONE_HEIGHT / 50);
  drawWrapedText(listLectures[thirdLecture] .titlu, PHONE_WIDTH / 2, PHONE_HEIGHT / 35 * 33, PHONE_WIDTH / 5, PHONE_HEIGHT / 50);

  drawButton(pauseButton, YELLOW);
  DrawText(TextFormat("%d", score), 0, 0, PHONE_WIDTH / 8, scoreColor);
  if (currentState == PAUSED)
    DrawText("Paused", PHONE_WIDTH / 4, PHONE_HEIGHT / 2 - PHONE_WIDTH / 24, PHONE_WIDTH / 12, BLACK);

  EndDrawing();
}

void quit()
{
  UnloadSound(goodSound);
  UnloadFont(textFont);
}

void drawWrapedText(char *text, int x, int y, int w, int fontSize)
{

  if (strlen(text) <= 0) return;

  int charWidth = fontSize / 12 * 9; //rough aproximation
  int row       = 0;
  int nextX     = x;

  char textCopy[120] = {'\0'};
  strcpy(textCopy, text);

  char *currentWord = strtok(textCopy, " ");
  while (currentWord != NULL)
  {
    if (nextX + strlen(currentWord) * charWidth > x + w && nextX != 0) //wrap the text
    {
      nextX = x;
      row++;
    }
    DrawText(currentWord, nextX, y + row * fontSize, fontSize, BLACK);
    nextX += strlen(currentWord) * charWidth;
    currentWord = strtok(NULL, " ");
  }
}
