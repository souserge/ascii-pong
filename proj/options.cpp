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


//extern int fieldX, fiXW, fieldY, fiYH, playMode;
//
//extern int platLen, wait, scoreLimit, reaction, aiType;
void updateColor()
{
    char color[128];
    sprintf(color, "COLOR %s", colorBF);
    system(color);
}

void updateField()
{
    if (width > WID-7)
    {
        WID = width+7;
    }
    if (height > HEI-5)
    {
        HEI = height+5;
    }
    if (width < WID-7)
    {
        WID = DEF_WID;
    }
    if (height < HEI-5)
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

int diffSetMenu()
{
    int state, prevState, action;

    char *navArr[64] = {"AI TYPE", "AI STRENGTH", "GAME SPEED", "BALL SPEED", "PLATFORM LENGTH", "SCORE LIMIT"};
    int navLen = 6, xPos, yPos;

    int isMenu = 1;
    while (isMenu)
    {
        state = 0;
        prevState = 0;
        action = 0;
        xPos = WID/2-7;
        yPos = 20;

        system("cls");
        asciiPr("||options||", 16, 4);
        printAt(xPos-4, yPos-2, "DIFFICULTY SETTINGS:");
        printNav(xPos, yPos, navArr, navLen);
        while (!(action = handleNav(state, navLen)))
        {
            moveArrow(state, prevState, xPos-2, yPos);
            Sleep(40);
        }
        if (action == -1)
            return 1;
        clearRow(xPos, yPos + 2*state, 20);
        printAt(xPos - 4, yPos + 2*state, navArr[state]);
        switch(state)
        {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        default:
            return 0;
        }
    }
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


int changeSounds(int xPos, int yPos)
{
    int state = sounds, action = 0, prevState = sounds;

    if (!state)
        printAt(xPos, yPos, "OFF");
    else
        printAt(xPos, yPos, "ON");

    while (!(action = handleNav(state, 2)))
        {
            if(state != prevState) {
                clearRow(xPos, yPos, 10);
                if (!state)
                    printAt(xPos, yPos, "OFF");
                else
                    printAt(xPos, yPos, "ON");
                prevState = state;
            }
            Sleep(40);
        }
        if (action == -1)
            return 1;
        else
            sounds = state;
}

int changeInt(int xPos, int yPos, int low, int high, int &param)
{
    int num;
    char buffer[128];
    char dec[16] = "0123456789";
    gets(buffer);
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
    if (num < low || num > high) {
        printAt(xPos, yPos, "OUT OF RANGE");
        getch();
        return 0;
    }
    param = num;
    return 1;
}

//extern int width, height;
int gameParamsMenu()
{
    int state, prevState, action;

    char widChar[30], heiChar[30], soundChar[10];
    char newArr[5][64], *newA[64];
    char *navArr[64] = {"FIELD WIDTH(30-200)", "FIELD HEIGHT(10-100)", "SOUNDS('W'/'S' TO CHANGE)", "COLOR(2 DIFF HEX DIGITS)"};
    char *valArr[64] = {widChar, heiChar, soundChar, colorBF};

    int navLen = 4, xPos, yPos;

    int isMenu = 1;
    while (isMenu)
    {
        state = 0;
        prevState = 0;
        action = 0;
        xPos = WID/2-10;
        yPos = 20;

        itoa(width, widChar, 10);
        itoa(height, heiChar, 10);
        if (!sounds)
            strcpy(soundChar, "OFF");
        else
            strcpy(soundChar, "ON");
        for (int i = 0; i < navLen; i++)
        {
            newArr[i][0] = '\0';
            strcat(newArr[i], navArr[i]);
            strcat(newArr[i], ": ");
            strcat(newArr[i], valArr[i]);
            newA[i] = newArr[i];
        }

        system("cls");
        asciiPr("||options||", 16, 4);
        printAt(xPos-4, yPos-2, "GAME PARAMETERS");
        printNav(xPos, yPos, newA, navLen);

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

    char *navArr[64] = {"(SOON) DIFFICULTY SETTINGS", "GAME PARAMETERS", "RESET TO DEFAULTS"};
    int navLen = 3, xPos, yPos;

    int isMenu = 1;
    while (isMenu)
    {
        state = 0;
        prevState = 0;
        action = 0;
        xPos = WID/2-7;
        yPos = 20;
        system("cls");
        asciiPr("||options||", 16, 4);
        printNav(xPos, yPos, navArr, navLen);
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
           // diffSetMenu();
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
