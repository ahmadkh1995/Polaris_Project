#ifndef TECHUSERPROXY_HPP_
#define TECHUSERPROXY_HPP_

// Project Header files
#include "UserProxy.hpp"
#include "SimulatedTracker.hpp"

// Action type define
#define TRACK_SIMULATOR 4

class TechUserProxy : public UserProxy {

public:

	double* share_data;

	/**
	* @brief Default constructor of TechUserProxy
	*/
	TechUserProxy();

	/**
	* @brief Default destroyer of TechUserProxy
	*/
	~TechUserProxy();


	/**
	* @brief Allow selection of the action for the user
	* @param the code of the chosen option
	* @param the set of input arguments passed to the program
	* @return forward the code of the chosen option
	*/
	int sendRequest(const int& req, const char** argv);

	/**
	* @brief Method that forwards the request to track the Polaris markers and show them in the simulator
	* @param the set of input arguments passed to the program
	* @return if the requestef functionality has finished properly
	*/
	bool requestForTrackSimulator(const char** argv);

	/**
	* @brief Check if the user is a technician
	*/
	inline bool isATechUser(){ return true; }

	/**
	* @brief Method that initializes the connected devices and let the user choose the tools
	* @param a static instance of the Tracker class
	* @param the input arguments of the program
	*/
	void initExtSystems(SimulatedTracker* trackPtr, const char** argv);

private:

	/**
	* @brief Check the validity of the option chosen by the user in the Menu
	* @param the code of the chosen option
	* @return a flag indicating the validity
	*/
	bool checkOptionValidity(const int& op);


};


#endif //TECHUSERPROXY_HPP_
