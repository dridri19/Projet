#ifndef PROJET_H_INCLUDED
#define PROJET_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <math.h>

#define NIMAGE 6
#define MAXI 100
#define NIMGMENU 8
#define NIMGPERSO 10
#define NIMGSON 4
#define min(a,b) (a<=b?a:b)

extern BITMAP *page;
extern BITMAP* niveau;
extern BITMAP* oiseau[NIMAGE];
extern BITMAP* fond;
extern BITMAP* collision;


#include "complement.h"
#include "perso.h"

#endif // PROJET_H_INCLUDED
