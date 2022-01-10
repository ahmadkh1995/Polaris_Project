// System Header files
#include <iostream>

// Project Header files
#include "UserProxy.hpp"
#include "KeyboardHandler.hpp"
#include "Timer.hpp"

UserProxy::UserProxy(){
	
	// Set the User type (UserProxy interfaces with Generic User)
	this->userType = GENERIC_USER;

}

UserProxy::~UserProxy(){} 


/**
* @brief Allow selection of the action for the user
* @param the code of the chosen option
* @param the set of input arguments passed to the program
* @return forward the code of the chosen option
*/
int UserProxy::sendRequest(const int& req, const char** argv){
	// Execute the corresponding action, based on the user's choice
	if (req == PAIRING){ // Start Pairing procedure
		this->requestForPairing();
	}
	else if (req == REGISTRATION){ // Start Registration procedure
		this->requestForRegistration(argv);
	}
	else if (req == TRACKING){ // Start Computer-Assisted procedure
		this->requestForApplicatorTracking(argv);
	}

	return req;
}

/**
* @brief Method that forwards the request to start the Pairing procedure
*/
bool UserProxy::requestForPairing(){
	
	bool ret = false;

#if DEBUG
	std::cout << "[U] Forwarding Pairing procedure request ... " << std::endl;
#endif

	return ret;
}

/**
* @brief Method that forwards the request to start the Registration procedure
*/
bool UserProxy::requestForRegistration(const char** argv){

	bool ret = false;

#if DEBUG
	std::cout << "[U] Forwarding Registration procedure request ... " << std::endl;
#endif
	
	// 0. Init variables
	int collectedPoints = 0;
	bool exitCondition = false;
	bool threadBased = false;
	KeyboardHandler kb;

	// 1. Let the Controller init the Sensor

	// 2. If sensorOk, send three inputs from keyboard
	// 2.1 Run until you collect 3 repere points, OR user requests for exit
	// 2.2 If Space bar hit
	// 2.2.1 Save 
	// 2.2 If the acquired point is valid ...
	// 2.3 ... increase the counter of collected points
	// 3. If thread-based, let the Controller close the Sensor

	return ret;

}

/**
* @brief Method that forwards the request to start the Applicator procedure
*/
bool UserProxy::requestForApplicatorTracking(const char** argv){

#if DEBUG
	std::cout << "[U] Forwarding Applicator tracking procedure request ... " << std::endl;
#endif

	// Init variables
	double dt, et, rate, tic, toc, tac, Ts;
	bool ret = false;
	int key = 0;
	int toolOption = NULL;
	int appOption = NULL;
	bool kbhitFlag = false;
	double* data;
	KeyboardHandler kb;
	Timer clock;

	// Create a static instance of the coordinator Tracker class
	Tracker track;

	// Get the clock rate
	rate = clock.getRate();
	Ts = 1.0 / rate;

	// 1. Initialize the required external systems (Polaris Sensor)
	initExtSystems(&track, argv);

	// 2. Request for the Applicator type
	std::cout << "Select the applicator that you want to use: \n" << std::endl;
	std::vector<std::string> applicatorList = track.getApplicatorList();
	appOption = chooseFromList(applicatorList);

	// 3. Request for the Polaris tool type
	std::cout << "Select the Polaris tool that you want to track: \n" << std::endl;
	std::vector<std::string> polarisToolList = track.getSensorToolNames();
	toolOption = chooseFromList(polarisToolList);

	// 4. Create a shared memory to share data with the Polaris thread, before its creation
	track.createShdMem();

	// 5. Launch parallel thread to share data
	std::thread polarisThread = track.startToolTracking(toolOption);
	
	std::cout << "Tracking started. Press ESC when you want to stop tracking... " << std::endl;
	// 6. Wait for exit request from the user
	while (!track.isExitRequested()){

		// Measure starting time
		tic = clock.getCurTime();

		//----------------------------------------------------------------//
		// Do stuff here... 

		// If a key is pressed ...
		kbhitFlag = kb.kbhit();
		if (kbhitFlag){

			// Get the current value
			key = kb.getChar();

			// If the key is ESC, terminate
			if (key == ESCAPE){
				// 7. Send the exit request to the thread and exit
				track.exit();
			}
		}

		// 8. Read applicator pose data in Polaris marker frame
		data = track.readDataFromShdMem();

		// Print acquired data
		this->printToolData(data);

		// 9. Extract keypoints of the applicator surface (hard-coded CAD-dependent transformation)
		// 10. Transform keypoint from applicator local frame to external visulizer, through registered marker-visualizer transformation
		//----------------------------------------------------------------//

		// Measure the ending time and the elapsed time
		toc = clock.getCurTime();
		dt = clock.elapsedTime(tic, toc);

		// Wait until Ts
		if (dt < Ts){
			clock.timeSleep(Ts - dt);
		}

		// Measure the final time after sleep to check the actual rate of the thread
		tac = clock.getCurTime();
		et = clock.elapsedTime(tic, tac);

#ifdef DEBUG
		//std::cout << "[UP] Running rate " << 1.0 / et << " Hz" << std::endl;
#endif // DEBUG

	}

	// N. Join the threads
	polarisThread.join();

	return ret;
}

/**
* @brief Check the validity of the option chosen by the user in the Menu
* @param the code of the chosen option
* @return a flag indicating the validity
*/
bool UserProxy::checkOptionValidity(const int& op){

	return (op == PAIRING || op == REGISTRATION || op == TRACKING);

}


/**
* @brief Method that initializes the connected devices and let the user choose the tools
* @param a static instance of the Tracker class
* @param the input arguments of the program
*/
void UserProxy::initExtSystems(Tracker* trackPtr, const char** argv){
	
	// Init variables
	std::vector<std::string> toolNames;
	int toolOption = NULL;

	// 1. Connect to the devices (in this point presence check is already done)
	trackPtr->setPolarisPort(std::string(argv[1]));

	// 2. Initialize the Polaris
	trackPtr->initPolaris();

}

/**
* @brief Let the user choice an option in a given list
* @param the list of names
* @return the index of the chosen option
*/
int UserProxy::chooseFromList(const std::vector<std::string>& list){

	int toolOption = NULL;

	// 4. Let the User choose the tool to load
	while (toolOption == NULL || (toolOption <= 0 || toolOption > list.size())) {

		// Raise error message in case of wrong input
		if (toolOption != NULL){
			std::cout << "Error: " << wrongOptionMessage() << std::endl;
		}

		for (int i = 0; i < list.size(); i++){
			std::cout << "\t " << i + 1 << ": " << list[i] << std::endl;
		}

		// Acquire input from the user
		std::cin >> toolOption;

	}

	// Return the chosen option
	return toolOption;

}

/**
* @brief Print data acquired from Polaris (from static array)
* @param Acquired data (in form of static array)
*/
void UserProxy::printToolData(const double* data){

#ifdef DEBUG
	std::cout << "[User] data: \n" <<
		"position: " << data[0] << ", " << data[1] << ", " << data[2] << "; " <<
		"orientation (quat): " << data[3] << ", " << data[4] << ", " << data[5] << ", " << data[6] << "; " << std::endl << std::endl;
#endif //DEBUG

}


