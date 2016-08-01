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


void updateColor()
{
    char color[128];
    sprintf(color, "COLOR %s", colorBF);
    system(color);
}

void updateField()
{
    if (width > DEF_WID-7)
    {
        WID = width+7;
    }
    if (height > DEF_HEI-5)
    {
        HEI = height+5;
    }
    if (width < DEF_WID-7)
    {
        WID = DEF_WID;
    }
    if (height < DEF_HEI-5)
    {
        HEI = DEF_HEI;
    }
    if (platLen > height-4)
    {
        platLen = height-4;
    }
    fieldX = (WID - width)/2;
    fieldY = (HEI - height)/2;
    fiXW = fieldX + width;
    fiYH = fieldY + height;
    char mode[128];
    sprintf(mode, "MODE CON: COLS=%d LINES=%d", WID, HEI);
    system(mode);
}

void updateSounds()
{
    static int prevState = OFF;
    if (sounds != prevState)
    {
        playWav("music.wav", 1);
        prevState = sounds;
    }
}

void updateAll()
{
    updateColor();
    updateSounds();
    updateField();
}



int changeInt(int xPos, int yPos, int low, int high, int &param)
{
    int num;
    char buffer[128];
    char dec[16] = "0123456789";
    gets(buffer);
    if (buffer[0] == '\0' || buffer[0] == '\n')
        return 2;

    int isInThere = 0;
    for (int i = 0; i < strlen(buffer); i++)
    {
        if (isInArray(buffer[i], dec, strlen(dec)))
        {
            isInThere = 1;
            break;
        }
    }
    clearRow(xPos, yPos, 40);
    if (!isInThere) {
        printAt(xPos, yPos, "NOT A NUMBER");
        getch();
        return 0;
    }
    num = atoi(buffer);
    if (num < low || num > high) {
        printAt(xPos, yPos, "OUT OF RANGE");
        getch();
        return 0;
    }
    param = num;
    return 1;
}

int changeState(int xPos, int yPos, int initState, char *statArr[], int len)
{
    int state = initState, action = 0, prevState = initState;

    printAt(xPos, yPos, statArr[state]);
    while (!(action = handleNav(state, len)))
        {
            if(state != prevState) {
                clearRow(xPos, yPos, 40);
                printAt(xPos, yPos, statArr[state]);
                prevState = state;
            }
            Sleep(40);
        }
        if (action == -1)
            return -1;
        else
            return state;
}



int changeAIType(int xPos, int yPos)
{
    char *statArr[32] = {"CRAZY_BALL", "THUNDER_BEAT"};
    int state;
    if((state = changeState(xPos, yPos, aiType, statArr, 2)) != -1)
        aiType = state;
    return 1;
}


int changeAILevel(int xPos, int yPos)
{
    char *statArr[32] = {"EASY", "MEDIUM", "HARD"};
    int state;
    if (state = changeState(xPos, yPos, aiLevel, statArr, 3) != -1)
        aiLevel = state;
    return 1;
}

int changeGameSpeed(int xPos, int yPos)
{
   // char *statArr[32] = {"IE", "PENTIUM 4", "HAYAI!!", "USAIN BOLT", "C++", "TACHYON"};
    int state;
    int val;
    if (!(val = changeInt(xPos, yPos, 0, 50, state)))
        return 0;
    if (val != 2)
        wait = 50 - state;
    return 1;
}

int changeBallSpeed(int xPos, int yPos)
{
    int state;
    int val;
    if (!(val = changeInt(xPos, yPos, 0, 50, state)))
        return 0;
    if (val != 2)
        ballSpeed = (double)state/25 + 0.5;
    return 1;
}

int changePlatLen(int xPos, int yPos)
{
    int num;
    int low = 1;
    int high = height - 4;
    char buffer[128];
    char dec[16] = "0123456789";
    gets(buffer);
    if (!strcmp(buffer, "AIFUN")){
        clearRow(xPos, yPos, 40);
        printAt(xPos, yPos, "HAVE FUN!");
        getch();
        aiFun = 1;
        return 1;
    }
    if (buffer[0] == '\0' || buffer[0] == '\n')
        return 1;

    int isInThere = 0;
    for (int i = 0; i < strlen(buffer); i++)
    {
        if (isInArray(buffer[i], dec, strlen(dec)))
        {
            isInThere = 1;
            break;
        }
    }
    clearRow(xPos, yPos, 40);
    if (!isInThere) {
        printAt(xPos, yPos, "NOT A NUMBER");
        getch();
        return 0;
    }
    num = atoi(buffer);
    if (num < low) {
        printAt(xPos, yPos, "HOW DO YOU IMAGINE THIS?");
        getch();
        return 0;
    }
    if (num > high) {
        printAt(xPos, yPos, "INCREASE THE FIELD HEIGHT FIRST");
        getch();
        return 0;
    }
    platLen = num;
    return 1;
}

