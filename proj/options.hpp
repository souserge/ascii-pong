#ifndef OPTIONS_HPP_INCLUDED
#define OPTIONS_HPP_INCLUDED

void updateColor();
void updateSounds();
void updateField();
void updateAll();
int optionsMenu();
int diffSetMenu();
int gameParamsMenu();
int changeColor(int xPos, int yPos);
int changeSounds(int xPos, int yPos);
int changeInt(int xPos, int yPos, int low, int high, int &param);
int changeColor();
int checkColor(char *color);

#endif // OPTIONS_HPP_INCLUDED
