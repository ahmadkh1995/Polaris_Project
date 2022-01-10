#ifndef KEYBOARDHANDLER_HPP_
#define KEYBOARDHANDLER_HPP_

class KeyboardHandler{

public:

	/**
	* @brief Default constructor of KeyboardHandler
	*/
	KeyboardHandler();

	/**
	* @brief Default destroyer of KeyboardHandler
	*/
	~KeyboardHandler();

	/**
	* @brief Check if a key has been hit or not
	* @return boolean value
	*/
	bool kbhit();

	/**
	* @brief Get the current char key upon pressure
	* @return the pressed key
	*/
	int getChar();

private:

};


#endif //KEYBOARDHANDLER_HPP_