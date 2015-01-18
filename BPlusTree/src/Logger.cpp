/*
 * Logger.cpp
 *
 *  Created on: 19/09/2011
 *      Author: facundo
 */

#include "Logger.h"

void Logger::logAccion(Logueable & obj, string accion) {
	string tipoEntidad = obj.getClassName();

	string urlArchivoLog = "./log.txt";
	string log;
	ofstream archivoLog(urlArchivoLog.c_str(), ios::app); //Ver si debo abrirlo solo en modo escritura

	if(!archivoLog.is_open())
		throw VotoElectronicoExcepcion("No se pudo abrir el archivo de log" + urlArchivoLog);

	log =  "[" + Utilidades::getFechaYHora() + "]"+ ": " + obj.getClassName() + " - " + accion;

	archivoLog<<log;
	archivoLog<<endl;
}

void Logger::Alta(Logueable & obj) {
	logAccion(obj, "Alta");
}

void Logger::Eliminar(Logueable & obj) {
	logAccion(obj, "Eliminación");
}

void Logger::Modificacion(Logueable & obj) {
	logAccion(obj, "Modificación");
}

void Logger::CambioDeVoto(Logueable &votante, int dni)
{
	logAccion(votante, "El votante de dni " + Utilidades::toString(dni) + " cambio su voto");
}

void Logger::ConfirmacionDeVoto(Logueable &votante, int dni)
{
	logAccion(votante, "El votante de dni " + Utilidades::toString(dni) + " confirmo su voto");
}

void Logger::Voto(Logueable &votante, int dni)
{
	logAccion(votante, "El votante de dni " + Utilidades::toString(dni) + "  voto");
}

