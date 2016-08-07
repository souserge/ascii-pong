#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <wincon.h>
#include <mmsystem.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include "dataControl.hpp"
#include "engine.hpp"
#include "graphics.hpp"
#include "inputHandle.hpp"
#include "options.hpp"
#include "networking.hpp"
#include "globVars.hpp"

int aiType,
    aiLevel,
    wait;
int platLen,
    scoreLimit,
    width,
    height,
    sounds;
double ballSpeed;
char colorBF[10];

int playMode,
    aiPlat1,
    aiPlat2,
    aiFun,
    fieldX,
    fieldY,
    fiXW,
    fiYH,
    WID,
    HEI;


void setAIType(int a) {aiType = a;}
void setAILevel(int a) {aiLevel = a;}
void setWait(int a) {wait = a;}
void setBallSpeed(double d) {ballSpeed = d;}
void setPlatLen(int a) {platLen = a;}
void setScoreLimit(int a) {scoreLimit = a;}

void setWidth(int a) {width = a;}
void setHeight(int a) {height = a;}
void setSounds(int a) {sounds = a;}
void setColorBF(char *col) {strcpy(colorBF, col);}

void setPlayMode(int a) {playMode = a;}
void setFieldX(int a) {fieldX = a;}
void setFieldY(int a) {fieldY = a;}
void setFiXW(int a) {fiXW = a;}
void setFiYH(int a) {fiYH= a;}

void setDefaults()
{
    aiType = 1;
    aiLevel = 1;
    wait = 16;
    ballSpeed = 1.75;
    platLen = 8;
    scoreLimit = 7;

    width = 97;
    height = 37;
    sounds = OFF;
    strcpy(colorBF, "3B");

    playMode = 0;
    aiPlat1 = 0;
    aiPlat2 = 0;
    aiFun = 0;
    fieldX = 3;
    fieldY = 2;
    fiXW = fieldX + width;
    fiYH = fieldY + height;
    WID = DEF_WID;
    HEI = DEF_HEI;
}
