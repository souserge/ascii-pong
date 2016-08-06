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
        if (counter%2 == 0 && counter < 13)
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
        if (counter%2 == 0 && counter < 13)
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

int randomChance(int level, int maximum) {
    int prob = rand()%(maximum+1)+1;
    if (prob <= level)
        return 1;
    return 0;
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

void setAIMode(int mode) {
    switch (mode) {
        case 1:
            aiPlat1 = 0;
            aiPlat2 = 1;
            playMode = 1;
            break;
        case 2:
            if (aiFun) {
                aiPlat1 = 1;
                aiPlat2 = 1;
                playMode = 3;
            }
            else {
                aiPlat1 = 0;
                aiPlat2 = 0;
                playMode = 2;
            }
            break;
        default:
            break;
    }
}

void handleAI(Ball b, Platform &p, AI &ai)
{
    switch (aiType)
    {
    case 0:
        crazyAI(b, p, ai);
        break;
    case 1:
        humanlikeAI(b, p, ai);
        break;
    }
}

int crazyAI(Ball b, Platform &p, AI &ai)
{
    switch (ai.state) {
    case IDLE:
        p.mom = STAY;
        if (!ai.isCalculated) {
            ai.mistake = (2-aiLevel)*10;
            ai.isCalculated = 1;
        }
        ai.mistake--;
        if (ai.mistake <= 0) {
            ai.mistake = 2-aiLevel;
            if (b.dir == ai.side && fabs(b.x-p.x) < (double)width/(ai.mistake+3)) {
                ai.ballY = b.ymom;
                ai.state = HIT;
            }
            else {
                ai.state = BACK;
            }
            ai.isCalculated = 0;
        }
        break;
    case BACK:
        if (p.mom == STAY) {
            p.mom = rand()%3 - 1;
        }
        else if (rand()%60 < 6) {
            p.mom *= -1;
        }
        if (b.dir == ai.side && fabs(b.x-p.x) < width/(ai.mistake+3)) {
                ai.ballY = b.ymom;
                ai.state = HIT;
        }
        break;
    case HIT:
        if (ai.ballY != b.ymom)
            ai.state = IDLE;
        else if (b.y <= (p.y - rand()%(ai.mistake+1)))
            p.mom = UP;
        else if (b.y >= (p.y + p.len + rand()%(ai.mistake+1)))
            p.mom = DOWN;
        break;
    }
    return 1;
}


int humanlikeAI(Ball b, Platform &p, AI &ai) {

    int miss = 0;
    switch(ai.state)
    {
    case IDLE:
        if(b.dir == ai.side && fabs(b.x-p.x) < (double)width/(ai.mistake+3))
            ai.state = HIT;
        else
            p.mom = STAY;
        break;
    case BACK:
        if(b.dir == ai.side && fabs(b.x-p.x) < (double)width/(ai.mistake+3))
            ai.state = HIT;
        else if (p.y == fieldY + height/2 - p.len/2)
            ai.state = IDLE;
        else if (p.y > fieldY + height/2 - p.len/2)
            p.mom = UP;
        else
            p.mom = DOWN;
        break;
    case HIT:
        if (!ai.isCalculated) {
            double ballYD = b.slope*(p.x - ai.side*b.speed)+b.offset;

            if (ballYD < fieldY) {
                ai.ballY = floor(fabs(ballYD-fieldY));
                if ((ai.ballY/height)%2 == 1) {
                    ai.ballY = fiYH - ai.ballY%height;
                }
                else {
                    ai.ballY = fieldY + ai.ballY%height;
                }
            }
            else if (ballYD > fiYH) {
                ai.ballY = floor(fabs(ballYD-fiYH));
                if ((ai.ballY/height)%2 == 0) {
                    ai.ballY = fiYH - ai.ballY%height;
                }
                else {
                    ai.ballY = fieldY + ai.ballY%height;
                }
            }
            else {
                ai.ballY = round(ballYD);
            }
            ai.isCalculated = 1;
            if (ai.mistake = randomChance(6-3*aiLevel, 10)) {
                ai.ballY += rand()%(2*p.len+1) - (double)p.len;
                miss = (2-aiLevel)*ai.mistake;
            }
            else {
                miss = 0;
            }
        }


        if (b.dir == -1*ai.side) {
            ai.isCalculated = 0;
            ai.state = BACK;
        }
        else if (ai.ballY + rand()%(miss+1) < p.y + p.len/2) {
            p.mom = UP;
        }
        else if (ai.ballY - rand()%(miss+1) > p.y + p.len/2) {
            p.mom = DOWN;
        }
        else if (fabs(b.x - p.x) <= b.speed) {
            if (ai.mistake == 1 && b.y < p.y + (double)p.len/2) {
                p.mom = DOWN;
            }
            else if (ai.mistake == 1 && b.y > p.y + (double)p.len/2) {
                p.mom = UP;
            }
            else if (b.y < p.y + (double)p.len/2) {
                p.mom = UP;
            }
            else if (b.y > p.y + (double)p.len/2) {
                p.mom = DOWN;
            }
            else
                p.mom = rand()%3 - 1;
        }
        else {
            p.mom = STAY;
        }
        break;
    }
    return 1;
}


AI createAI(int side) {
    AI ai;
    ai.mistake = 0;
    ai.state = IDLE;
    ai.ballY = -1;
    ai.isCalculated = 0;
    ai.side = side;
    return ai;
}
