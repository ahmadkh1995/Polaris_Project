#ifndef SIMULATORPROXY_HPP_
#define SIMULATORPROXY_HPP_

#include "SensorProxyInterface.hpp"


class SimulatorProxy : public SensorProxyInterface {

public:


	double* recieved_data;


	/**
	* @brief Default constructor of SimulatorProxy
	*/
	SimulatorProxy();

	/**
	* @brief Default destroyer of SimulatorProxy
	*/
	~SimulatorProxy();

	/**
	*@brief Initialization function
	*/
	void init();

	int clientID;

	void setSimulatedToolData(double* data, int error_flag);


	/**
	*@brief Error Connection Message of the sensor
	*@return the message string
	*/
	char* errorConnectionMessage();

	/**
	*@brief Set function. Set the simulation port for the V-REP simulator
	*@param the simulator port
	*/
	inline void setIP(const std::string& ip_){ this->IPaddress = ip_; }

private:


	std::string IPaddress;				//!< The IP Address on which the V-REP simulator can be found

	/**
	*@brief Verify if the sensor is properly connected to the system and ready for communication (set available flag)
	*@return flag stating if the sensor is present
	*/
	void checkConnection();




};


#endif //SIMULATORPROXY_HPP_