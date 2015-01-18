/*
 * Logger.h
 *
 *  Created on: 19/09/2011
 *      Author: facundo
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "Logueable.h"
#include "Utilidades.h"
#include "VotoElectronicoExcepcion.h"
#include <fstream>
#include <ctime>
#include <string>

using namespace std;

class Logger {
private:
	static void logAccion(Logueable & obj, string accion);
	Logger();
	virtual ~Logger();
public:
	static void Alta(Logueable & obj);
	static void Modificacion(Logueable & obj);
	static void Eliminar(Logueable & obj);

	static void CambioDeVoto(Logueable &votante, int dni);
	static void ConfirmacionDeVoto(Logueable &votante, int dni);
	static void Voto(Logueable &votante, int dni);

	//Configuracion::getValorPorPrefijo(RUTA_ARCHIVO_VOTAR);
};

#endif /* LOGGER_H_ */
