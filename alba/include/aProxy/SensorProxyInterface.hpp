#ifndef SENSORPROXYINTERFACE_HPP_
#define SENSORPROXYINTERFACE_HPP_

// System Header file
#include <string>
#include <vector>
#include <thread>


#define TOOLS_NUM 3

class SensorProxyInterface{

public:

	/**
	* @brief Default constructor of SensorProxyInterface
	*/
	SensorProxyInterface(){
	
		this->toolList.push_back("8700338.rom");
		this->toolList.push_back("8700339.rom");
		this->toolList.push_back("8700340.rom");

	}

	/**
	* @brief Default destroyer of SensorProxyInterface
	*/
	~SensorProxyInterface(){}

	/**
	*@brief Error Connection Message of the sensor
	*@return the message string 
	*/
	virtual char* errorConnectionMessage() = 0;

	/**
	*@brief Initialization function
	*/
	virtual void init() = 0;

	/**
	*@brief Returns if the sensor is available
	*@return flag stating if the sensor is present
	*/
	inline bool isSensorConnected(){ return connected; };

	/**
	*@brief Get function. Return the list of tools
	*@return the list of tools
	*/
	inline std::vector<std::string> getToolNameList(){ return this->toolList; }

	/**
	*@brief Get function. Return the list of tools
	*@return the list of tools
	*/
	inline std::vector<int> getTrackedToolList(){ return this->trackedTools; }

protected:

	bool connected;							//!< If the sensor is available
	std::vector<std::string> toolList;		//!< The List of available tools to be tracked
	std::vector<int> trackedTools;			//!< Length-varying array of the indexes of the tracking tools

	/**
	*@brief Verify if the sensor is properly connected to the system and ready for communication
	*/
	virtual void checkConnection() = 0;


private:





};


#endif //SENSORPROXYINTERFACE_HPP_
