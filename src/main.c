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

int score;
int highScore;
int timer;
int timerStart;
int maxTime = 5;

enum GameState {
  MAIN_MENU,
  GAME,
  PAUSED
};

enum GameState currentState;
Sound goodSound;
Sound badSound;

Texture2D backgroundTexture;
Texture2D rectTexture;
Texture2D cardTexture;

Lecture listLectures[15];

int firstLecture;
int secondLecture;
int thirdLecture;

int selectedInfo;

void init();
void update();
void render();
void quit();

int main()
{
  init();

  while (!WindowShouldClose())
  {
    update();
    render();
  }

  quit();

  return 0;
}

void init()
{
  srand(time(NULL));

  InitWindow(PHONE_WIDTH, PHONE_HEIGHT, "Bac Romana");
  ChangeDirectory("assets");
  textFont = LoadFont("fonts/0xProtoNerdFontMono-Regular.ttf");
  SetTextureFilter(textFont.texture, TEXTURE_FILTER_TRILINEAR);

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
  timer = 10;

  startButton = createButton(PHONE_WIDTH / 4, PHONE_HEIGHT / 2 - PHONE_WIDTH / 8, PHONE_WIDTH / 2, PHONE_WIDTH / 4);
  pauseButton = createButton(PHONE_WIDTH / 12 * 11, 0, PHONE_WIDTH / 12, PHONE_WIDTH / 12);

  card.width = PHONE_WIDTH / 1.5;
  card.height = card.width / 2 * 3;
  card.x = (PHONE_WIDTH - card.width) / 2;
  card.y = (PHONE_HEIGHT - card.height) / 2;

  char *fileData = LoadFileText("savedata/highScore.sv");
  highScore = atoi(fileData);
  score = 0;

  goodSound = LoadSound("sounds/good.wav");
  badSound  = LoadSound("sounds/bad.wav");

  backgroundTexture = LoadTexture("img/bg.png");
  rectTexture       = LoadTexture("img/rect.png");
  cardTexture       = LoadTexture("img/card.png");
}

int swipeDistanceX = 0;
int swipeDistanceY = 0;
int isScreenTouched();
void update()
{

  /*int deltaTime = GetFrameTime();*/

  switch (currentState) 
  {
    case GAME:
      if (isButtonPressed(pauseButton))
        currentState = PAUSED;

      timer = GetTime() - timerStart;

      if (timer >= maxTime)
      {
        score -= 10;
        PlaySound(badSound);
        timerStart = GetTime();
        selectedInfo = (rand() % 3 + 1) * 10 + rand() % 7 + 1; 
        swipeDistanceX = 0;
        swipeDistanceY = 0;

        break;
      }

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
          answerPoints = maxTime - timer;
      } else 
      if (selectedInfo / 10 == 3) 
        answerPoints = maxTime - timer;

      if (answerPoints > 0)
        PlaySound(goodSound);
      else
        PlaySound(badSound);

      score += answerPoints;
      timerStart = GetTime();
      selectedInfo = (rand() % 3 + 1) * 10 + rand() % 7 + 1; 
      swipeDistanceX = 0;
      swipeDistanceY = 0;

      break;
    case PAUSED:

      if (!isScreenTouched()) break;

      currentState = GAME;
      timerStart = GetTime() - timer;

      if (score > highScore)
      {
        char scoreStr[6];
        snprintf(scoreStr, 6, "%d", score);
        SaveFileText("savedata/highScore.sv", scoreStr);
      }

      break;
    case MAIN_MENU:
      if (isScreenTouched() && isButtonPressed(startButton))
      {
        currentState = GAME;
        timerStart = GetTime();
      }
      break;
  }

}

