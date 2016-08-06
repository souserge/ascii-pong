#ifndef DATACONTROL_HPP_INCLUDED
#define DATACONTROL_HPP_INCLUDED

void stopWav(void);
void playWav(char *filedest, int loop);
double fRand(double fMax);
void gotoxy( int column, int line);
char getxy(int x, int y);
void asciiPr(char *keyword, int x, int y);
int isInArray(char c, char *arr, int length);
int readSettings();
int takeSettings(char *value, int index);
int writeSettings();
int storeSettings(char *buffer, int index);
void combineArrs(char *arr1[], char *arr2[], int len);
void initConsole(void);

#endif // DATACONTROL_HPP_INCLUDED
