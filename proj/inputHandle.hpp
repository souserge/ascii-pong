#ifndef INPUTHANDLE_HPP_INCLUDED
#define INPUTHANDLE_HPP_INCLUDED

void keyboard(Platform &plat1, Platform &plat2);
void handleInput(Platform &plat1, Platform &plat2);
int handleNav(int &state, int noOfStates);
int helpMenu();
int optionsMenu();
int mainMenu();

#endif // INPUTHANDLE_HPP_INCLUDED
