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

//for swipe motion logic
int startSwipeX = 0;
int startSwipeY = 0;

Button startButton;
Button pauseButton;

Rectangle card;

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
Sound badSound;

Lecture listLectures[15];

int firstLecture;
int secondLecture;
int thirdLecture;

int selectedInfo;

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

   firstLecture = rand() % (15 / 3) + 1;
  secondLecture = rand() % ((15 / 3) * 2 - firstLecture) + firstLecture + 1;
   thirdLecture = rand() % (15 - secondLecture) + secondLecture;

  //this is to prevent the edge case where all 3 select the lecture with id 0
   firstLecture--;
  secondLecture--;

  selectedInfo = (rand() % 3 + 1) * 10 + rand() % 7 + 1; 

  startButton = createButton(PHONE_WIDTH / 4, PHONE_HEIGHT / 2 - PHONE_WIDTH / 8, PHONE_WIDTH / 2, PHONE_WIDTH / 4);
  pauseButton = createButton(PHONE_WIDTH / 12 * 11, 0, PHONE_WIDTH / 12, PHONE_WIDTH / 12);

  card.width = PHONE_WIDTH / 1.5;
  card.height = PHONE_HEIGHT / 1.5;
  card.x = (PHONE_WIDTH - card.width) / 2;
  card.y = (PHONE_HEIGHT - card.height) / 2;

  char *fileData = LoadFileText("savedata/highScore.sv");
  highScore = atoi(fileData);
  /*highScore = 0;*/
  score = 0;

  goodSound = LoadSound("sounds/good.wav");
  badSound  = LoadSound("sounds/bad.wav");
}

int swipeDistanceX = 0;
int swipeDistanceY = 0;
int isScreenTouched();
void update()
{

  switch (currentState) 
  {
    case GAME:
      if (isButtonPressed(pauseButton))
        currentState = PAUSED;

      if (isScreenTouched() && (startSwipeX == 0 && startSwipeY == 0))
      {
        startSwipeX = GetTouchX();
        startSwipeY = GetTouchY();
      }

      if (!isScreenTouched() && (startSwipeY != 0 || startSwipeX != 0))
      {
        swipeDistanceX = GetTouchX() - startSwipeX;
        swipeDistanceY = GetTouchY() - startSwipeY;
        startSwipeX = 0;
        startSwipeY = 0;
      }


      //cancel swipe action if swipe was too small or if the user swiped up
      if (abs(swipeDistanceX) < PHONE_WIDTH / 10 && swipeDistanceY < PHONE_WIDTH / 10) break;

      int answerPoints = -10;

      //see which swipe was bigger and use that
      if (abs(swipeDistanceX) > abs(swipeDistanceY))
      {
        //selectedInfo structure : 2 digit number
        //1st digit : the side where you would have to swipe to for a corrent answer
        //1 = left ; 2 = right ; 3 = down 
        //2nd digit : the random information shown from the lecture. See the renderCard function bellow.

        if ((swipeDistanceX > 0 && selectedInfo / 10 == 2) || (swipeDistanceX < 0 && selectedInfo / 10 == 1)) 
          answerPoints = 10;
      } else 
      if (selectedInfo / 10 == 3) 
        answerPoints = 10;

      if (answerPoints > 0)
        PlaySound(goodSound);
      else
       PlaySound(badSound);

      score += answerPoints;
      selectedInfo = (rand() % 3 + 1) * 10 + rand() % 7 + 1; 
      swipeDistanceX = 0;
      swipeDistanceY = 0;

      break;
    case PAUSED:

      if (isScreenTouched())
        currentState = GAME;

      if (score > highScore)
      {
        char scoreStr[6];
        snprintf(scoreStr, 6, "%d", score);
        SaveFileText("savedata/highScore.sv", scoreStr);
      }
      break;
    case MAIN_MENU:
      if (isScreenTouched() && isButtonPressed(startButton))
        currentState = GAME;
      break;
  }

  if (score > highScore) 
    scoreColor = DARKGREEN;
  else 
    scoreColor = BLACK;
}

void renderCard();
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

  renderCard();

  drawWrapedText(listLectures[firstLecture] .titlu, PHONE_WIDTH / 100, PHONE_HEIGHT / 2, PHONE_WIDTH / 5, PHONE_HEIGHT / 50);
  drawWrapedText(listLectures[secondLecture].titlu, PHONE_WIDTH / 100 * 90, PHONE_HEIGHT / 2, PHONE_WIDTH / 5, PHONE_HEIGHT / 50);
  drawWrapedText(listLectures[thirdLecture] .titlu, PHONE_WIDTH / 2, PHONE_HEIGHT / 35 * 33, PHONE_WIDTH / 5, PHONE_HEIGHT / 50);

  drawButton(pauseButton, YELLOW);
  DrawText(TextFormat("%d", score), PHONE_WIDTH / 2, PHONE_HEIGHT / 20, PHONE_WIDTH / 8, scoreColor);
  if (currentState == PAUSED)
    DrawText("Paused", PHONE_WIDTH / 4, PHONE_HEIGHT / 2 - PHONE_WIDTH / 24, PHONE_WIDTH / 12, BLACK);

  EndDrawing();
}

void quit()
{
  UnloadSound(goodSound);
  UnloadSound(badSound);
  UnloadFont(textFont);

  if (score > highScore)
  {
    char scoreStr[6];
    snprintf(scoreStr, 6, "%d", score);
    SaveFileText("savedata/highScore.sv", scoreStr);
  }
}

void renderCard()
{
  DrawRectangleRec(card, GRAY);

  int lectureID;
  switch (selectedInfo / 10)
  {
    case 1:
    default:
      lectureID = firstLecture;
      break;
    case 2:
      lectureID = secondLecture;
      break;
    case 3:
      lectureID = thirdLecture;
      break;
  }

  char *infoText;
  switch (selectedInfo % 10)
  {
    case 1:
    default:
      infoText = listLectures[lectureID].anAparitie;
      break;
    case 2:
      infoText = listLectures[lectureID].autor;
      break;
    case 3:
      infoText = listLectures[lectureID].curentLiterar;
      break;
    case 4:
      infoText = listLectures[lectureID].temaOperei;
      break;
    case 5:
      infoText = listLectures[lectureID].elemStr;
      break;
    case 6:
      infoText = listLectures[lectureID].scena1;
      break;
    case 7:
      infoText = listLectures[lectureID].scena2;
      break;
  }

  drawWrapedText(infoText, card.x + card.width / 20, card.y + card.height / 2, card.width / 20 * 18, PHONE_WIDTH / 24);

}

void drawWrapedText(char *text, int x, int y, int w, int fontSize)
{

  if (strlen(text) <= 0) return;

  int charWidth = fontSize / 16 * 11; //rough aproximation
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
    DrawTextEx(textFont, currentWord, (Vector2){nextX, y + row * fontSize}, fontSize, 1, BLACK);
    nextX += strlen(currentWord) * charWidth;
    currentWord = strtok(NULL, " ");
  }
}

int isScreenTouched()
{
  return GetTouchPointCount() != 0 || IsMouseButtonDown(0);
}
