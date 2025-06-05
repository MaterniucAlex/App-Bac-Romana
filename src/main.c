#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "Button/Button.h"
#include "Button/ToggleButton.h"
#include "Lectures/lectures.h"

/*
 * Before anyone else tries to read this code I must say I apologise
 * This could have been written much better if I had a structure in mind in the first place
 * Or if I didn't work on this project exclusively at 1AM
 *
 * Either way, this code is horendous, I am ashamed of it, but it works
 * The logic is pretty alright, if you can ignore the awful structure of it 
 *
 * I may revisit this at some point, but we may see if that happens
 *
*/

#ifdef PHONE 
int PHONE_WIDTH  = 1080;
int PHONE_HEIGHT = 2340;
#else
int PHONE_WIDTH  = 1080 / 2.5;
int PHONE_HEIGHT = 2340 / 2.5;
#endif

Font textFont;

//for swipe motion logic
int startSwipeX = 0;
int startSwipeY = 0;

int selectableLectures = 11;
int selectableInfo = 4;

Button startButton;
Button optionsButton;
Button mainMenuButton;
Button exitButton;

ToggleButton lecturesButtonList[15];

Lecture listLectures[15];

int firstLecture;
int secondLecture;
int thirdLecture;

int selectedInfo;

Rectangle card;

int score;
int highScore;
int timer;
int timerStart;
int maxTime = 5;

enum GameState {
  MAIN_MENU,
  OPTIONS,
  GAME,
};

enum GameState currentState;
Sound goodSound;
Sound badSound;

Texture2D backgroundTexture;
Texture2D rectTexture;
Texture2D cardTexture;
Texture2D exitTexture;

Color overlayColor = (Color){0x00, 0xBB, 0x00, 0x00};


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
  textFont = LoadFont("fonts/font.ttf");
  SetTextureFilter(textFont.texture, TEXTURE_FILTER_POINT);

  InitAudioDevice();
  SetTargetFPS(60);
  currentState = MAIN_MENU;

  initLectures(listLectures);

  startButton   = createButton(PHONE_WIDTH / 4, PHONE_HEIGHT / 2 - PHONE_WIDTH / 8, PHONE_WIDTH / 2, PHONE_WIDTH / 4);
  optionsButton = createButton(PHONE_WIDTH / 4, PHONE_HEIGHT / 2 + PHONE_WIDTH / 3 - PHONE_WIDTH / 8, PHONE_WIDTH / 2, PHONE_WIDTH / 4);
  mainMenuButton= createButton(PHONE_WIDTH / 4, PHONE_HEIGHT / 4 * 3 + PHONE_WIDTH / 3 - PHONE_WIDTH / 8, PHONE_WIDTH / 2, PHONE_WIDTH / 6);
  exitButton   = createButton(PHONE_WIDTH / 36 * 35 - PHONE_WIDTH / 12, PHONE_WIDTH / 36, PHONE_WIDTH / 12, PHONE_WIDTH / 12);

  for(int i = 0; i < 15; i++)
  {
    lecturesButtonList[i] = createToggleButton(PHONE_WIDTH / 20, PHONE_WIDTH / 20 * (1 + i) + PHONE_WIDTH / 21 * i, PHONE_WIDTH / 14, PHONE_WIDTH / 14);
  }
  lecturesButtonList[0].isToggled = 1;
  lecturesButtonList[1].isToggled = 1;
  lecturesButtonList[2].isToggled = 1;

  card.width = PHONE_WIDTH / 1.5;
  card.height = card.width / 3 * 4;
  card.x = (PHONE_WIDTH - card.width) / 2;
  card.y = (PHONE_HEIGHT - card.height) / 4 * 3;

  char *fileData = LoadFileText("savedata/highScore.sv");
  highScore = atoi(fileData);
  score = 0;

  goodSound = LoadSound("sounds/good.wav");
  badSound  = LoadSound("sounds/bad.wav");

  backgroundTexture = LoadTexture("img/bg.png");
  rectTexture       = LoadTexture("img/rect.png");
  cardTexture       = LoadTexture("img/card.png");
  exitTexture      = LoadTexture("img/pause.png");
}

