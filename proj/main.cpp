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

int mainGameLoop()
{
    Ball b;
    Platform plat1, plat2;
    int state = 0;
    int oldX, oldY;
    int dir = -1;
    int result = 0;
    int p1Score = 0;
    int p2Score = 0;
    int isRunning = 1;
    AI ai1;
    AI ai2;

    if (aiPlat1) {
        ai1 = createAI(-1);
    }
    if (aiPlat2) {
        ai2 = createAI(1);
    }

    while (isRunning)
    {
        fflush(stdin);
        if (result == 1)
        {
            p1Score++;
            dir = -1;
        }
        else if (result == 2)
        {
            p2Score++;
            dir = 1;
        }

        system("cls");
        drawField();
        printResult(p1Score, p2Score);
        if (p1Score >= scoreLimit)
        {
            isRunning = 0;
            if (aiPlat1 == 1)
            {
              asciiPr("||comWin||", WID/2 - 41, HEI/2 - 3);
            }
            asciiPr("||p1Win||", WID/2 - 37, HEI/2 - 3);
            Sleep(2000);
            fflush(stdin);
            printAt((WID-30), (HEI-3), "<Press ENTER to continue>");
            while (handleNav(state, 0) != 1);
        }
        else if (p2Score >= scoreLimit)
        {
            isRunning = 0;
            if (aiPlat2 == 1)
            {
              asciiPr("||comWin||", WID/2 - 41, HEI/2 - 3);
            }
            else
                asciiPr("||p2Win||", WID/2 - 39, HEI/2 - 3);
            Sleep(2000);
            fflush(stdin);
            printAt((WID-30), (HEI-3), "<Press ENTER to continue>");
            while (handleNav(state, 0) != 1);
        }
        else {
            plat1 = initPlat(fieldX);
            plat2 = initPlat(fiXW);
            drawPlat(plat1);
            drawPlat(plat2);
            b = genInitPos(dir);
            drawBall(b);
            Sleep(1337);
            int correctPos = 1;
            int counter = 0;
            while (correctPos)
            {
                updateBallPos(b, oldX, oldY);
                if ((result = isScored(b)) != 0) {
                    correctPos = 0;
                }
                else
                {
                    if (aiPlat2) {
                        handleAI(b, plat2, ai2);
                    }
                    if (aiPlat1) {
                        handleAI(b, plat1, ai1);
                    }
                    if (!keyboard(plat1, plat2))
                        return 0;
                    handleInput(plat1, plat2);
                    handleBallMove(b, plat1, plat2, counter);
                    gotoxy(oldX, oldY);
                    putch(b.prevChar);
                    drawFigures(b, plat1, plat2);
                    Sleep(wait);
                }
            }
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    setDefaults();
    initConsole();
    int isGame = 1;
    while (isGame)
    {
        playWav("music.wav", 1);
        switch(mainMenu()) {
        case 0:
            gbMessage();
            isGame = 0;
            return 0;
        case 1:
            setAIMode(1);
            break;
        case 2:
            setAIMode(2);
            break;
        }
        stopWav();
        mainGameLoop();
    }
    return 0;
}
