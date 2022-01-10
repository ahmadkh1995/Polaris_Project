#include "PolarisProxy.hpp"
#include "Timer.hpp"

#include<iostream>

#ifdef DEBUG
#include <iostream>
#endif
#ifdef _WIN32
#include <windows.h>
#elif unix
#endif

// Boost Header files
#include <boost/interprocess/shared_memory_object.hpp> // for shared_memory usage
#include <boost/interprocess/mapped_region.hpp> // for mapped region usage
using namespace boost::interprocess;

/**
* @brief Constructor of PolarisProxy with Port Number argument
*/
PolarisProxy::PolarisProxy() {}

/**
* @brief Default destroyer of PolarisProxy
*/
PolarisProxy::~PolarisProxy() {

	// Delete the dynamic pointer to the NDI Class
	delete capi;

}


/**
*@brief Initialization function
*/
void PolarisProxy::init() {

	// Dynamic instance of NDI Combined API Class
	capi = new CombinedApi();

	// Initialize the connected flag to false
	this->connected = false;

	// Initialize the BX2 support flag to false // TODO: Check what is the difference with BX
	this->apiSupportsBX2 = false;

	// Initialize the tracking flag
	this->tracking = false;

	this->tracking_vrep = false;

	// Set the samplingRate of the sensor to 20Hz
	this->samplingRate = 20.00;

	// Set the name of the shared memory to be used
	this->shdMemName = std::string("PolarisDataShdMem");

	// Check the connection with the Polaris sensor
	checkConnection();

	if (this->connected) {
		// Check the BX2 support for the current API version
		determineApiSupportForBX2();

		// Send the INIT command to the Polaris Sensor (aka Initialize the communication)
		int ret = capi->initialize();

#ifdef DEBUG
		std::cout << "Polaris Sensor " << ((ret == 0) ? "initialized successfully." : "not initialized. Error in initialize() function.") << std::endl;
#endif // DEBUG

	}
}



/**
*@brief Verify if the sensor is properly connected to the system and ready for communication (set available flag)
*/
void PolarisProxy::checkConnection() {

	// Try to connect
#ifdef DEBUG
	std::cout << "Trying to connect to " << this->serialPort << " ..." << std::endl;
#endif

	if (capi->connect(this->serialPort) != 0) {
#ifdef DEBUG
		std::cout << "Connection to Serial Port " << this->serialPort << " failed. Trying another port..." << std::endl;
#endif
	}
	else {
#ifdef DEBUG
		std::cout << "Connected to Serial Port " << this->serialPort << "!" << std::endl;
#endif
		this->connected = true;
	}

}


/**
* @brief Determines whether an NDI device supports the BX2 command by looking at the API revision
*/
void PolarisProxy::determineApiSupportForBX2()
{
	// Lookup the API revision
	std::string response = capi->getApiRevision();

	// Refer to the API guide for how to interpret the APIREV response
	char deviceFamily = response[0];
	int majorVersion = capi->stringToInt(response.substr(2, 3));

	// As of early 2017, the only NDI device supporting BX2 is the Vega
	// Vega is a Polaris device with API major version 003
	if (deviceFamily == 'G' && majorVersion >= 3)
	{
		this->apiSupportsBX2 = true;
#ifdef DEBUG
		std::cout << "This API version supports BX2." << std::endl;
#endif // DEBUG
	}
	else {
#ifdef DEBUG
		std::cout << "This API version DOES NOT support BX2." << std::endl;
#endif // DEBUG
	}
}



/**
*@brief Enable the tracking of a specific given tool
*@param index of the tool list
*/
void PolarisProxy::enableTracking() {

#ifdef DEBUG
	std::cout << "Inside the Polaris Proxy thread. " << std::endl;
#endif // DEBUG

	// Init time variables
	double dt, et, rate, tic, toc, tac, Ts;
	double* tdVal;
	Timer clock;

	// Open the shared memory (assumed to be already created)
	shared_memory_object shm_obj
	(open_only                    //only open
		, this->shdMemName.c_str()    //name
		, read_write                  //read-write mode
	);///*/


	  // Set tracking flag to true
	this->tracking = true;
	this->tracking_vrep = true;
	// Get the clock rate
	rate = clock.getRate();
	Ts = 1.0 / rate;

	// Start tracking
	capi->startTracking();



	while (this->tracking) {

		// Measure starting time
		tic = clock.getCurTime();

		//----------------------------------------------------------------//
		// Do stuff here... 

		std::vector<ToolData>td = (this->apiSupportsBX2) ? capi->getTrackingDataBX2("--6d=tools --3d=none --sensor=none --1d=buttons") : capi->getTrackingDataBX();
		/*
		std::vector<ToolData>send_data = (this->apiSupportsBX2) ? capi->getTrackingDataBX2("--6d=tools --3d=none --sensor=none --1d=buttons") : capi->getTrackingDataBX();
		std::vector<ToolData >td;
		std::vector<std::vector<ToolData>>save_data; //A vector of vector with ToolData types
		for (int i = 1; i >= 1; i++) {
			save_data[i][]=send_data; //store data in a vector of vector 

			//Check Tool data validity

			for (int k = 0; k < save_data[i].size(); k++)
			{
				if (save_data[i][k].transform.tx >= -400)  //Valid Range of ToolData
				{
					td = save_data[i]; //send new ToolData to Memory
				}
				else
				{
					std::cout << "Tool data is not Valid" << std::endl;
					td = save_data[i - 1]; //Send Previous ToolData to Memory

				}
			}

		}
		*/
			// Print the acquired data from ToolData structure (works only in DEBUG mode)
			this->printTrackingData(td);
			//	trackingDataPrint(td);
			std::cout << td.size() << std::endl;

			//Map the whole shared memory in this process
			mapped_region region(shm_obj, read_write);

			// Convert acquired data in a static array
			tdVal = this->toolData2StaticArray(td[0]);

			//Write on the shared memory // TODO: PUT A MUTEX HERE
			std::memcpy(region.get_address(), tdVal, sizeof(double)*POSE_DIM);


			// Print the acquired data from ToolData structure (works only in DEBUG mode)
			//this->printTrackingData(static_cast<double*>(region.get_address()));

			//----------------------------------------------------------------//

			// Measure the ending time and the elapsed time
			toc = clock.getCurTime();
			dt = clock.elapsedTime(tic, toc);

			// Wait until Ts
			if (dt < Ts) {
				clock.timeSleep(Ts - dt);
			}

			// Measure the final time after sleep to check the actual rate of the thread
			tac = clock.getCurTime();
			et = clock.elapsedTime(tic, tac);

		

#ifdef DEBUG
		//std::cout << "[PP] Running rate " << 1.0/et << " Hz" << std::endl;
#endif // DEBUG

	}


	// Stop tracking
	std::cout << "Stop Tracking";
	capi->stopTracking();


#ifdef DEBUG
	std::cout << "[PP] Exiting... " << std::endl;
#endif // DEBUG


}