int swipeDistanceX = 0;
int swipeDistanceY = 0;
int isScreenTouched();

int selectLectures()
{

  selectableLectures = 0;
  for(int i = 0; i < 15; i++)
    if (lecturesButtonList[i].isToggled) selectableLectures++;

  if (selectableLectures < 3) return 0;

  int  firstLectureId = rand() % (selectableLectures / 3) + 1;
  int secondLectureId = rand() % ((selectableLectures / 3) * 2 - firstLectureId) + firstLectureId + 1;
  int  thirdLectureId = rand() % (selectableLectures - secondLectureId) + secondLectureId;

  //this is to prevent the edge case where all 3 select the lecture with id 0
  firstLectureId--;
  secondLectureId--;

  for(int i = 0, k = -1; i < 15; i++)
  {
    if (lecturesButtonList[i].isToggled) k++;
    if (k == firstLectureId)   firstLecture = i;
    if (k == secondLectureId) secondLecture = i;
    if (k == thirdLectureId){  thirdLecture = i; i = 15;}
  }

  return 1;
}

void update()
{

  /*int deltaTime = GetFrameTime();*/

  switch (currentState) 
  {
    default: break;
    case MAIN_MENU:
      if (isButtonPressed(&startButton))
      {

        if (selectLectures() == 0) 
        {
          PlaySound(badSound);
          break;
        }

        selectedInfo = (rand() % 3 + 1) * 10 + rand() % 7 + 1; 
        timer = 5;

        score = 0;

        currentState = GAME;
        timerStart = GetTime();
      }
      if (isButtonPressed(&optionsButton))
      {
        currentState = OPTIONS;
      }
      break;
    case OPTIONS:
      if (isButtonPressed(&mainMenuButton))
      {
        currentState = MAIN_MENU;
      }
      for(int i = 0; i < 15; i++)
      {
        isToggleButtonPressed(&lecturesButtonList[i]);
      }
      break;
    case GAME:
      if (isButtonPressed(&exitButton))
      {
        currentState = MAIN_MENU;
        startSwipeX = 0;
        startSwipeY = 0;
        break;
      }

      if (maxTime - timer != maxTime - (int)GetTime() + timerStart && maxTime - (int)GetTime() + timerStart != 5)
      {
        overlayColor.a = 0x00;
      }
      timer = GetTime() - timerStart;

      if (timer >= maxTime)
      {
        score -= 10;
        PlaySound(badSound);
        timerStart = GetTime();
        selectedInfo = (rand() % 3 + 1) * 10 + rand() % 7 + 1; 
        startSwipeY = 0;
        startSwipeX = 0;
        swipeDistanceX = 0;
        swipeDistanceY = 0;
        overlayColor = (Color){0xBB, 0x00, 0x00, 0x77};

        break;
      }


      if (isScreenTouched() && (startSwipeX == 0 && startSwipeY == 0) && (
        GetTouchX() >= card.x && GetTouchX() <= card.x + card.width &&
        GetTouchY() >= card.y && GetTouchY() <= card.y + card.height))
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
        //2nd digit : the random information shown from the lecture. See the drawCard function bellow.

        if ((swipeDistanceX > 0 && selectedInfo / 10 == 2) || (swipeDistanceX < 0 && selectedInfo / 10 == 1)) 
          answerPoints = maxTime - timer;
      } else 
      if (selectedInfo / 10 == 3) 
        answerPoints = maxTime - timer;

      if (answerPoints > 0)
      {
        overlayColor = (Color){0x00, 0xBD, 0x00, 0x69};
        PlaySound(goodSound);
      }
      else
      {
        overlayColor = (Color){0xBB, 0x00, 0x00, 0x77};
        PlaySound(badSound);
      }

      score += answerPoints;
      timerStart = GetTime();
      selectedInfo = (rand() % 3 + 1) * 10 + rand() % selectableInfo + 1; 
      swipeDistanceX = 0;
      swipeDistanceY = 0;

      break;
  }

}

