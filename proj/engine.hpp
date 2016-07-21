#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "globVars.hpp"

int getY(Ball ball);
Ball genInitPos(int dir);
Platform initPlat(int x);
void handleBallMove(Ball &b, Platform &plat1, Platform &plat2, int &counter);
void updateBallPos(Ball &b, int &oldX, int &oldY);
int isScored(Ball b);
void handleAI(Ball b, Platform &p);
int crazyAI(Ball b, Platform &p);
int humanlikeAI(Ball b, Platform &p);
#endif // ENGINE_HPP_INCLUDED
