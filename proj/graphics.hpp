#ifndef GRAPHICS_HPP_INCLUDED
#define GRAPHICS_HPP_INCLUDED

void drawField();
void drawBall(Ball &b);
void clearChar(int x, int y);
void drawPlat(Platform plat);
void drawFigures(Ball &b, Platform &plat1, Platform &plat2);
void drawScore(int score, int x, int y);
void printResult(int p1Score, int p2Score);
void gbMessage();
void moveFooter(int startx, int endx, int y);
void drawFooter(int x, int y, int startI, int endI, char message[]);
#endif // GRAPHICS_HPP_INCLUDED