void drawWrapedText(char *text, int x, int y, int w, int fontSize);
void drawCardInfoText(char *text, Rectangle cardDrawRect, int fontSize, int rotation);
void drawTextInMiddleWrapped(char *text, int x, int y, int w, int fontSize);
void drawLecturesSideText();
void drawCard();
void render()
{
  BeginDrawing();
  ClearBackground(RAYWHITE);

  Rectangle sourceRec = {0, 0, backgroundTexture.width, backgroundTexture.height};
  Rectangle destRec   = {0, 0, PHONE_WIDTH, PHONE_HEIGHT};
  DrawTexturePro(backgroundTexture, sourceRec, destRec, (Vector2){0, 0}, 0, WHITE);

  switch (currentState) {

    case MAIN_MENU:
      drawButtonTexture(startButton, &rectTexture);
      drawTextInMiddleWrapped("Play", startButton.x, startButton.y + startButton.h / 3, startButton.w, startButton.h / 3);
      drawButtonTexture(optionsButton, &rectTexture);
      drawTextInMiddleWrapped("Options", optionsButton.x, optionsButton.y + optionsButton.h / 3, optionsButton.w, optionsButton.h / 3);
      break;
    case OPTIONS:
      drawButtonTexture(mainMenuButton, &rectTexture);
      drawTextInMiddleWrapped("Main Menu", mainMenuButton.x, mainMenuButton.y + mainMenuButton.h / 3, mainMenuButton.w, mainMenuButton.h / 3);

      for(int i = 0; i < 15; i++)
      {
        drawToggleButtonTexture(lecturesButtonList[i], &rectTexture);
        DrawTextPro(textFont, listLectures[i].titlu,
                    (Vector2){lecturesButtonList[i].x + lecturesButtonList[i].w / 3 * 4, lecturesButtonList[i].y},
                    (Vector2){0, 0}, 0, lecturesButtonList[i].h, 1, WHITE);
      }

      break;
    case GAME:
      drawCard();

      drawLecturesSideText();

      //time
      char timeStr[6];
      snprintf(timeStr, 6, "%d", maxTime - timer);
      drawTextInMiddleWrapped(timeStr, card.x, PHONE_HEIGHT / 8, card.width, PHONE_WIDTH / 12);

      //score
      char scoreStr[6];
      snprintf(scoreStr, 6, "%d", score);
      drawTextInMiddleWrapped(scoreStr, card.x, PHONE_HEIGHT / 20, card.width, PHONE_WIDTH / 6);

      drawButtonTexture(exitButton, &exitTexture);

      if (overlayColor.a != 0x00)
      {
        DrawRectangle(0, 0, PHONE_WIDTH, PHONE_HEIGHT, overlayColor);
      }
  }
  EndDrawing();
}

void drawCard()
{
  Rectangle sourceRec = {0, 0, cardTexture.width, cardTexture.height};
  Rectangle cardDrawRect = {card.x + card.width / 2, card.y + card.height / 2, card.width, card.height};

  int cardRotation = 0;
  if (startSwipeX >= card.x && startSwipeX <= card.x + card.width 
  &&  startSwipeY >= card.y && startSwipeY <= card.y + card.height)
  {
    cardDrawRect.x = card.x - (startSwipeX - GetTouchX()) * 2 + card.width / 2;
    cardDrawRect.y = card.y - (startSwipeY - GetTouchY()) * 2 + card.height / 2;
    cardRotation = (GetTouchX() - startSwipeX) / 6;
  }

  DrawTexturePro(cardTexture, sourceRec, card, (Vector2){0, 0}, 0, GRAY);
  DrawTexturePro(cardTexture, sourceRec, cardDrawRect, (Vector2){card.width / 2, card.height / 2}, cardRotation, WHITE);

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

  drawCardInfoText(infoText, cardDrawRect, PHONE_WIDTH / 16, cardRotation);

}