void PolarisProxy::configureUserParameters()
{
#ifdef DEBUG
	std::cout << capi->getUserParameter("Param.User.String0") << std::endl;
#endif
	capi->setUserParameter("Param.User.String0", "customString");
#ifdef DEBUG
	std::cout << capi->getUserParameter("Param.User.String0") << std::endl;
#endif
	capi->setUserParameter("Param.User.String0", "");
}


void PolarisProxy::loadTool(const char* toolDefinitionFilePath) {

	// Request a port handle to load a passive tool into
	int portHandle = capi->portHandleRequest();

#ifdef DEBUG
	std::cout << "Polaris portHandle: " << portHandle << std::endl;
#endif // DEBUG


	// Load the .rom file using the previously obtained port handle
	capi->loadSromToPort(toolDefinitionFilePath, portHandle);

#ifdef DEBUG
	std::cout << "Tool loaded." << std::endl;
#endif // DEBUG


}


/**
* @brief Initialize and enable the loaded tools
*/
void PolarisProxy::toolInitAndEnable() {

#ifdef DEBUG
	std::cout << "Initializing and Enabling tools ... " << std::endl;
#endif //DEBUG	


	// Initialize and enable tools
	std::vector<PortHandleInfo> portHandles = capi->portHandleSearchRequest(PortHandleSearchRequestOption::NotInit);

#ifdef DEBUG
	std::cout << "portHandles.size(): " << portHandles.size() << std::endl;
#endif //DEBUG	

	for (int i = 0; i < portHandles.size(); i++)
	{
		capi->portHandleInitialize(portHandles[i].getPortHandle());
		capi->portHandleEnable(portHandles[i].getPortHandle());
	}

	// Print all enabled tools
	portHandles = capi->portHandleSearchRequest(PortHandleSearchRequestOption::Enabled);
#ifdef DEBUG
	for (int i = 0; i < portHandles.size(); i++)
	{
		std::cout << portHandles[i].toString() << std::endl;
	}
#endif // DEBUG


}

/**
* @brief Print Tracking data (from ToolData structure)
* @param Tracked data (in form of ToolData structure)
*/
void PolarisProxy::printTrackingData(const std::vector<ToolData>& td) {


#ifdef DEBUG
	std::cout << "[PP] " << std::endl;
#endif //DEBUG

	for (int i = 0; i < td.size(); i++) {
		

	#ifdef DEBUG
		std::cout << "Frame #" << i + 1 << ": " << std::endl;
		std::cout << "Position: " << td[i].transform.tx << ", " << td[i].transform.ty << ", " << td[i].transform.tz << " " << std::endl;
		std::cout << "Orientation (quat): " << td[i].transform.q0 << ", " << td[i].transform.qx << ", " << td[i].transform.qy << ", " << td[i].transform.qz << " " << std::endl << std::endl;
#endif //DEBUG

	}
}

/**
* @brief Print Tracking data (from static array)
* @param Tracked data (in form of static array)
*/
void PolarisProxy::printTrackingData(const double* val) {

#ifdef DEBUG
	std::cout << "Frame #" << 1 << ": " << std::endl;
	std::cout << "Position: " << val[0] << ", " << val[1] << ", " << val[2] << " " << std::endl;
	std::cout << "Orientation (quat): " << val[3] << ", " << val[4] << ", " << val[5] << ", " << val[6] << " " << std::endl << std::endl;
#endif //DEBUG


}

/**
* @brief Convert the Transform data stored in a ToolData structure to a static array
* @param the input ToolData structure
* @return the output static array
*/
double* PolarisProxy::toolData2StaticArray(const ToolData& td) {

	static double vec[POSE_DIM];

	vec[0] = td.transform.tx;
	vec[1] = td.transform.ty;
	vec[2] = td.transform.tz;
	vec[3] = td.transform.q0;
	vec[4] = td.transform.qx;
	vec[5] = td.transform.qy;
	vec[6] = td.transform.qz;

	return vec;
}


/**
*@brief Error Connection Message of the sensor
*@return the message string
*/
char* PolarisProxy::errorConnectionMessage() {


	return "[POLARIS] Simulator not detected. Cannot run the required functionalities.";

}



void PolarisProxy::trackingDataPrint(const ToolData& td) {

	// Print Transform Data
	std::cout << "x:" << td.transform.tx << "\t y:" << td.transform.ty << "\t z:" << td.transform.tz << "\t";
}



