#ifndef OPTIONS_HPP_INCLUDED
#define OPTIONS_HPP_INCLUDED

void updateColor();
void updateSounds();
void updateField();
void updateAll();

int changeInt(int xPos, int yPos, int low, int high, int &param);
int changeState(int xPos, int yPos, int initState, char *statArr[], int len);

int changeAIType(int xPos, int yPos);
int changeAILevel(int xPos, int yPos);
int changeGameSpeed(int xPos, int yPos);
int changeBallSpeed(int xPos, int yPos);
int changePlatLen(int xPos, int yPos);
int diffSetMenu();

int changeSounds(int xPos, int yPos);
int changeColor(int xPos, int yPos);
int checkColor(char *color);
int gameParamsMenu();

int optionsMenu();

#endif // OPTIONS_HPP_INCLUDED
