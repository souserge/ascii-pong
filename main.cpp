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
#define MAXX 98
#define MINX 3
#define MAXY 37
#define MINY 2
#define TERMH 2

enum {STAY, UP, DOWN};
//double slopeValues[] = {0.25, 0.5, 1, 1.5, 2, 2.5, 3};

typedef struct Ball {
    double x, y;
    short dir, ymom;
    double slope;
    double speed;
    double offset;
    char prevChar;
} Ball;

typedef struct Platform {
    int x, y;
    int len;
    int mom;
} Platform;

typedef struct Terminal {
    int startX, startY;
    int lastX, lastY;
    int length, height;
} Terminal;

int platLen = 5;

void stopWav(void) {
    sndPlaySound(NULL, SND_ASYNC);
}

void playWav(char *filedest, int loop) {
    stopWav();
    char dest[200] = "res/sound/";
    strcat(dest, filedest);
    if (loop) {
        sndPlaySound(TEXT(dest), SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
    else {
        sndPlaySound(TEXT(dest), SND_FILENAME | SND_ASYNC);
    }
}



double fRand(double fMax)
{
    srand (time(NULL));
    double f = (double)(rand()%1000)/1000;
    return f*fMax;
}

void gotoxy( int column, int line )
  {
  COORD coord;
  coord.X = column;
  coord.Y = line;
  SetConsoleCursorPosition(
    GetStdHandle( STD_OUTPUT_HANDLE ),
    coord
    );
  }

char getxy(int x, int y)
{
    HANDLE     hStdOut      = GetStdHandle( STD_OUTPUT_HANDLE );
   // PCHAR_INFO buffer       = new CHAR_INFO[ 1 ];
   CHAR_INFO buffer;
    COORD      buffer_size  = { 1, 1 };
    COORD      buffer_index = { 0, 0 };  // read/write rectangle has upper-right corner at upper-right corner of buffer
    SMALL_RECT read_rect    = { x,     y,     x     , y    };

    ReadConsoleOutput(  hStdOut, &buffer, buffer_size, buffer_index, &read_rect );
    return (char)buffer.Char.AsciiChar;
}

int wherex()
  {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD  result;
  if (!GetConsoleScreenBufferInfo(
         GetStdHandle( STD_OUTPUT_HANDLE ),
         &csbi
         ))
    return -1;
  return result.X;
  }

int wherey()
  {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD  result;
  if (!GetConsoleScreenBufferInfo(
         GetStdHandle( STD_OUTPUT_HANDLE ),
         &csbi
         ))
    return -1;
  return result.Y;
  }

void asciiPr(char *keyword, int x, int y)
{
    int keylen = strlen(keyword);
    int found = 0;
    char str[1000], stop[]="|stop|";
    FILE *art = fopen("res/text/ascii.txt", "r");
    if (art == NULL)
    {
        perror("");
        return;
    }

    while (fgets(str, sizeof(str), art) != NULL)
    {
        if (found == 1 && strncmp(stop, str, 6) == 0)
            break;
        if (found == 1) {
            gotoxy(x,y);
            printf("%s", str);
            y++;
        }
        if (strncmp(keyword, str, keylen) == 0)
            found = 1;
        if(feof(art)) {
            break;
        }
    }
    fclose(art);
}

int getY(Ball ball) {return MINY + ball.slope*ball.x + ball.offset;}

Ball genInitPos(int dir)
{
    Ball b;
    b.x = MINX+(MAXX-MINX)/2;
    b.dir = dir;
    b.slope = fRand(2.5);
    if (b.slope > -0.1 && b.slope < 0.1) {
        b.slope = 1;
    }
    if(b.slope > 0)
        b.ymom = dir;
    else
        b.ymom = -1*dir;
    b.offset = MINY+rand()%(MAXY-MINY-1) - b.slope*b.x +1; //TOFIX
    b.speed = sqrt(2)/(sqrt(b.slope*b.slope+1));
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

void drawField()
{
    int x, y;
    gotoxy(MINX-1, MINY-1);
    putchar('o');
    for (x = MINX; x <= MAXX; x++)
    {
        putchar('-');
    }
    putchar('o');
    for (y = MINY; y <= MAXY; y++) {
        gotoxy(MINX-1, y);
        putchar('|');
        for (x = MINX; x <= MAXX; x++) {
            putchar(' ');
        }
        putchar('|');
    }
    gotoxy(MINX-1, MAXY+1);
    putchar('o');
    for (x = MINX; x <= MAXX; x++)
    {
        putchar('-');
    }
    putchar('o');
}

void drawBall(Ball &b)
{
    int x = round(b.x);
    int y = round(b.y);
    gotoxy(x, y);
    b.prevChar = getxy(x, y);
    putch('O');
}

void clearChar(int x, int y)
{
    gotoxy(x+1, y);
    putch('\b');
    putch(' ');
}

void drawPlat(Platform plat)
{
    int y;
    for (y = 0; y < plat.len; y++){
        gotoxy(plat.x, plat.y+y);
        putch('X');
    }
}

void keyboard(Platform &plat1, Platform &plat2) {
        while (kbhit()) {
            switch (toupper(getch())) {
            case 'W':
                plat1.mom = UP;
                break;
            case 'S':
                plat1.mom = DOWN;
                break;
            case 'O':
                plat2.mom = UP;
                break;
            case 'L':
                plat2.mom = DOWN;
                break;
            default:
                break;
            }
        }
        if (!GetAsyncKeyState(0x57) && !GetAsyncKeyState(0x53))
            plat1.mom = STAY;
        if (!GetAsyncKeyState(0x4F) && !GetAsyncKeyState(0x4C))
            plat2.mom = STAY;
}

void handleInput(Platform &plat1, Platform &plat2)
{
    if (plat1.mom == UP && plat1.y > MINY) {
            clearChar(plat1.x, plat1.y+plat1.len-1);
            plat1.y--;
    }
    if (plat1.mom == DOWN && plat1.y+plat1.len-1 < MAXY) {
        clearChar(plat1.x, plat1.y);
        plat1.y++;
    }
    if (plat2.mom == UP && plat2.y > MINY) {
        clearChar(plat2.x, plat2.y+plat2.len-1);
        plat2.y--;
    }
    if (plat2.mom == DOWN && plat2.y+plat2.len-1 < MAXY) {
        clearChar(plat2.x, plat2.y);
        plat2.y++;
    }
}

void handleBallMove(Ball &b, Platform &plat1, Platform &plat2)
{
    if  (b.dir == -1 && b.x <= (double)(plat1.x + b.speed) && b.x > plat1.x && b.y >= plat1.y && b.y <= plat1.y + plat1.len)
    {
        b.offset += 2*b.slope*b.x;
        b.slope *= -1;
        b.dir *= -1;
        b.x = plat1.x+1;
        playWav("platHit.wav", 0);
    }
    if (b.dir == 1 && b.x >= (double)(plat2.x - b.speed) && b.x < plat2.x && b.y >= plat2.y && b.y <= plat2.y + plat2.len)
    {
        b.offset += 2*b.slope*b.x;
        b.slope *= -1;
        b.dir *= -1;
        b.x = plat2.x-1;
        playWav("platHit.wav", 0);
    }

    if ( b.y <= MINY && b.ymom == -1) {
        b.offset += 2*b.slope*b.x;
        b.slope *= -1;
        b.ymom = 1;
        b.y = MINY;
        playWav("board.wav", 0);
    }
    if (b.y >= MAXY && b.ymom == 1) {
        b.offset += 2*b.slope*b.x;
        b.slope *= -1;
        b.ymom = -1;
        b.y = MAXY;
        playWav("board.wav", 0);
    }
}

void drawFigures(Ball &b, Platform &plat1, Platform &plat2)
{
    drawBall(b);
    drawPlat(plat1);
    drawPlat(plat2);
}

int isScored(Ball b)
{
     if (b.x <= MINX)
        return 2;
    else if (b.x >= MAXX)
        return 1;
    else
        return 0;
}

int scored(int result)
{
    playWav("score.wav", 0);
    return 0;
}


int handleNav(int &state, int noOfStates)
{
    while (kbhit()) {
            switch (toupper(getch())) {
            case 27:
                return -1;
            case 'W':
                if(state > 0)
                    state--;
                break;
            case 'S':
                if(state < noOfStates-1)
                    state++;
                break;
            case '\r': case '\n':
                return 1;
                break;
            default:
                break;
            }
        }
        return 0;
}

int helpMenu()
{
    int state, action;

    HELP_MSG1:
    state = 0;
    action = 0;
    system("cls");
    asciiPr("||help||", ((MINX+MAXX+1)/2-20), MINY+1);
    asciiPr("||helpMsg1||", ((MINX+MAXX+1)/2-28), MINY+13);
    while ((action = handleNav(state, 0)) == 0);
    switch (action)
    {
    case -1:
        return 1;
    case 1:
        break;
    default:
        break;
    }
    system("cls");
    asciiPr("||help||", ((MINX+MAXX+1)/2-20), MINY+1);
    asciiPr("||helpMsg2||", ((MINX+MAXX+1)/2-28), MINY+13);
    while (!(action = handleNav(state, 0)));
    switch (action)
    {
    case -1:
        goto HELP_MSG1;
    case 1:
        return 1;
    default:
        return 0;
    }
}

int mainMenu()
{
    int state, prevState, action;

    int isMenu = 1;
    while(isMenu)
    {
        state = 0;
        prevState = 0;
        action = 0;
        system("cls");
        asciiPr("||bannerAB||", ((MINX+MAXX+1)/2-39), MINY+1);
        gotoxy((MINX+MAXX+1)/2-2, MINY+11+6);
        printf("START");
        gotoxy((MINX+MAXX+1)/2-2, MINY+11+8);
        printf("HELP");
        gotoxy((MINX+MAXX+1)/2-2, MINY+11+10);
        printf("QUIT");

        gotoxy((MINX+MAXX+1)/2-4, MINY+11+6);
        putch('>');
        while (!(action = handleNav(state, 3)))
        {
            if(state != prevState) {
                clearChar((MINX+MAXX+1)/2-4, MINY+11+6+2*prevState);
                gotoxy((MINX+MAXX+1)/2-4, MINY+11+6+2*state);
                putch('>');
                prevState = state;
            }
        }
        if (action == -1)
            state = 2;
        switch (state)
        {
        case 0:
            return 1;
        case 1:
            if (helpMenu() == 1)
                break;
        case 2:
            return 0;
        }
    }
    return 0;
}

void drawScore(int score, int x, int y)
{
    char keyword[15];
    sprintf(keyword, "||num%d||", score);
    asciiPr(keyword, x, y);
}


void printResult(Terminal term, int p1Score, int p2Score)
{
//    gotoxy(5, term.startY);
//    printf("Player1:  %d", p1Score);
//    gotoxy(term.length-20, term.startY);
//    printf("Player2:  %d", p2Score);
    drawScore(p1Score, MINX+5, MINY+3);
    drawScore(p2Score, MAXX-15, MINY+3);
}


Terminal initTerminal()
{
    Terminal term;
    term.startX = 0;
    term.startY = MAXY+3;
    term.lastX = term.startX;
    term.lastY = term.startY;
    term.height = TERMH;
    term.length = MAXX;
    return term;
}

void updateBallPos(Ball &b, int &oldX, int &oldY)
{
    oldX = round(b.x);
    oldY = round(b.y);
    b.x += b.dir*b.speed;
    b.y = getY(b);
}

int mainGameLoop()
{
    Ball b;
    Platform plat1, plat2;
    Terminal term;

    term = initTerminal();
    int state = 0;
    int oldX, oldY;
    int dir = -1;
    int wait = 20;
    int result = 0;
    int p1Score = 0;
    int p2Score = 0;
    int scoreLimit = 7;
    int isRunning = 1;
    while (isRunning)
    {
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
        printResult(term, p1Score, p2Score);
        if (p1Score >= scoreLimit)
        {
            isRunning = 0;
            asciiPr("||p1Win||", (MINX+MAXX)/2 - 37, (MINY+MAXY)/2 - 3);
            Sleep(2000);
            fflush(stdin);
            gotoxy((MAXX-28), (MAXY-2));
            printf("<Press ENTER to continue>");
            while (handleNav(state, 0) != 1);
        }
        else if (p2Score >= scoreLimit)
        {
            isRunning = 0;
            asciiPr("||p2Win||", (MINX+MAXX)/2 - 39, (MINY+MAXY)/2 - 3);
            Sleep(2000);
            fflush(stdin);
            gotoxy((MAXX-28), (MAXY-2));
            printf("<Press ENTER to continue>");
            while (handleNav(state, 0) != 1);
        }
        else {
            plat1 = initPlat(MINX);
            plat2 = initPlat(MAXX);
            drawPlat(plat1);
            drawPlat(plat2);
            b = genInitPos(dir);
            drawBall(b);
            Sleep(1337);
            int correctPos = 1;
            while (correctPos)
            {
                updateBallPos(b, oldX, oldY);
                if ((result = isScored(b)) != 0)
                {
                    scored(result);
                    correctPos = 0;
                }
                else
                {
                    keyboard(plat1, plat2);
                    handleInput(plat1, plat2);
                    handleBallMove(b, plat1, plat2);
                    gotoxy(oldX, oldY);
                    putch(b.prevChar);
                    //putch('1');
                    drawFigures(b, plat1, plat2);
                    Sleep(wait);
                }
            }
        }
    }
}

void gbMessage()
{
    int state;
    system("cls");
    asciiPr("||goodbye||", ((MINX+MAXX+1)/2-26), (MINY+MAXY+1)/3);
    fflush(stdin);
    gotoxy((MINX+MAXX-20), (MINY+MAXY-3));
    printf("<Press any key>");
    getch();
}

void initConsole()
{
    char mode[300];
    if (MAXX > 78 && MAXY > 28)
        sprintf(mode, "MODE CON: COLS=%d LINES=%d", (MINX+MAXX+1), (MINY+MAXY+TERMH+1));
    else
        sprintf(mode, "MODE CON: COLS=%d LINES=%d", 80, 28);
    system(mode);
    system("color 3B");
}

int main(int argc, char *argv[])
{
    initConsole();
    int isGame = 1;
    while (isGame)
    {
        if(!mainMenu())
        {
            gbMessage();
            isGame = 0;
        }
        else
        {
            mainGameLoop();
        }
    }
    return 0;
}
