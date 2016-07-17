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
#include "networking.hpp"
#include "globVars.hpp"

void keyboard(Platform &plat1, Platform &plat2) {
        while (kbhit()) {
            switch (toupper(getch())) {
            case 'W':
                plat1.mom = UP;
                break;
            case 'S':
                plat1.mom = DOWN;
                break;
            case 'O':
                if (playMode == 2)
                    plat2.mom = UP;
                break;
            case 'L':
                if (playMode == 2)
                    plat2.mom = DOWN;
                break;
            default:
                break;
            }
        }
        if (!GetAsyncKeyState(0x57) && !GetAsyncKeyState(0x53))
            plat1.mom = STAY;
        if (playMode == 2 && !GetAsyncKeyState(0x4F) && !GetAsyncKeyState(0x4C))
            plat2.mom = STAY;
}

void handleInput(Platform &plat1, Platform &plat2)
{
    if (plat1.mom == UP && plat1.y > MINY) {
            clearChar(plat1.x, plat1.y+plat1.len-1);
            plat1.y--;
    }
    if (plat1.mom == DOWN && plat1.y+plat1.len-1 < MAXY) {
        clearChar(plat1.x, plat1.y);
        plat1.y++;
    }
    if (plat2.mom == UP && plat2.y > MINY) {
        clearChar(plat2.x, plat2.y+plat2.len-1);
        plat2.y--;
    }
    if (plat2.mom == DOWN && plat2.y+plat2.len-1 < MAXY) {
        clearChar(plat2.x, plat2.y);
        plat2.y++;
    }
}

int handleNav(int &state, int noOfStates)
{
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

int helpMenu()
{
    int state, action;

    HELP_MSG1:
    state = 0;
    action = 0;
    system("cls");
    asciiPr("||help||", ((MINX+MAXX+1)/2-20), MINY+1);
    asciiPr("||helpMsg1||", ((MINX+MAXX+1)/2-28), MINY+13);
    while ((action = handleNav(state, 0)) == 0);
    switch (action)
    {
    case -1:
        return 1;
    case 1:
        break;
    default:
        break;
    }
    system("cls");
    asciiPr("||help||", ((MINX+MAXX+1)/2-20), MINY+1);
    asciiPr("||helpMsg2||", ((MINX+MAXX+1)/2-28), MINY+13);
    while (!(action = handleNav(state, 0)));
    switch (action)
    {
    case -1:
        goto HELP_MSG1;
    case 1:
        return 1;
    default:
        return 0;
    }
}

int optionsMenu()
{
    return 1;
}

int mainMenu()
{
    int state, prevState, action;

    int isMenu = 1;
    while(isMenu)
    {
        state = 0;
        prevState = 0;
        action = 0;
        system("cls");
        asciiPr("||bannerAB||", 6, 4);

        gotoxy((MINX+MAXX+1)/2-2, MINY+11+6);
        printf("1 PLAYER");
        gotoxy((MINX+MAXX+1)/2-2, MINY+11+8);
        printf("2 PLAYERS");
        gotoxy((MINX+MAXX+1)/2-2, MINY+11+10);
        printf("HELP");
        gotoxy((MINX+MAXX+1)/2-2, MINY+11+12);
        printf("OPTIONS");
        gotoxy((MINX+MAXX+1)/2-2, MINY+11+14);
        printf("QUIT");
        gotoxy((MINX+MAXX+1)/2-4, MINY+11+6);
        putch('>');
        while (!(action = handleNav(state, 5)))
        {
            if(state != prevState) {
                clearChar((MINX+MAXX+1)/2-4, MINY+11+6+2*prevState);
                gotoxy((MINX+MAXX+1)/2-4, MINY+11+6+2*state);
                putch('>');
                prevState = state;
            }
            moveFooter(MINX, MAXX, MAXY);
            Sleep(40);
        }
        if (action == -1)
            state = 2;
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