void drawWrapedText(char *text, int x, int y, int w, int fontSize);
void drawTextInMiddleWrapped(char *text, int x, int y, int w, int fontSize);
void renderCard();
void render()
{
  BeginDrawing();
  ClearBackground(RAYWHITE);

  Rectangle sourceRec = {0, 0, backgroundTexture.width, backgroundTexture.height};
  Rectangle destRec   = {0, 0, PHONE_WIDTH, PHONE_HEIGHT};
  DrawTexturePro(backgroundTexture, sourceRec, destRec, (Vector2){0, 0}, 0, WHITE);

  if (currentState == MAIN_MENU)
  {
    drawButtonTexture(startButton, &rectTexture);
    drawTextInMiddleWrapped("Play", startButton.x, startButton.y + startButton.h / 3, startButton.w, startButton.h / 3);
    EndDrawing();
    return;
  }

  renderCard();

  char *lectureTitle = listLectures[firstLecture].titlu;
  int titleFontSize = PHONE_WIDTH / 12;

  for(int i = 0; i < strlen(lectureTitle); i++)
  {
    char text[2];
    text[0] = listLectures[firstLecture].titlu[i];
    text[1] = '\0';

    Vector2 titleCharSize = MeasureTextEx(textFont, text, titleFontSize, 1);

    DrawTextEx(textFont, text, (Vector2){(card.x - titleCharSize.x) / 2, card.y + card.height / 2 - strlen(lectureTitle) * titleFontSize / 2 + titleFontSize * i}, titleFontSize, 1,  WHITE);

  }

  lectureTitle = listLectures[secondLecture].titlu;
  for(int i = 0; i < strlen(lectureTitle); i++)
  {
    char text[2];
    text[0] = listLectures[secondLecture].titlu[i];
    text[1] = '\0';

    Vector2 titleCharSize = MeasureTextEx(textFont, text, titleFontSize, 1);

    DrawTextEx(textFont, text, (Vector2){card.x + card.width + (PHONE_WIDTH - card.x - card.width) / 2 - titleCharSize.x / 2, card.y + card.height / 2 - strlen(lectureTitle) * titleFontSize / 2 + titleFontSize * i}, titleFontSize, 1, WHITE);

  }

  drawTextInMiddleWrapped(listLectures[thirdLecture] .titlu, card.x, PHONE_HEIGHT / 35 * 33, card.width, titleFontSize);

  char timeStr[6];
  snprintf(timeStr, 6, "%d", maxTime - timer);
  drawTextInMiddleWrapped(timeStr, card.x, PHONE_HEIGHT / 8, card.width, PHONE_WIDTH / 12);

  drawButton(pauseButton, YELLOW);

  char scoreStr[6];
  snprintf(scoreStr, 6, "%d", score);

  drawTextInMiddleWrapped(scoreStr, card.x, PHONE_HEIGHT / 20, card.width, PHONE_WIDTH / 6);
  if (currentState == PAUSED)
  {
    DrawRectangle(0, 0, PHONE_WIDTH, PHONE_HEIGHT, (Color){0x00, 0x00, 0x00, 0xAA});
    drawTextInMiddleWrapped("Paused", card.x, PHONE_HEIGHT / 2 - PHONE_WIDTH / 24, card.width, PHONE_WIDTH / 12);
  }

  EndDrawing();
}

void quit()
{
  UnloadSound(goodSound);
  UnloadSound(badSound);
  UnloadFont(textFont);
  UnloadTexture(backgroundTexture);
  UnloadTexture(rectTexture);
  UnloadTexture(cardTexture);

  if (score > highScore)
  {
    char scoreStr[6];
    snprintf(scoreStr, 6, "%d", score);
    SaveFileText("savedata/highScore.sv", scoreStr);
  }

  CloseAudioDevice();
  CloseWindow();
}

void renderCard()
{
  Rectangle sourceRec = {0, 0, cardTexture.width, cardTexture.height};
  DrawTexturePro(cardTexture, sourceRec, card, (Vector2){0, 0}, 0, WHITE);

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

  drawWrapedText(infoText, card.x + card.width / 12, card.y + card.width / 12, card.width / 12 * 10.5, PHONE_WIDTH / 16);

}

void drawWrapedText(char *text, int x, int y, int w, int fontSize)
{

  if (strlen(text) <= 0) return;

  int row       = 0;
  int nextX     = x;

  Vector2 spaceSize = MeasureTextEx(textFont, " ", fontSize, 1);

  char textCopy[120] = {'\0'};
  strcpy(textCopy, text);

  char *currentWord = strtok(textCopy, " ");
  while (currentWord != NULL)
  {
    /*strcat(currentWord, " ");*/
    Vector2 wordSize = MeasureTextEx(textFont, currentWord, fontSize, 1);
    if (nextX + wordSize.x > x + w && nextX != 0) //wrap the text
    {
      nextX = x;
      row++;
    }
    DrawTextEx(textFont, currentWord, (Vector2){nextX, y + row * fontSize}, fontSize, 1, WHITE);
    nextX += wordSize.x + spaceSize.x;
    currentWord = strtok(NULL, " ");
  }
}

void drawTextInMiddleWrapped(char *text, int x, int y, int w, int fontSize)
{
  if (strlen(text) <= 0) return;

  int row = 0;

  char textCopy[120];
  strcpy(textCopy, text);

  char *nextWord = strtok(textCopy, " ");
  char textToDraw[120] = "";

  Vector2 nextWordSize   = MeasureTextEx(textFont, nextWord  , fontSize, 1);
  Vector2 textToDrawSize = MeasureTextEx(textFont, textToDraw, fontSize, 1);
  do
  {
    while (nextWord && textToDrawSize.x + nextWordSize.x <= w) {
      strcat(textToDraw, nextWord);
      nextWord = strtok(NULL, " ");
      if (nextWord) strcat(textToDraw, " ");
      textToDrawSize = MeasureTextEx(textFont, textToDraw, fontSize, 1);
      nextWordSize   = MeasureTextEx(textFont, nextWord  , fontSize, 1);
    }

    DrawTextEx(textFont, textToDraw, (Vector2){x + w / 2 - textToDrawSize.x / 2, y + row * fontSize}, fontSize, 1, WHITE);
    row++;

    if (nextWord == NULL) continue;

    strcpy(textToDraw, "");
    textToDrawSize = MeasureTextEx(textFont, textToDraw, fontSize, 1);

  } while(nextWord);
}

int isScreenTouched()
{
  return GetTouchPointCount() != 0 || IsMouseButtonDown(0);
}
