#pragma once
#ifndef LECTURE
#define LECTURE

typedef struct {
  char *titlu;
  char *anAparitie;
  char *autor;
  char *curentLiterar;
  char *temaOperei;
  char *elemStr;
  char *scena1;
  char *scena2;
} Lecture;

void initLectures(Lecture *listLectures);

#endif