int diffSetMenu()
{
    int state, prevState, action;

    char *aiTypeArr[64] = {"CRAZY_BALL", "THUNDER_BEAT"},
        *aiLevelArr[64] = {"EASY", "MEDIUM", "HARD"};
    char aiTypeChar[32],
        aiLevelChar[32],
        gameSpeedChar[32],
        ballSpeedChar[32],
        platLenChar[32],
        scoreLimChar[32];
    char newArr[6][128], *newA[128];
    char *valArr[64] = {aiTypeChar, aiLevelChar, gameSpeedChar, ballSpeedChar, platLenChar, scoreLimChar};

    char *navArr[64] = {"AI TYPE('W'/'S')", "AI LEVEL", "GAME SPEED(0-50)", "BALL SPEED(0-50)", "PLATFORM LENGTH(1-???)", "SCORE LIMIT(1-9)"};
    int navLen = 6, xPos, yPos;
    state = 0;
    prevState = 0;
    int isMenu = 1;
    while (isMenu)
    {
        action = 0;
        xPos = WID/2-7;
        yPos = HEI/10 + 16;


        itoa(50-wait, gameSpeedChar, 10);
        itoa(platLen, platLenChar, 10);
        itoa(scoreLimit, scoreLimChar, 10);
        itoa((double)(ballSpeed - 0.5)*25, ballSpeedChar, 10);
        strcpy(aiTypeChar, aiTypeArr[aiType]);
        strcpy(aiLevelChar, aiLevelArr[aiLevel]);
        for (int i = 0; i < navLen; i++)
        {
            newArr[i][0] = '\0';
            strcat(newArr[i], navArr[i]);
            strcat(newArr[i], ": ");
            strcat(newArr[i], valArr[i]);
            newA[i] = newArr[i];
        }

        system("cls");
        asciiPr("||options||", WID/2-34, HEI/10);
        printAt(xPos-4, yPos-2, "DIFFICULTY SETTINGS:");
        printNav(xPos, yPos, newA, navLen, state);
        while (!(action = handleNav(state, navLen)))
        {
            moveArrow(state, prevState, xPos-2, yPos);
            Sleep(40);
        }
        if (action == -1)
            return 1;

        int isNotSet = 1;
        while (isNotSet)
        {
            clearRow(xPos, yPos + 2*state, 40);
            printAt(xPos - 4, yPos + 2*state, navArr[state]);
            printf(": ");
            switch(state)
            {
            case 0:
                if (changeAIType(xPos - 2 + strlen(navArr[state]), yPos + 2*state))
                    isNotSet = 0;
                break;
            case 1:
                if (changeAILevel(xPos - 2 + strlen(navArr[state]), yPos + 2*state))
                    isNotSet = 0;
                break;
            case 2:
                if (changeGameSpeed(xPos - 4, yPos + 2*state))
                    isNotSet = 0;
                break;
            case 3:
                if (changeBallSpeed(xPos - 4, yPos + 2*state))
                    isNotSet = 0;
                break;
            case 4:
                if (changePlatLen(xPos - 4, yPos + 2*state))
                    isNotSet = 0;
                break;
            case 5:
                if (changeInt(xPos - 4, yPos + 2*state, 1, 9, scoreLimit))
                    isNotSet = 0;
                break;
            default:
                return 0;
            }
        }
    }
    return 1;
}



int changeSounds(int xPos, int yPos)
{
    char *statArr[32] = {"OFF", "ON"};
    int state = sounds;
    if ((state = changeState(xPos, yPos, sounds, statArr, 2)) != -1)
        sounds = state;
    return 1;
}

int checkColor(char *color)
{
    char hex[] = {"0123456789ABCDEF"};
    if (color[0] == '\n' || color[0] == '\0')
        return -1;
    if (strlen(color) > 2)
        return 0;
    else if (!isInArray(color[0], hex, strlen(hex)) || !isInArray(color[1], hex, strlen(hex)))
        return 1;
    else if (color[0] == color[1])
        return 2;
    else
        return 3;
}

