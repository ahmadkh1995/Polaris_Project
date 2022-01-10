#ifndef UTILS_HPP_
#define UTILS_HPP_

// System include
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

#ifdef _WIN32
   // For Windows Sleep(ms)
   #include <windows.h>
#endif //_WIN32

// NDI Header files //TODO: IT MAY BE REDUNDANT INCLUSION. CONSIDER REMOVAL
#include "CombinedApi.h"

// User Types code
#define GENERIC_USER 1
#define TECH_USER 2

#define REPERE_POINTS_NUM 3

#define ESCAPE 27
#define SPACE_BAR 33

/**
* @brief Prints a debug message if a method call failed.
* @details To use, pass the method name and the error code returned by the method.
*          Eg: onErrorPrintDebugMessage("capi.initialize()", capi.initialize());
*          If the call succeeds, this method does nothing.
*          If the call fails, this method prints an error message to stdout.
*/
void onErrorPrintDebugMessage(const CombinedApi& capi, std::string methodName, int errorCode);

#ifdef _WIN32
/**
* @brief There's no standard cross platform sleep() method prior to C++11
*/
void sleepSeconds(unsigned numSeconds);
#endif


/** Utility messages**/ /// <--- Maybe these cane become exceptions (future development)
/**
* @brief Initial welcome message of the software
*/
char* welcomeMessage();

/**
* @brief show the wrong input message
*/
char* wrongInputMessage();

/**
* @brief show the not available option message
*/
char* notAvailableOptionMessage();

/**
* @brief show the not available tool name message
*/
char* wrongOptionMessage();

/**
* @brief show the dismissal message
*/
char* dismissalMessage();

#endif //UTILS_HPP_
