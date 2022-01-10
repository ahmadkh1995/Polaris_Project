#ifndef SIMULATED_TRACKER_HPP_
#define SIMULATED_TRACKER_HPP_

// System Header files
#include <string>

// Project Header files
#include "PolarisProxy.hpp"
#include "SimulatorProxy.hpp"
#include "Tracker.hpp"

class SimulatedTracker : public Tracker{

public:

	double* recieved_data;



	/**
	*@brief Default constructor of the SimulatorVisualizer Class
	*/
	SimulatedTracker(){}

	/**
	*@brief Default destroyer of the SimulatorVisualizer Class
	*/
	~SimulatedTracker(){}

	/**
	*@brief Set function. Set the simulation port for the V-REP simulator
	*@param the simulator port
	*/
	inline void setSimIP(const std::string& ip_){ sp.setIP(ip_); }

	/**
	*@brief Initialize the V-REP simulator
	*/
	void initVREP();

 
	inline void sendDataToSimProxy(double* data,  int error_flag) {sp.setSimulatedToolData(data, error_flag) ;	}


private:

	SimulatorProxy sp;			//!< Static instance of the SimulatorProxy class

};


#endif //SIMULATED_TRACKER_HPP_
