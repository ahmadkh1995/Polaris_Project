#ifndef APPLICATOR_HPP_
#define APPLICATOR_HPP_

// System header files
#include <vector>
#include <string>

enum APP_IDs{TYPE_1 = 1, TYPE_2, TYPE_3};

class Applicator{

public:

	/**
	* @brief Constructor of the Applicator Class
	* @param ID index of the applicator (default is TYPE_1)
	*/
	Applicator(const int& typeID_ = TYPE_1);

	/**
	* @brief Default destroyer of the Applicator Class
	*/
	~Applicator();

	/**
	*@brief Get function. Return the list of applicators
	*@return the list of applicators
	*/
	inline std::vector<std::string> getNamesList(){ return this->appList; }

private:

	int typeID;								//!< ID index of the applicator
	std::vector<std::string> appList;		//!< The List of available applicators // --- I DON'T LIKE THIS HERE

};

#endif //APPLICATOR_HPP_