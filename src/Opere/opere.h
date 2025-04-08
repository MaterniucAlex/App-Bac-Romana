#pragma once
#ifndef OPERE 
#define OPERE

typedef struct {
  char *titlu;
  char *anAparitie;
  char *autor;
  char *curentLiterar;
  char *temaOperei;
  char *elemStr;
  char *scena1;
  char *scena2;
} Opera;

void initOpere(Opera *listOpere);

#endif
