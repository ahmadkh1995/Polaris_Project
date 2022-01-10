  #include<iostream>

// Project header files
#include "Tracker.hpp"

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

/**
*@brief Default constructor of the Tracker Class
*/
Tracker::Tracker(){

	// Initialize exitRequest to false
	this->exitRequest = false;

}

/**
*@brief Default destroyer of the Tracker Class
*/
Tracker::~Tracker(){}

/**
*@brief Start the tracking of the given tool
*/
std::thread Tracker::startToolTracking(const int toolID){

	std::thread th;

	// 1. Build the string for the path of the tool to load
	std::vector<std::string> toolList = pp.getToolNameList();
	std::string pathString = "./../../sroms/";
	std::string toolPath = pathString + toolList[toolID-1];

#ifdef DEBUG
	std::cout << "toolPath: " << toolPath << std::endl;
#endif // DEBUG

	// 2. Load the tool on the Polaris
	pp.loadTool(toolPath.c_str());

	// 3. Initialize and enable the tool
	pp.toolInitAndEnable();

	std::cout << "Start trackThread--->enableTracking \n";
	std::thread trackThread(&PolarisProxy::enableTracking, &pp);

	return trackThread;
}

/**
*@brief Create the shared memory on which read data from Polaris thread
*/
void Tracker::createShdMem(){

	// Remove eventual pre-existing shared memory with the same name
	shared_memory_object::remove(this->getPolarisShdMem().c_str());

	// Create the shared memory
	this->shdMem = shared_memory_object
		(create_only                          //only create
		, this->getPolarisShdMem().c_str()    //name
		, read_write                          //read-write mode
		);

	// Truncate the size of the shared memory
	this->shdMem.truncate(sizeof(double)*POSE_DIM);

}


/**
* @brief Read data from Polaris thread
* @return a static array containing the read data
*/
double* Tracker::readDataFromShdMem(){

	static double data[POSE_DIM];

	//Map the whole shared memory in this process
	mapped_region region(this->shdMem, read_write);

	// Copy data //TODO: PUT MUTEX HERE
	std::memcpy(data, static_cast<double*>(region.get_address()), sizeof(double)*POSE_DIM);

	// Return data
	return data;

}


/**
*@brief Remove the previously created shared memory on which read data from Polaris thread (delete shdMem member variable)
*/
void Tracker::removeShdMem(){

	this->shdMem.remove(pp.getShdMemName().c_str());


}