void drawLecturesSideText()
{
  char *lectureTitle = listLectures[firstLecture].titlu;
  int titleFontSize = PHONE_WIDTH / 12;

  for(int i = 0; i < strlen(lectureTitle); i++)
  {
    char text[2];
    text[0] = listLectures[firstLecture].titlu[i];
    text[1] = '\0';

    Vector2 titleCharSize = MeasureTextEx(textFont, text, titleFontSize, 1);

    DrawTextEx(textFont, text, (Vector2){(card.x - titleCharSize.x) / 2, card.y + card.height / 2 - strlen(lectureTitle) * titleFontSize / 2 + titleFontSize / 10 * 9 * i}, titleFontSize, 1,  WHITE);

  }

  lectureTitle = listLectures[secondLecture].titlu;
  for(int i = 0; i < strlen(lectureTitle); i++)
  {
    char text[2];
    text[0] = listLectures[secondLecture].titlu[i];
    text[1] = '\0';

    Vector2 titleCharSize = MeasureTextEx(textFont, text, titleFontSize, 1);

    DrawTextEx(textFont, text, (Vector2){card.x + card.width + (PHONE_WIDTH - card.x - card.width) / 2 - titleCharSize.x / 2, card.y + card.height / 2 - strlen(lectureTitle) * titleFontSize / 2 + titleFontSize / 10 * 9 * i}, titleFontSize, 1, WHITE);

  }

  drawTextInMiddleWrapped(listLectures[thirdLecture] .titlu, card.x, PHONE_HEIGHT / 35 * 33, card.width, titleFontSize);
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

void drawCardInfoText(char *text, Rectangle cardDrawRect, int fontSize, int rotation)
{

  if (strlen(text) <= 0) return;

  int nextX     = card.x + card.width / 12;

  char textCopy[120] = {'\0'};
  strcpy(textCopy, text);

  char word[20] = {'\0'};

  int wordNext = 0;
  for(int i = 0; i < strlen(textCopy); i++)
  {
    if (textCopy[i] != ' ')
    {
      word[wordNext] = textCopy[i];
      wordNext++;
      if (wordNext >= 20) wordNext = 0;
      continue;
    }

    Vector2 wordSize = MeasureTextEx(textFont, word, fontSize, 1);
    if (nextX + wordSize.x >= card.x + card.width - card.width / 12 && nextX != card.x + card.width / 12)
    {
      textCopy[i - strlen(word) - 1] = '\n';
      nextX = card.x + card.width / 12;
    }

    wordNext = 0;
    for(int i = 0; i < 20; word[i++] = '\0');

    nextX += wordSize.x;
  }

  Vector2 wordSize = MeasureTextEx(textFont, word, fontSize, 1);
  if (nextX + wordSize.x >= card.x + card.width - card.width / 12 && nextX != card.x + card.width / 12)
  {
    textCopy[strlen(textCopy) - strlen(word) - 1] = '\n';
    nextX = card.x + card.width / 12;
  }

  DrawTextPro(textFont, textCopy,
              (Vector2){cardDrawRect.x + cardDrawRect.width / 12, cardDrawRect.y + cardDrawRect.width / 12},
              (Vector2){cardDrawRect.width / 2, cardDrawRect.height / 2},
              rotation, fontSize, 1, WHITE);
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

void quit()
{
  UnloadSound(goodSound);
  UnloadSound(badSound);
  UnloadFont(textFont);
  UnloadTexture(backgroundTexture);
  UnloadTexture(rectTexture);
  UnloadTexture(cardTexture);
  UnloadTexture(exitTexture);

  if (score > highScore)
  {
    char scoreStr[6];
    snprintf(scoreStr, 6, "%d", score);
    SaveFileText("savedata/highScore.sv", scoreStr);
  }

  CloseAudioDevice();
  CloseWindow();
}

int isScreenTouched()
{
  return GetTouchPointCount() != 0 || IsMouseButtonDown(0);
}
