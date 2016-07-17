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

int getY(Ball ball) {return ball.slope*ball.x + ball.offset;}

Ball genInitPos(int dir)
{
    Ball b;
    b.x = MINX+(MAXX-MINX)/2;
    b.dir = dir;
    b.slope = 0;
    b.ymom = 0;
    b.offset = (MAXY-MINY)/2;
    b.vel = 0.76;
    b.speed = b.vel/(sqrt(b.slope*b.slope+1));
    b.y = getY(b);
    return b;
}

Platform initPlat(int x)
{
    Platform plat;
    plat.len = platLen;
    plat.mom = STAY;
    plat.x = x;
    plat.y = (MAXY - MINY - plat.len)/(2);
    return plat;
}

void handleBallMove(Ball &b, Platform &plat1, Platform &plat2, int &counter)
{
    if  (b.dir == -1 && b.x <= (double)(plat1.x + b.speed) && b.x >= plat1.x && round(b.y) >= plat1.y && round(b.y) <= plat1.y + plat1.len)
    {
        counter++;
        b.slope *= -1;
        b.slope += (fRand(0.3)+0.3)*plat1.mom;
        b.vel += (fRand(0.12)-0.05)*plat1.mom*b.ymom;
        b.dir *= -1;
        b.x = plat1.x+1;
        b.offset = b.y-b.slope*b.x;
        b.speed = b.vel/(sqrt(b.slope*b.slope+1));
        playWav("platHit.wav", 0);
        if (counter%2 == 0 && counter < 10)
            b.vel += 0.2;
    }
    if (b.dir == 1 && b.x >= (double)(plat2.x - b.speed) && b.x <= plat2.x && round(b.y) >= plat2.y && round(b.y) <= plat2.y + plat2.len)
    {
        counter++;
        b.slope *= -1;
        b.slope += (fRand(0.3)+0.3)*plat2.mom;
        b.vel += (fRand(0.12)-0.05)*plat1.mom*b.ymom;
        b.dir *= -1;
        b.x = plat2.x-1;
        b.offset = b.y-b.slope*b.x;
        b.speed = b.vel/(sqrt(b.slope*b.slope+1));
        playWav("platHit.wav", 0);
        if (counter%2 == 0 && counter < 10)
            b.vel += 0.2;
    }
    if(b.slope > 0)
        b.ymom = b.dir;
    else if(b.slope < 0)
        b.ymom = -1*b.dir;
    else
        b.ymom = 0;
    if ( b.y <= MINY && b.ymom == -1) {
        b.slope *= -1;
        b.y = MINY;
        b.offset = b.y-b.slope*b.x;
        playWav("board.wav", 0);
    }
    if (b.y >= MAXY && b.ymom == 1) {
        b.slope *= -1;
        b.y = MAXY;
        b.offset = b.y-b.slope*b.x;
        playWav("board.wav", 0);
    }
    if(b.slope > 0)
        b.ymom = b.dir;
    else if(b.slope < 0)
        b.ymom = -1*b.dir;
    else
        b.ymom = 0;
}

void updateBallPos(Ball &b, int &oldX, int &oldY)
{
    oldX = round(b.x);
    oldY = round(b.y);
    b.x += b.dir*b.speed;
    b.y = getY(b);
}

int isScored(Ball b)
{
    if (b.x <= MINX) {
        playWav("score.wav", 0);
        return 2;
    }
    else if (b.x >= MAXX) {
        playWav("score.wav", 0);
        return 1;
    }
    else
        return 0;
}


int handleAI(Ball b, Platform &plat2)
{
    static int reactTime = reaction;
    if (b.dir = 1)
    {
        if (reactTime > 0)
        {
            reactTime -= wait;
        }
        else if (b.x > MAXX-(MAXX-MINX)/3)
        {
            if (b.y <= plat2.y)
                plat2.mom = UP;
            else if (b.y >= plat2.y+plat2.len)
                plat2.mom = DOWN;
            else
                reactTime = reaction;
        }
        else if (b.x > MAXX-(MAXX-MINX)/2)
        {
            if (plat2.mom == STAY)
            {
                plat2.mom = rand()%3 - 1;
            }
            else if (rand()%60 < 6)
            {
                plat2.mom *= -1;
            }
        }
    }
    else
        plat2.mom = STAY;
    return 1;
}
