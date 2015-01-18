/*
 * DataAccess.h
 *
 *  Created on: 03/10/2011
 *      Author: facundo
 */

#ifndef DATAACCESS_H_
#define DATAACCESS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Grabable.h"
#include "Logueable.h"
#include "Logger.h"
#include "Utilidades.h"
#include "VotoElectronicoExcepcion.h"
#include "Configuracion.h"


class DataAccess {

private:
	Configuracion & configuracion;
public:
	DataAccess();
	virtual ~DataAccess();

	unsigned long int Guardar(Grabable & obj);				// Retorna el offset de donde lo guardo.
	//void Eliminar(Grabable & obj);
	bool Leer(Grabable & obj, unsigned long int offset);


};

#endif /* DATAACCESS_H_ */
