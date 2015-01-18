/*
 * Grabable.h
 *
 *  Created on: 19/09/2011
 *      Author: facundo
 */

#ifndef GRABABLE_H_
#define GRABABLE_H_

#include <string>
#include <iostream>
#include <fstream>
#include "Logueable.h"

using namespace std;

class Grabable : public Logueable {
private:

public:
	virtual unsigned long int Guardar(ofstream & ofs) = 0;	// Devuelve el offset de donde lo grabo
	//virtual void Modificar() = 0;
	//virtual void Eliminar() = 0;
	virtual bool Leer(ifstream & ifs, unsigned long int offset) = 0;

	virtual int getTamanioEnDisco() = 0;

	virtual string getURLArchivoDatos() = 0;

	//Es la interface Loggueable q heredo
	virtual string getClassName() = 0;
};

#endif /* GRABABLE_H_ */
