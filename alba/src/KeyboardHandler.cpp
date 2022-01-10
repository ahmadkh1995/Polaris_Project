#include "KeyboardHandler.hpp"

#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#elif unix
#endif


/**
* @brief Default constructor of TechUserProxy
*/
KeyboardHandler::KeyboardHandler(){}

/**
* @brief Default destroyer of TechUserProxy
*/
KeyboardHandler::~KeyboardHandler(){}

/**
* @brief Check if a key has been hit or not
* @return boolean value
*/
bool KeyboardHandler::kbhit(){
	
	bool kbhitFlag;

#ifdef _WIN32
	kbhitFlag = (_kbhit() == 1) ? true : false;
#elif unix
	kbhitFlag = false;
#endif

	return kbhitFlag;

}

/**
* @brief Get the current char key upon pressure
* @return the pressed key
*/
int KeyboardHandler::getChar(){

	int key;

#ifdef _WIN32
	// Acquire hit character
	key = _getch();
#elif unix
	kbhitFlag = false;
#endif

	return key;
}
