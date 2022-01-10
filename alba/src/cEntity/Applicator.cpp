// Project Header files
#include "Applicator.hpp"

/**
* @brief Constructor of the Applicator Class
* @param ID index of the applicator (default is TYPE_1)
*/
Applicator::Applicator(const int& typeID_){

	this->typeID = typeID_;

	this->appList.push_back("TYPE 1");
	this->appList.push_back("TYPE 2");
	this->appList.push_back("TYPE 3");

}

/**
* @brief Default destroyer of the Applicator Class
*/
Applicator::~Applicator(){}