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

void drawField()
{
    int x, y;
    gotoxy(MINX-1, MINY-1);
    putchar('o');
    for (x = MINX; x <= MAXX; x++)
    {
        putchar('-');
    }
    putchar('o');
    for (y = MINY; y <= MAXY; y++) {
        gotoxy(MINX-1, y);
        putchar('|');
        for (x = MINX; x <= MAXX; x++) {
            putchar(' ');
        }
        putchar('|');
    }
    gotoxy(MINX-1, MAXY+1);
    putchar('o');
    for (x = MINX; x <= MAXX; x++)
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
    drawScore(p1Score, MINX+5, MINY+3);
    drawScore(p2Score, MAXX-15, MINY+3);
}

void gbMessage()
{
    int state;
    system("cls");
    asciiPr("||goodbye||", ((MINX+MAXX+1)/2-26), (MINY+MAXY+1)/3);
    Sleep(1000);
    fflush(stdin);
    gotoxy((MINX+MAXX-20), (MINY+MAXY-3));
    printf("<Press any key>");
    getch();
}

void moveFooter(int startx, int endx, int y)
{
    static int counter = 0;
    char message[] = "NOTE: use 'W'/'S' to navigate; ENTER to select          Make sure you have an English keyboard layout!";
    static int len = strlen(message);
    int startI, endI, x;


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
