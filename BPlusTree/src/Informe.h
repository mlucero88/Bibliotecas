/*
 * Informe.h
 *
 *  Created on: 19/09/2011
 *      Author: facundo
 */

#ifndef INFORME_H_
#define INFORME_H_

#include <string>
#include <vector>
#include <fstream>
#include "Eleccion.h"
#include "Distrito.h"
#include "Lista.h"
#include "Conteo.h"

using namespace std;

class Informe {
public:
	Informe(Eleccion& eleccion);
        Informe(Eleccion& eleccion, string rutaInforme);

	Informe(Distrito& distrito);
        Informe(Distrito& distrito, string rutaInforme);

	Informe(Lista& lista);
        Informe(Lista& lista, string rutaInforme);


	virtual ~Informe();
};

#endif /* INFORME_H_ */
