/*
 * Logueable.h
 *
 *  Created on: 19/09/2011
 *      Author: facundo
 */

#ifndef LOGUEABLE_H_
#define LOGUEABLE_H_
#include <string.h>

#include <string>

using namespace std;

class Logueable {
private:
	string className;
	string description;
public:
	virtual string getClassName() = 0;
};

#endif /* LOGUEABLE_H_ */
