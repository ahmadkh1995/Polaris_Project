#ifndef USERPROXY_HPP_
#define USERPROXY_HPP_

// Project Header files
#include "UserProxyInterface.hpp"
#include "Tracker.hpp"
#include "utils.hpp"

// Action type defines
#define PAIRING 1
#define REGISTRATION 2
#define TRACKING 3
#define QUIT_INPUT 0

class UserProxy : public UserProxyInterface {

public:

	/**
	* @brief Default constructor of UserProxy
	*/
	UserProxy();

	/**
	* @brief Default destroyer of UserProxy
	*/
	~UserProxy();

	/**
	* @brief Get the user type
	*/
	inline int getUserType() { return userType; }

	/**
	* @brief Allow selection of the action for the user
	* @param the code of the chosen option
	* @param the set of input arguments passed to the program
	* @return forward the code of the chosen option
	*/
	int sendRequest(const int& req, const char** argv);

	/**
	* @brief Method that forwards the request to start the Pairing procedure
	*/
	bool requestForPairing();

	/**
	* @brief Method that forwards the request to start the Registration procedure
	* @param the set of input arguments passed to the program
	*/
	bool requestForRegistration(const char** argv);

	/**
	* @brief Method that forwards the request to start the Applicator tracking procedure
	* @param the set of input arguments passed to the program
	*/
	bool requestForApplicatorTracking(const char** argv);

	/**
	* @brief Check if the user is a technician
	*/
	inline bool isATechUser(){ return false; }

	/**
	* @brief Method that initializes the connected devices
	* @param a static instance of the Tracker class
	* @param the input arguments of the program
	*/
	void initExtSystems(Tracker* trackPtr, const char** argv);

	/**
	* @brief Let the user choice an option in a given list
	* @param the list of names 
	* @return the index of the chosen option
	*/
	int chooseFromList(const std::vector<std::string>& list);

	/**
	* @brief Print data acquired from Polaris (from static array)
	* @param Acquired data (in form of static array)
	*/
	void printToolData(const double* data);

protected:

	int userType;				//!< Type of the user (Generic or Technician)

	/**
	* @brief Check the validity of the option chosen by the user in the Menu
	* @param the code of the chosen option
	* @return a flag indicating the validity
	*/
	bool checkOptionValidity(const int& op);



};

#endif //USERPROXY_HPP_