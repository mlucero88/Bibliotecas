/*
 * Distrito.cpp
 *
 *  Created on: 16/09/2011
 *      Author: Facundo Rossi
 */

#include "Distrito.h"

Distrito::Distrito(string nombre){
	this->_nombre = nombre;
	this->_id = 0;
}


/* Constructor que no se deberia usar. No guardar algo contruido asi. Desp se saca si no es necesario.*/
Distrito::Distrito()
{
	_id = -1;
	_nombre = "";
}


Distrito::Distrito(const Distrito &distrito) {
	this->_nombre = distrito._nombre;
	this->_id = distrito._id;
}


long Distrito::getId() {return _id;}


string Distrito::getNombre() {return this->_nombre;}


Distrito::~Distrito() {}


unsigned long int Distrito::Guardar(ofstream & ofs)
{
	unsigned long int offset = ofs.tellp();

	// Comienzo escritura de atributos
	ofs.write(reinterpret_cast<char *>(&_id), sizeof(_id));
	Utilidades::stringToFile(_nombre, ofs);

	return offset;
}


bool Distrito::Leer(ifstream & ifs, unsigned long int offset)
{
	// Me posiciono en el archivo
	ifs.seekg(offset,ios::beg);

	//Comienzo lectura de atributos
	ifs.read(reinterpret_cast<char *>(&_id), sizeof(_id));
	_nombre = Utilidades::stringFromFile(ifs);
	return true;
}


int Distrito::getTamanioEnDisco() {
	/* id + nroCaracteres + nombre */
	int size = _nombre.size();
	return (sizeof(_id) + sizeof(size) + sizeof(char) * size);
}


void Distrito::Imprimir()
{
	cout<<"Id Distrito: " << _id << endl;
	cout<<"Nombre Distrito: " << _nombre << endl << endl;
}


string Distrito::getURLArchivoDatos()
{
	return ((*Configuracion::getConfig()).getValorPorPrefijo(Configuracion::URL_DISTRITOS));
}


string Distrito::getClassName() {return "Distrito";}
