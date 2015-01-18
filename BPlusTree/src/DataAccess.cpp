/*
 * DataAccess.cpp
 *
 *  Created on: 03/10/2011
 *      Author: facundo
 */

#include "DataAccess.h"

DataAccess::DataAccess() : configuracion(*Configuracion::getConfig()){
}

DataAccess::~DataAccess() {}


//Recibe el id del grabable q quiero leer, obj es una instacia vacia a sobreescribir con los
//datos leidos de la BD dentro del metodo
bool DataAccess::Leer(Grabable & obj, unsigned long int offset)
{
	string rutaArchivo = obj.getURLArchivoDatos();
	ifstream ifs(rutaArchivo.c_str(), ios::in | ios::binary);
	if(!ifs.is_open())
		throw VotoElectronicoExcepcion("No se pudo abrir el archivo de " + obj.getClassName());

	bool leido = obj.Leer(ifs, offset);

	ifs.close();
	return leido;
}

unsigned long int DataAccess::Guardar(Grabable & obj)
{
	string rutaArchivo = obj.getURLArchivoDatos();
	ofstream ofs(rutaArchivo.c_str(), ios::out | ios::app | ios::binary);	// abro para escribir siempre al final (ios::app)
	if(!ofs.is_open())
		throw VotoElectronicoExcepcion("No se pudo abrir el archivo de " + obj.getClassName());

	unsigned long int offset = obj.Guardar(ofs);

	ofs.close();

	return offset;
}

/*void DataAccess::Eliminar(Grabable & obj)
{
	string rutaArchivo = obj.getURLArchivoDatos();

	//NO USAR ELEMINAR HASTA NO TENER EL HASH o EL ARBOL

	ofstream ofs(rutaArchivo.c_str(), ios::binary);
	//Posicionarme en el ofstream mediante el hash, para luego borrar el registro
	//Luego borrar del hash el registro

	if(!ofs.is_open())  //o if(!ofs)
		throw VotoElectronicoExcepcion("No se pudo abrir el archivo de " + obj.getClassName());

	//obj.Eliminar(ofs);

	Logger::Eliminar(obj); //Se tiene q logguear alta y modificacion, ver como lo valido

	ofs.close();
}*/
