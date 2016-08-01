#ifndef GLOBVARS_HPP_INCLUDED
#define GLOBVARS_HPP_INCLUDED

#define MAXX 98
#define MINX 3
#define MAXY 37
#define MINY 2
#define DEF_WID 104
#define DEF_HEI 42

enum {UP = -1, STAY = 0, DOWN = 1};
enum {IDLE, BACK, HIT};
enum {OFF, ON};

typedef struct Ball {
    double x, y;
    short dir, ymom;
    double slope;
    double speed;
    double offset;
    double vel;
    char prevChar;
} Ball;

typedef struct Platform {
    int x, y;
    int len;
    int mom;
} Platform;

typedef struct AI {
    int state;
    int ballY;
    int isCalculated;
    int side;
} AI;

extern int aiLevel, aiType, wait; extern double ballSpeed; extern int platLen, scoreLimit;
extern int width, height, sounds; extern char colorBF[10];
extern int playMode, aiPlat1, aiPlat2, aiFun, fieldX, fiXW, fieldY, fiYH, WID, HEI;

void setAIType(int a);
void setAILevel(int a);
void setWait(int a);
void setBallSpeed(double d);
void setPlatLen(int a);
void setScoreLimit(int a);

void setWidth(int a);
void setHeight(int a);
void setSounds(int a);
void setColorBF(char *col);

void setPlayMode(int a) ;
void setFieldX(int a);
void setFieldY(int a);
void setFiXW(int a);
void setFiYH(int a);

void setDefaults();

#endif // GLOBVARS_HPP_INCLUDED
