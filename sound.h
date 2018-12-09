#ifndef __Sound__
#define __Sound__

#include <iostream>
#include <stdio.h>
#include "al.h"
#include "alc.h"


extern int play(int, char **, char* filepath);
extern int playApplause(int, char **, char* filepath);
extern int playTurnback(int, char **, char* filepath);
extern void stopPlaying();
extern int playBoo(int, char **, char* filepath);




#endif 