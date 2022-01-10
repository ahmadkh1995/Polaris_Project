// System Header files
#include <iostream>


// Project Header files
#include "utils.hpp"

/**
* @brief Prints a debug message if a method call failed.
* @details To use, pass the method name and the error code returned by the method.
*          Eg: onErrorPrintDebugMessage("capi.initialize()", capi.initialize());
*          If the call succeeds, this method does nothing.
*          If the call fails, this method prints an error message to stdout.
*/
void onErrorPrintDebugMessage(const CombinedApi& capi, std::string methodName, int errorCode)
{
	if (errorCode < 0)
	{
		std::cout << methodName << " failed: " << capi.errorToString(errorCode) << std::endl;
	}
}

#ifdef _WIN32
/**
* @brief There's no standard cross platform sleep() method prior to C++11
*/
void sleepSeconds(unsigned numSeconds)
{
	Sleep((DWORD)1000 * numSeconds); // Sleep(ms)
}
#endif

/** Utility messages**/ /// <--- Maybe these cane become exceptions (future development)
char* welcomeMessage(){

	return "Please select your user role (General User as default)\n" 
		"\t1 General User\n"
		"\t2 Technician \n\n";

}

char* wrongInputMessage(){

	return "\nWrong input.Please insert a correct number for the corresponding action to request.\n ";
}

char* notAvailableOptionMessage(){

	return "\nThe chosen option is not available in the current system configuration. \n";

}

char* wrongOptionMessage(){

	return "\nThe choice does not correspond to an available option. Repeat your choice: \n";

}

/**
* @brief show the dismissal message
*/
char* dismissalMessage(){

	return "\nOne is glad to be of service.";

}

