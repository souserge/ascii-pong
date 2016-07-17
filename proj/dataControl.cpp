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

void initConsole()
{
    char mode[300];
    if (MAXX > 80 && MAXY > 35)
        sprintf(mode, "MODE CON: COLS=%d LINES=%d", (MINX+MAXX+1), (MINY+MAXY+1));
    else
        sprintf(mode, "MODE CON: COLS=%d LINES=%d", 80, 28);
    system(mode);
    system("TITLE ASCII Ball");
    system("COLOR 3B");
}
