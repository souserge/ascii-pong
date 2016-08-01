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

int keyboard(Platform &plat1, Platform &plat2) {
    GetAsyncKeyState(0x0D); GetAsyncKeyState(0x1B);
    while (kbhit()) {
        switch (toupper(getch())) {
        case 'W':
            if (aiPlat1 == 0)
                plat1.mom = UP;
            break;
        case 'S':
            if (aiPlat1 == 0)
                plat1.mom = DOWN;
            break;
        case 'O':
            if (aiPlat2 == 0)
                plat2.mom = UP;
            break;
        case 'L':
            if (aiPlat2 == 0)
                plat2.mom = DOWN;
            break;
        default:
            break;
        }
    }
    if (aiPlat1 == 0 && !GetAsyncKeyState(0x57) && !GetAsyncKeyState(0x53))
        plat1.mom = STAY;
    if (aiPlat2 == 0 && !GetAsyncKeyState(0x4F) && !GetAsyncKeyState(0x4C))
        plat2.mom = STAY;
    if (GetAsyncKeyState(0x0D) && GetAsyncKeyState(0x1B))
        return 0;
    return 1;
}

void handleInput(Platform &plat1, Platform &plat2)
{
    if (plat1.mom == UP && plat1.y > fieldY) {
            clearChar(plat1.x, plat1.y+plat1.len-1);
            plat1.y--;
    }
    if (plat1.mom == DOWN && plat1.y+plat1.len-1 < fieldY+height) {
        clearChar(plat1.x, plat1.y);
        plat1.y++;
    }
    if (plat2.mom == UP && plat2.y > fieldY) {
        clearChar(plat2.x, plat2.y+plat2.len-1);
        plat2.y--;
    }
    if (plat2.mom == DOWN && plat2.y+plat2.len-1 < fieldY+height) {
        clearChar(plat2.x, plat2.y);
        plat2.y++;
    }
}

int handleNav(int &state, int noOfStates)
{
    fflush(stdin);
    while (kbhit()) {
            switch (toupper(getch())) {
            case 27:
                return -1;
            case 'W':
                if(state > 0)
                    state--;
                break;
            case 'S':
                if(state < noOfStates-1)
                    state++;
                break;
            case '\r': case '\n':
                return 1;
                break;
            default:
                break;
            }
        }
        return 0;
}

int mainMenu()
{
    int state, prevState, action;

    char *navArr[64] = {"1 PLAYER", "2 PLAYERS", "HELP", "OPTIONS", "QUIT"};
    int navLen = 5, xPos, yPos;

    char message[] = "NOTE: use 'W'/'S' to navigate; ENTER to select        Make sure you have an English keyboard layout!";
    state = 0;
    prevState = 0;
    int isMenu = 1;
    while(isMenu)
    {
        action = 0;
        xPos = WID/2-4;
        yPos = HEI/10+16;

        cls();
        asciiPr("||bannerAB||", WID/2-45, HEI/10);
        printNav(xPos, yPos, navArr, navLen, state);
        while ((action = handleNav(state, navLen) != 1))
        {
            moveArrow(state, prevState, xPos-2, yPos);
            moveFooter(2, WID-2, HEI - 2, message);
            Sleep(40);
        }
        switch (state)
        {
        case 0:
            return 1;
        case 1:
            return 2;
        case 2:
            if (helpMenu() == 1)
                break;
        case 3:
            if (optionsMenu() == 1)
                break;
        case 4:
            return 0;
        }
    }
    return 0;
}
