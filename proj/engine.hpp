#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "globVars.hpp"

int getY(Ball ball);
Ball genInitPos(int dir);
Platform initPlat(int x);
void handleBallMove(Ball &b, Platform &plat1, Platform &plat2, int &counter);
void updateBallPos(Ball &b, int &oldX, int &oldY);
int isScored(Ball b);
int handleAI(Ball b, Platform &plat2);

#endif // ENGINE_HPP_INCLUDED
