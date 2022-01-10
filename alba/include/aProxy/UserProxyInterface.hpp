#ifndef USERPROXYINTERFACE_HPP_
#define USERPROXYINTERFACE_HPP_

class UserProxyInterface {

public:

	/**
	* @brief Default constructor of UserProxyInterface
	*/
	UserProxyInterface(){}

	/**
	* @brief Default destroyer of UserProxyInterface
	*/
	~UserProxyInterface(){}

	/**
	* @brief Allow selection of the action for the user
	* @param the code of the chosen option
	* @param the set of input arguments passed to the program
	* @return forward the code of the chosen option
	*/
	virtual int sendRequest(const int& req, const char** argv) = 0;

	/**
	* @brief Get the user type
	*/
	virtual int getUserType() = 0;

	/**
	* @brief Check if the user is a technician
	*/
	virtual bool isATechUser() = 0;


protected:

	/**
	* @brief Check the validity of the option chosen by the user in the Menu
	* @param the code of the chosen option
	* @return a flag indicating the validity
	*/
	virtual bool checkOptionValidity(const int& op) = 0;


};


#endif //USERPROXYINTERFACE_HPP_