int changeColor(int xPos, int yPos)
{
    char colorChar[10];
    gets(colorChar);
    for (int i = 0; i < strlen(colorChar); i++)
    {
        colorChar[i] = toupper(colorChar[i]);
    }
    clearRow(xPos, yPos, 40);
    switch (checkColor(colorChar))
    {
    case -1:
        return 1;
    case 0:
        printAt(xPos, yPos, "THE INPUT IS TOO LONG");
        break;
    case 1:
        printAt(xPos, yPos, "INVALID COLOR(S)");
        break;
    case 2:
        printAt(xPos, yPos, "COLORS MUST BE DIFFERENT");
        break;
    case 3:
        strcpy(colorBF, colorChar);
        return 1;
    }
    getch();
    return 0;
}

int gameParamsMenu()
{
    int state, prevState, action;
    char *statArr[32] = {"OFF", "ON"};
    char widChar[30], heiChar[30], soundChar[10];
    char newArr[5][64], *newA[64];
    char *navArr[64] = {"FIELD WIDTH(30-200)", "FIELD HEIGHT(10-100)", "SOUNDS('W'/'S' TO CHANGE)", "COLOR(2 DIFF HEX DIGITS)"};
    char *valArr[64] = {widChar, heiChar, soundChar, colorBF};

    int navLen = 4, xPos, yPos;
    state = 0;
    prevState = 0;
    int isMenu = 1;
    while (isMenu)
    {
        action = 0;
        xPos = WID/2-12;
        yPos = HEI/10 + 16;

        itoa(width, widChar, 10);
        itoa(height, heiChar, 10);
        strcpy(soundChar, statArr[sounds]);
        for (int i = 0; i < navLen; i++)
        {
            newArr[i][0] = '\0';
            strcat(newArr[i], navArr[i]);
            strcat(newArr[i], ": ");
            strcat(newArr[i], valArr[i]);
            newA[i] = newArr[i];
        }

        system("cls");
        asciiPr("||options||", WID/2-34, HEI/10);
        printAt(xPos-4, yPos-2, "GAME PARAMETERS");
        printNav(xPos, yPos, newA, navLen, state);

        while (!(action = handleNav(state, navLen)))
        {
            moveArrow(state, prevState, xPos-2, yPos);
            Sleep(40);
        }
        if (action == -1)
            return 1;
        int isNotSet = 1;
        while (isNotSet)
        {
            clearRow(xPos, yPos + 2*state, 40);
            printAt(xPos - 4, yPos + 2*state, navArr[state]);
            printf(": ");
            switch(state)
            {
            case 0:
                if (changeInt(xPos - 4, yPos + 2*state, 30, 200, width)) {
                    isNotSet = 0;
                    updateField();
                }
                break;
            case 1:
                if (changeInt(xPos - 4, yPos + 2*state, 10, 100, height)) {
                    isNotSet = 0;
                    updateField();
                }
                break;
            case 2:
                if (changeSounds(xPos - 2 + strlen(navArr[state]), yPos + 2*state)) {
                    updateSounds();
                    isNotSet = 0;
                }
                break;
            case 3:
                if(changeColor(xPos - 4, yPos + 2*state)) {
                    updateColor();
                    isNotSet = 0;
                }
                break;
            default:
                return 0;
            }
        }
    }
    return 1;
}

int optionsMenu()
{
    int state, prevState, action;

    char *navArr[64] = {"DIFFICULTY SETTINGS", "GAME PARAMETERS", "RESET TO DEFAULTS"};
    int navLen = 3, xPos, yPos;
    prevState = state = 0;
    int isMenu = 1;
    while (isMenu)
    {
        action = 0;
        xPos = WID/2-7;
        yPos = HEI/10+16;
        system("cls");
        asciiPr("||options||", WID/2-34, HEI/10);
        printNav(xPos, yPos, navArr, navLen, state);
        while (!(action = handleNav(state, navLen)))
        {
            moveArrow(state, prevState, xPos-2, yPos);
            Sleep(40);
        }
        if (action == -1)
            return 1;
        switch(state)
        {
        case 0:
            diffSetMenu();
            break;
        case 1:
            gameParamsMenu();
            break;
        case 2:
            setDefaults();
            updateAll();
            break;
        default:
            return 0;
        }
    }
    return 1;
}
