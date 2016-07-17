#ifndef GLOBVARS_HPP_INCLUDED
#define GLOBVARS_HPP_INCLUDED

#define MAXX 98
#define MINX 3
#define MAXY 37
#define MINY 2

enum {UP = -1, STAY = 0, DOWN = 1};

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

extern int platLen, wait, scoreLimit, playMode, reaction;
extern int fieldX, filedY, width, heigth;

#endif // GLOBVARS_HPP_INCLUDED
