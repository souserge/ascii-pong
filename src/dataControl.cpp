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

void stopWav(void) {
    sndPlaySound(NULL, SND_ASYNC);
}

void playWav(char *filedest, int loop) {
    stopWav();
    if (sounds == OFF)
    {
        return;
    }
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

int isInArray(char c, char *arr, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (c == arr[i])
            return 1;
    }
    return 0;
}



int readSettings() {
    char buffer[1000], value[100];
    int i = 0;
    FILE *settings = fopen("options.txt", "r");
    if (settings == NULL) {
        fclose(settings);
        return 0;
    }
    while (fgets(buffer, sizeof(buffer), settings) != NULL && i < NUM_OF_PARAMS) {
        sscanf(buffer, "%*s %s", value);
        if(!takeSettings(value, i)) {
            fclose(settings);
            return 0;
        }
        i++;
    }
    fclose(settings);
    if (i != NUM_OF_PARAMS)
        return 0;
    return 1;
}


int takeSettings(char *value, int index) {
    int counter = 0, intValues = 8, doubleValues = 1, charArrValues = 1;
    int *intValuesArr[] = {&aiLevel, &aiType, &wait, &platLen, &scoreLimit, &width, &height, &sounds};
    double *doubleValuesArr[] = {&ballSpeed};
    char *charArrValuesArr[] = {colorBF};

    if (index < (counter + intValues)) {
        *intValuesArr[index] = atoi(value);
    }
    else if (index < (counter += intValues) + doubleValues) {
        *doubleValuesArr[index-counter] = atof(value);
    }
    else if (index < (counter += doubleValues) + charArrValues) {
        strcpy(charArrValuesArr[index-counter], value);
    }
    else
        return 0;
    return 1;
}

int writeSettings() {
    char names[][50] = {"AI_TYPE", "AI_LEVEL", "GAME_SPEED", "PADDLE_LENGTH", "SCORE_LIMIT",
                        "FIELD_WIDTH", "FIELD_HEIGHT", "SOUNDS", "BALL_SPEED", "COLOR_BF"};
    char buffer[1000], values[100], temp[1000];
    int i = 0;
    FILE *settings = fopen("options.txt", "w+");
    if (settings == NULL) {
        fclose(settings);
        return 0;
    }
    for (i = 0; i < NUM_OF_PARAMS; i++) {
        if (!storeSettings(temp, i)) {
            fclose(settings);
            return 0;
        }
        sprintf(buffer, "%s: %s\n", names[i], temp);
        fputs(buffer, settings);
    }
    fclose(settings);
    return 1;
}

int storeSettings(char *buffer, int index) {
    int counter = 0, i, intValues = 8, doubleValues = 1, charArrValues = 1;
    int *intValuesArr[] = {&aiLevel, &aiType, &wait, &platLen, &scoreLimit, &width, &height, &sounds};
    double *doubleValuesArr[] = {&ballSpeed};
    char *charArrValuesArr[] = {colorBF};

    if (index < (intValues)) {
        itoa(*intValuesArr[index], buffer, 10);
    }
    else if (index < ((counter += intValues) + doubleValues)) {
        sprintf(buffer, "%lf", *doubleValuesArr[index-counter]);
    }
    else if (index < ((counter += doubleValues) + charArrValues)) {
        strcpy(buffer, charArrValuesArr[index-counter]);
    }
    else
        return 0;
    return 1;
}

void combineArrs(char *arr1[], char *arr2[], int len)
{
    for (int i = 0; i < len; i++)
    {
        strcat(arr1[i], ": ");
        strcat(arr1[i], arr2[i]);
    }
}
void initConsole()
{
    system("TITLE ASCII PONG");

    char mode[128], color[128];
    sprintf(mode, "MODE CON: COLS=%d LINES=%d", WID, HEI);
    sprintf(color, "COLOR %s", colorBF);
    system(mode);
    system(color);
}
