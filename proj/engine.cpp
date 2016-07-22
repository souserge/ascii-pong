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


int getY(Ball ball) {return ball.slope*ball.x + ball.offset;}

Ball genInitPos(int dir)
{
    Ball b;
    b.x = fieldX+(width)/2;
    b.dir = dir;
    b.slope = 0;
    b.ymom = 0;
    b.offset = fieldY + (height)/2;
    b.vel = ballSpeed;
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
    plat.y = fieldY + height/2 - plat.len/2;
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
        if (counter%2 == 0 && counter < 12)
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
    if ( b.y <= fieldY && b.ymom == -1) {
        b.slope *= -1;
        b.y = fieldY;
        b.offset = b.y-b.slope*b.x;
        playWav("board.wav", 0);
    }
    if (b.y >= fieldY+height && b.ymom == 1) {
        b.slope *= -1;
        b.y = fieldY+height;
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
    if (b.x <= fieldX) {
        playWav("score.wav", 0);
        return 2;
    }
    else if (b.x >= fieldX + width) {
        playWav("score.wav", 0);
        return 1;
    }
    else
        return 0;
}

void handleAI(Ball b, Platform &p)
{
    switch (aiType)
    {
    case 0:
        crazyAI(b, p);
        break;
    case 1:
        humanlikeAI(b, p);
        break;
    }
}

int crazyAI(Ball b, Platform &p)
{
    if (b.x > fieldX+(2*width)/3)
    {
        if (b.y <= p.y)
            p.mom = UP;
        else if (b.y >= p.y+p.len)
            p.mom = DOWN;
    }
    else if (b.x > fieldX)
    {
        if (p.mom == STAY)
        {
            p.mom = rand()%3 - 1;
        }
        else if (rand()%60 < 6)
        {
            p.mom *= -1;
        }
    }
    return 1;
}


int humanlikeAI(Ball b, Platform &p)
{
    static int state = IDLE;

    switch(state)
    {
    case IDLE:
        if(b.dir == 1 && b.x >= fieldX+(2*width)/3)
            state = HIT;
        else
            p.mom = STAY;
        break;
    case BACK:
        if(b.dir == 1 && b.x >= fieldX+(2*width)/3)
            state = HIT;
        else if (p.y == fieldY + height/2 - p.len/2)
            state = IDLE;
        else if (p.y > fieldY + height/2 - p.len/2)
            p.mom = UP;
        else
            p.mom = DOWN;
        break;
    case HIT:
        if (b.dir == -1)
            state = BACK;
        else if (b.y <= p.y)
            p.mom = UP;
        else if (b.y >= p.y+p.len)
            p.mom = DOWN;
        break;
    }
    return 1;
}
