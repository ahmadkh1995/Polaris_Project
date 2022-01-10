
#include "SimulatorProxy.hpp"
#include"TechUserProxy.hpp"

/*
---------------------------------------------------------------------------------------------------------
For Connecting the project to V-Rep we should add following directories to
the "Additional Include Directories"

C:\Program Files\V-REP3\V-REP_PRO_EDU\programming\remoteApi
C:\Program Files\V-REP3\V-REP_PRO_EDU\programming\include

also we should add theses lines to preprocessor definition in Project properties section:
(because extApi might get parsed/compiled before your file where you have the definition)

-   NON_MATLAB_PARSING
-   DO_NOT_USE_SHARED_MEMORY
-   MAX_EXT_API_CONNECTIONS=255
_   CRT_SECURE_NO_WARNINGS

more details: http://www.coppeliarobotics.com/helpFiles/en/remoteApiClientSide.htm
-----------------------------------------------------------------------------------------------
*/

#include <conio.h>
#include "windows.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


extern "C" {
#include "extApi.h"
}

/**
* @brief Default constructor of SimulatorProxy
*/
SimulatorProxy::SimulatorProxy() {

	this->connected = false;

}


/**
* @brief Default destroyer of SimulatorProxy
*/
SimulatorProxy::~SimulatorProxy() {

}

/**
*@brief Initialization function
*/
void SimulatorProxy::init() {

	simxFinish(-1);

	int userclientID = simxStart((simxChar*)"127.0.0.1", 19997, true, true, 50, 5);  //!< Main connection to V-REP   // wait 5 seconds to connect to vrep
	simxInt syncho = simxSynchronous(userclientID, true);  //Synchronous with client
	SimulatorProxy::clientID = userclientID;

	if (userclientID != -1)  // if Connection status is OK
	{
		cout << " Connection with V-REP established" << endl;
	}
	else
	{
		cout << " Connection with V-REP failed" << endl;
	}
	/*
	simxFinish(-1);                 //! Close any previously unfinished business
	int clientID1 = 0;
	clientID1 = simxStart((simxChar*)"127.0.0.1", 19997, true, true, 5000, 5);  //!< Main connection to V-REP   // wait 5 seconds to connect to vrep

	cout << "ClientID=" << clientID1 << "\n";

	Sleep(1000);
	if (clientID1 != -1)  // if Connection status is OK
	{
		cout << " Connection with V-REP established" << endl;
		simxInt syncho = simxSynchronous(clientID1, true);  //Synchronous with client
		/*
		int sim_status;
		sim_status = simxStartSimulation(clientID1, simx_opmode_oneshot); //start simulation in V-rep 
		cout << "Simulation status: " << sim_status << endl;
		if (sim_status == 1) {

			cout << "Simulation Started ..." << endl;
		}
		else {
			cout << "Simulation Not Started ..." << endl;
		}
	

	}

	else
	{
		cout << " Connection status to VREP: FAILED" << endl;
	}
	simxFinish(clientID1);

	*/

    }


void SimulatorProxy::setSimulatedToolData(double *data,int error_flag) {


	int userclientID = SimulatorProxy::clientID;
		
	//	simxStartSimulation(userclientID, simx_opmode_oneshot); //start simulation in V-rep 

		int polaris_probe = 0;
		simxGetObjectHandle(userclientID, "Dummy", &polaris_probe, simx_opmode_oneshot_wait);  // Get Shape(RigidBody)

		int polaris_sensor = 0;
		simxGetObjectHandle(userclientID, "Dummy0", &polaris_sensor, simx_opmode_oneshot_wait);  // Get Shape(RigidBody)

		//Position(X,Y,Z format)  data
		double	tx = data[0]/1000 ;       //td.transform.tx
		double	ty = data[1] /1000;      //td.transform.ty
		double	tz = data[2] /1000;     //td.transform.tz

		//Orientation (Quaternion format)  data
		double	Q0 = data[3] ;     //td.transform.q0
		double	Qx = data[4] ;       //td.transform.qx
		double	Qy = data[5] ;      //td.transform.qy
		double	Qz =data[6] ;     //td.transform.qz

		std::cout << "Error Flag : " << error_flag << std::endl;

		if (error_flag == 1) {
			int signal_value = 1;
			simxSetIntegerSignal(userclientID, "Error_flag", signal_value, simx_opmode_oneshot_wait);	
		}
		if (error_flag == 0) {
			int signal_value = 0;
			simxSetIntegerSignal(userclientID, "Error_flag", signal_value, simx_opmode_oneshot_wait);
		}

	    	std::cout << "Position Data  :" << "X: "<< tx << "\t" << "Y: " <<ty << "\t" << "Z: " << tz << std::endl;

			std::cout << "Orientation Data  :" << "q0: " <<Q0 << "\t" << "qx: " << Qx << "\t" << "qy: " << Qy << "\t" << "qz: " << Qz << std::endl;

			const simxFloat my_position[3] = { tx,ty,tz }; // position of Object(Shape) in  X,Y,Z format 

			const simxFloat my_orientation[4]= {Qx,Qy,Qz,Q0 }; // orientation of Object(Shape) in  {qx,qy,qz,w} "Quaternion " format

			simxSetObjectPosition(userclientID, polaris_probe, polaris_sensor, my_position, simx_opmode_oneshot_wait);  // Set new Object Position

			simxSetObjectQuaternion(userclientID, polaris_probe, polaris_sensor, my_orientation, simx_opmode_oneshot_wait);  // Set new Object Position

		

}



/**
*@brief Verify if the sensor is properly connected to the system and ready for communication (set available flag)
*@return flag stating if the sensor is present
*/
void SimulatorProxy::checkConnection() {


}


/**
*@brief Error Connection Message of the sensor
*@return the message string
*/
char* SimulatorProxy::errorConnectionMessage() {


	return "[SIMULATOR] Simulator not detected. Cannot run the required functionalities.";

}