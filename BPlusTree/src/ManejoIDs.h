/*
 * ManejoIDs.h
 *
 *  Created on: 11/10/2011
 *      Author: martin
 */

#ifndef MANEJOIDS_H_
#define MANEJOIDS_H_

#include <iostream>
#include <stdio.h>
#include "Configuracion.h"
using namespace std;

class ManejoIDs {
public:
	virtual ~ManejoIDs();
	static long int obtenerIDnuevo(string classname);
	static long int getContadorID(string classname);

private:
	ManejoIDs();
	static void guardarIDs();
	static void cargarIDs();
	static long int id_distrito;
	static long int id_eleccion;
	static long int id_cargo;
	static long int id_candidato;
	static long int id_lista;
	static long int id_votante;

};

#endif /* MANEJOIDS_H_ */
