/*
 * ManejoIDs.cpp
 *
 *  Created on: 11/10/2011
 *      Author: martin
 */

#include "ManejoIDs.h"

ManejoIDs::ManejoIDs() {}

ManejoIDs::~ManejoIDs() {}

/* Guarda en el archivo de IDs los valores de ID de cada entidad */
/* Orden: id_distrito, id_eleccion, id_cargo, id_candidato, id_lista, id_votante */
void ManejoIDs::guardarIDs() {
	FILE* archivo = fopen((*Configuracion::getConfig()).getValorPorPrefijo(Configuracion::URL_IDs).c_str(),"wb");
	long int buffer[] = {id_distrito,id_eleccion,id_cargo,id_candidato,id_lista,id_votante};
	fwrite(buffer,sizeof(long int),6,archivo);
	fclose(archivo);
}

/* Carga del archivo de IDs los valores de ID de cada entidad */
/* Orden: id_distrito, id_eleccion, id_cargo, id_candidato, id_lista, id_votante */
void ManejoIDs::cargarIDs() {
	FILE* archivo = fopen((*Configuracion::getConfig()).getValorPorPrefijo(Configuracion::URL_IDs).c_str(),"rb");
	if (archivo == NULL) {
		ManejoIDs::guardarIDs();		// No existia el archivo, entonces lo creo.
		archivo = fopen((*Configuracion::getConfig()).getValorPorPrefijo(Configuracion::URL_IDs).c_str(),"rb");	// Lo abro una vez creado
	}
	long int buffer[6];
	fread(buffer,sizeof(long int),6,archivo);
	fclose(archivo);
	id_distrito = buffer[0];
	id_eleccion = buffer[1];
	id_cargo = buffer[2];
	id_candidato = buffer[3];
	id_lista = buffer[4];
	id_votante = buffer[5];
}

/* Incrementa primero el id de la entidad y luego retorna el valor (ya incrementado) */
long int ManejoIDs::obtenerIDnuevo(string classname) {
	ManejoIDs::cargarIDs();
	if (classname.compare("Distrito") == 0) {
		id_distrito++;
		ManejoIDs::guardarIDs();
		return id_distrito;
	}
	if (classname.compare("Eleccion") == 0) {
		id_eleccion++;
		ManejoIDs::guardarIDs();
		return id_eleccion;
	}
	if (classname.compare("Cargo") == 0) {
		id_cargo++;
		ManejoIDs::guardarIDs();
		return id_cargo;
	}
	if (classname.compare("Candidato") == 0) {
		id_candidato++;
		ManejoIDs::guardarIDs();
		return id_candidato;
	}
	if (classname.compare("Lista") == 0) {
		id_lista++;
		ManejoIDs::guardarIDs();
		return id_lista;
	}
	if (classname.compare("Votante") == 0) {
		id_votante++;
		ManejoIDs::guardarIDs();
		return id_votante;
	}
	return -1;																// O UNA EXCEPCION
}

/* Retorna el ultimo valor de ID que se uso para una entidad dada */
long int ManejoIDs::getContadorID(string classname) {
	ManejoIDs::cargarIDs();
	if (classname.compare("Distrito") == 0) return id_distrito;
	if (classname.compare("Eleccion") == 0) return id_eleccion;
	if (classname.compare("Cargo") == 0) return id_cargo;
	if (classname.compare("Candidato") == 0) return id_candidato;
	if (classname.compare("Lista") == 0) return id_lista;
	if (classname.compare("Votante") == 0) return id_votante;
	return -1;																// O UNA EXCEPCION
}

long int ManejoIDs::id_distrito = 0;
long int ManejoIDs::id_eleccion = 0;
long int ManejoIDs::id_cargo = 0;
long int ManejoIDs::id_candidato = 0;
long int ManejoIDs::id_lista = 0;
long int ManejoIDs::id_votante = 0;
