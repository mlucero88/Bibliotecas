/*
 * DataGetter.h
 *
 *  Created on: 13/10/2011
 *      Author: administrador
 */

#ifndef DATAGETTER_H_
#define DATAGETTER_H_

#include <vector>
#include "Votante.h"
#include "Eleccion.h"
#include "Lista.h"
#include "Distrito.h"
#include "Conteo.h"
#include "ArbolBMas.h"
#include "hash_extensible.h"

class DataGetter {
public:
	DataGetter();
	virtual ~DataGetter();

	/* Se le pasa una eleccion, y un vector vacio de punteros a listas (vecListas) donde se retornaran las listas de esa eleccion */
	/* Retorna TRUE si se encontro por lo menos una lista, retorna FALSE si no se encontro ninguna */
	static bool getListasPorEleccion(vector<Lista*> & vecListas, Eleccion &eleccion);

	/* Se le pasa un vector vacio de punteros a elecciones (vecElecciones) donde se devolveran los resultados,
	 * una fecha en formato normal (dd/mm/aaaa o ddmmaaaa) y el votante que pide las elecciones. */
	/* Retorna TRUE si se encontro por lo menos una eleccion, retorna FALSE si no se encontro ninguna */
	static bool getEleccionesPorFechayDistrito(vector<Eleccion*> & vecElecciones,string fecha, Votante &votante);


	//Metodos para Informes

	static void getConteosPorDistrito(vector<Conteo *> & conteos, Distrito& distrito);

	static void getConteosPorLista(vector<Conteo *> & conteos, Lista& lista);

	static void getConteosPorEleccion(vector<Conteo *> & conteos, Eleccion& eleccion);

};

#endif /* DATAGETTER_H_ */
