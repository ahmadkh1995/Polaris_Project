// System Header files
#include <iostream>

// Project Header files
#include "utils.hpp"
#include "UserProxy.hpp"
#include "TechUserProxy.hpp"

#include "PolarisProxy.hpp"
#include "SimulatorProxy.hpp"



/**
* @brief Show the available functionalities for the user
*/
void printMenu(const int& userType, const bool& polarisOK, const bool& simulatorOK) {

	std::cout << ""
		"\n\n\nPlease select the desired functionality: ('*' ---> not available) \n"
		"\t 1. --- " << ((!polarisOK) ? "*" : "") << std::endl <<
		"\t 2. Registration " << ((!polarisOK) ? "*" : "") << std::endl <<
		"\t 3. Applicator Tracking " << ((!polarisOK) ? "*" : "") << std::endl;

	if (userType == TECH_USER) {
		std::cout << "\t 4. Applicator Tracking and Simulated visualization" << ((!simulatorOK || !polarisOK) ? "*" : "") << std::endl;
	}


	std::cout << "\n\t 0. Exit the program." << std::endl;
}


/**
* @brief Allow selection of the action for the user
* @return the code of the chosen option
*/
int Menu(const int& userType, const bool& polarisOK, const bool& simulatorOK) {

	// Print user options
	printMenu(userType, polarisOK, simulatorOK);


	// Consider also the case of a tech user
	int maxentry = (userType == GENERIC_USER) ? 3 : 4;

	// Iterate until a correct option is selected
	int option = -1000;
	while (option < 0 || option > maxentry) {

		if (option != -1000) {
			std::cout << wrongInputMessage() << std::endl;
			printMenu(userType, polarisOK, simulatorOK);
		}

		// Wait for user input
		std::cin >> option;

	}

	// Check if the chosen option refers to a not available functionality
	if (!polarisOK && (option == PAIRING || option == REGISTRATION || option == TRACKING) ||
		(!simulatorOK && option == TRACK_SIMULATOR)) {

		// Raise an error message
		std::cout << notAvailableOptionMessage() << std::endl;
		return -1;
	}
	else { // The chosen option is correct
		return option;
	}
}

int main(int argc, const char** argv) {

	int userType = -1;
	int actionInput = -1;
	std::string serialPort;
	std::string simIPAddress;

	// Validate the number of arguments received
	// Polaris Vicra sensor accepts COMX serial port as first argument to connect, while Polaris Vega sensor requires zeroconf hostname string PX-XXXXX.local.
	// Default setting is for Polaris Vega
	serialPort = (argc >= 2) ? (std::string(argv[1])) : (std::string("P9-01528.local"));
	simIPAddress = (argc == 3) ? (std::string(argv[2])) : (std::string(""));

	/* Probably using an interface is not the best idea to implement the pair of users considered for this application.
	* Consider the idea to use two separate classes for Generic and Tech Users.
	*/
	UserProxyInterface *up = NULL;
	PolarisProxy *pp = NULL;
	SimulatorProxy *sp = NULL;

	std::cout << "Alba Thermal Applicator Tracking Software.\n"
		"Marco Ferro, all rights reserved, 2019." << std::endl;
	std::cout << welcomeMessage();

	// Iterate until correct input is inserted
	while (userType != GENERIC_USER && userType != TECH_USER) {

		// Wait for input from user
		std::cin >> userType;

		// Select the User mode
		switch (userType) {
		case GENERIC_USER: // No access to simulator functionalities
			up = new UserProxy();
			std::cout << "Accessed to the system as Generic User." << std::endl;
			break;
		case TECH_USER: // Allows access to simulator functionalities
			up = new TechUserProxy();
			std::cout << "Accessed to the system as Tech User." << std::endl;
			break;
		default:
			std::cout << "Wrong input.\n" << welcomeMessage() << std::endl;
			break;
		}

	}

	// Instantiate the Proxy for the Polaris and the Simulator sensor
	pp = new PolarisProxy();
	pp->setPort(serialPort);
	pp->init();

	sp = new SimulatorProxy();
	sp->setIP(simIPAddress);
	sp->init();

	// Check which sensor is available
	bool isPolarisPresent = pp->isSensorConnected();
	bool isSimulatorPresent = sp->isSensorConnected();

	isSimulatorPresent = true;

	if (!isPolarisPresent) { // If the Polaris is not available, raise an error message
		std::cerr << pp->errorConnectionMessage() << std::endl;
	}
	if (!isSimulatorPresent) { // If the Simulator is not available, raise an error message
		std::cerr << sp->errorConnectionMessage() << std::endl;
	}

	// PolarisProxy and SimulatorProxy instantiated here are no longer required (they are needed only to verivy a priori the available functionalities)
	// New instances of the classes will be declared at Coordinator level in each implemented functionality
	delete pp;
	delete sp;

	while (actionInput < 0) { // Iterate until proper input is selected by the user
							  // Show the functionalities allowed for the user and send the corresponding request
		actionInput = Menu(userType, isPolarisPresent, isSimulatorPresent);
	}

	if (actionInput != QUIT_INPUT) { // If the user does not want to close the program, send the desired request
									 // Send request for the required functionality
		up->sendRequest(actionInput, argv);
	}
	else {

		// Dismiss the program
		std::cout << dismissalMessage() << std::endl;

	}

	// Clear dynamic structures
	delete up;


	system("PAUSE");

	return 0;
}