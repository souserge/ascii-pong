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

void drawField()
{
    int x, y;
    gotoxy(fieldX-1, fieldY-1);
    putchar('o');
    for (x = 0; x <= width; x++)
    {
        putchar('-');
    }
    putchar('o');
    for (y = 0; y <= height; y++) {
        gotoxy(fieldX-1,fieldY+y);
        putchar('|');
        for (x = 0; x <= width; x++) {
            putchar(' ');
        }
        putchar('|');
    }
    gotoxy(fieldX-1, fieldY+height+1);
    putchar('o');
    for (x = 0; x <= width; x++)
    {
        putchar('-');
    }
    putchar('o');
}

void drawBall(Ball &b)
{
    int x = round(b.x);
    int y = round(b.y);
    gotoxy(x, y);
    b.prevChar = getxy(x, y);
    putch('O');
}

void clearChar(int x, int y)
{
    gotoxy(x+1, y);
    putch('\b');
    putch(' ');
}

void clearRow(int x, int y, int len)
{
    for (int i = 0; i < len; i++)
    {
        clearChar(x+i, y);
    }
}

void printAt(int x, int y, char *message)
{
    gotoxy(x, y);
    printf("%s", message);
}

void drawPlat(Platform plat)
{
    int y;
    for (y = 0; y < plat.len; y++){
        gotoxy(plat.x, plat.y+y);
        putch('X');
    }
}

void drawFigures(Ball &b, Platform &plat1, Platform &plat2)
{
    drawBall(b);
    drawPlat(plat1);
    drawPlat(plat2);
}

void drawScore(int score, int x, int y)
{
    char keyword[15];
    sprintf(keyword, "||num%d||", score);
    asciiPr(keyword, x, y);
}

void printResult(int p1Score, int p2Score)
{
    drawScore(p1Score, fieldX+width/10, fieldY+height/10);
    drawScore(p2Score, fiXW-7-(width)/10, fieldY+height/10);
}

void gbMessage()
{
    system("cls");
    asciiPr("||goodbye||", ((WID+1)/2-26), HEI/3);
    Sleep(1000);
    clearBuffer();
    printAt((WID-20), (HEI-4), "<Press any key>");
    getch();
}

void cls() {system("cls");}

void printNav(int x, int y, char *navArr[64], int length, int state)
{
    for (int i = 0; i < length; i++)
    {
        gotoxy(x, y+2*i);
        puts(navArr[i]);
    }
    gotoxy(x-2, y + 2*state);
    putch('>');
}

void moveArrow(int &state, int &prevState, int x, int y)
{
    if(state != prevState) {
        clearChar(x, y+2*prevState);
        gotoxy(x, y+2*state);
        putch('>');
        prevState = state;
    }
}

void moveFooter(int startx, int endx, int y, char *message)
{
    static int counter = 0;
    int len = strlen(message);
    int startI, endI, x;
    //int startTailI, endTailI, tailX; TODO

    if (counter > (endx - startx)) {
        x = startx;
        startI = counter - (endx - startx);
    }
    else {
        x = (endx - counter);
        startI = 0;
    }
    if (counter < len) {
        endI = counter;
    }
    else {
        endI = len;
    }

    drawFooter(x, y, startI, endI, message);
    counter++;
    if (counter > endx-startx+len)
        counter = 0;
}

void drawFooter(int x, int y, int startI, int endI, char message[])
{
    gotoxy(x, y);
    for (int i = startI; i <= endI; i++)
    {
        putchar(message[i]);
    }
    putchar('\0');
}

int helpMenu()
{
    int state, action;

    while (true)
    {
        state = 0;
        action = 0;
        system("cls");
        asciiPr("||help||", (WID/2-20), HEI/10);
        asciiPr("||helpMsg1||", (WID/2-28), HEI/10+12);
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
        asciiPr("||help||", (WID/2-20), HEI/10);
        asciiPr("||helpMsg2||", (WID/2-28), HEI/10+12);

        while (!(action = handleNav(state, 0)));
        switch (action)
        {
        case -1:
            break;
        case 1:
            return 1;
        default:
            return 0;
        }
    }
}
