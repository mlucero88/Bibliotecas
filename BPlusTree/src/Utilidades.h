/*
 * Utilidades.h
 *
 *  Created on: 21/09/2011
 *      Author: facundo
 */

#ifndef UTILIDADES_H_
#define UTILIDADES_H_

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>


#define MAX_STRING_BUFFER = 500;

using namespace std;

class Utilidades {
public:
	Utilidades();
	virtual ~Utilidades();
	static string getFechaYHora();
	static string toString(long int a);
	static string toString(int a);
	static string toString(string::size_type a);
	static long toLong(string a);
	static int toInt(string a);
	static string indexarFecha(string fecha);
	static void formatearClave(string &clave);

	static unsigned int getDNIaleatorio(int semilla);

	//La idea es que guarde en un archivo modo binario
	//primero la longitud de la cadena como int y luego
	//la cadena misma como char *
	static void stringToFile(std::string & str, std::ofstream & ofs);

	//La idea es que lea de un archivo modo binario
	//primero la longitud de la cadena como un int
	//y luego lea la cadena misma teniendo en cuenta el largo
	//guardado en el int leido primero
	static string stringFromFile(std::ifstream & ifs);

	static void stringToFile(std::string & str, std::fstream & ofs);

	static string stringFromFile(std::fstream & ifs);

	static void sleep(unsigned int mseconds);

	static string obtenerClaveDistrito(const string &nombreDistrito);
	static string obtenerClaveCargo(const string &cargoPrincipal);
	static string obtenerClaveEleccion(const string &fecha, const string &cargoPrincipal);
	static string obtenerClaveLista(const string &fecha, const string &cargoPrincipal, const string &nombreLista);
	static string obtenerClaveVotante(const int &DNI);
	static string obtenerClaveCandidato(const string &fecha, const string &cargoPrincipal, const string &nombreLista, const int &DNI);
	static string obtenerClaveAdministrador(const string &usuario);

};

#endif /* UTILIDADES_H_ */
