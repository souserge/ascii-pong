#ifndef GRAPHICS_HPP_INCLUDED
#define GRAPHICS_HPP_INCLUDED

void drawField();
void drawBall(Ball &b);
void clearChar(int x, int y);
void clearRow(int x, int y, int len);
void printAt(int x, int y, char *message);
void drawPlat(Platform plat);
void drawFigures(Ball &b, Platform &plat1, Platform &plat2);
void drawScore(int score, int x, int y);
void printResult(int p1Score, int p2Score);
void gbMessage();
void cls();
void printNav(int x, int y, char *navArr[64], int length, int state);
void moveArrow(int &state, int &prevState, int x, int y);
void moveFooter(int startx, int endx, int y, char message[]);
void drawFooter(int x, int y, int startI, int endI, char message[]);
int helpMenu();
#endif // GRAPHICS_HPP_INCLUDED
