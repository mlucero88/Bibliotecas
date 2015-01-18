/*
 * Configuracion.h
 *
 *  Created on: 19/09/2011
 *      Author: facundo
 *
 *  caso de uso
 *  
 *  Configuracion *t = Configuracion::getConfig();
  *  cout << t->getValorPorPrefijo(RUTA_VOTANTES);
 *
 */

#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_

#define RUTA_VOTANTES "<ruta_votante>"
#define RUTA_DISTRITOS "<ruta_distrito>"
#define RUTA_ELECCION "<ruta_eleccion>"
#define RUTA_LISTA "<ruta_lista>"
#define RUTA_CONTEO "<ruta_conteo>"
#define RUTA_CANDIDATO "<ruta_candidato>"
#define RUTA_CARGO "<ruta_cargo>"
#define RUTA_ADMINISTRADOR "<ruta_administrador>"
#define RUTA_IDs "<ruta_IDs>"
#define RUTA_HASH_ADMINISTRADOR_REGS "<ruta_hash_administrador_regs>"
#define RUTA_HASH_ADMINISTRADOR_BLOQ_LIB "<ruta_hash_administrador_bloq_lib>"
#define RUTA_HASH_ADMINISTRADOR_TABLA "<ruta_hash_administrador_tabla>"
#define RUTA_ARBOL_ELECCION "<ruta_arbol_eleccion>"
#define RUTA_ARBOL_LISTA "<ruta_arbol_lista>"
#define RUTA_ARBOL_CANDIDATO "<ruta_arbol_candidato>"
#define RUTA_HASH_DISTRITO_REGS "<ruta_hash_distrito_regs>"
#define RUTA_HASH_DISTRITO_BLOQ_LIB "<ruta_hash_distrito_bloq_lib>"
#define RUTA_HASH_DISTRITO_TABLA "<ruta_hash_distrito_tabla>"
#define RUTA_HASH_CARGO_REGS "<ruta_hash_cargo_regs>"
#define RUTA_HASH_CARGO_BLOQ_LIB "<ruta_hash_cargo_bloq_lib>"
#define RUTA_HASH_CARGO_TABLA "<ruta_hash_cargo_tabla>"
#define RUTA_HASH_VOTANTE_REGS "<ruta_hash_votante_regs>"
#define RUTA_HASH_VOTANTE_BLOQ_LIB "<ruta_hash_votante_bloq_lib>"
#define RUTA_HASH_VOTANTE_TABLA "<ruta_hash_votante_tabla>"
#define RUTA_HASH_IDELECCION_REGS "<ruta_hash_ideleccion_regs>"
#define RUTA_HASH_IDELECCION_BLOQ_LIB "<ruta_hash_ideleccion_bloq_lib>"
#define RUTA_HASH_IDELECCION_TABLA "<ruta_hash_ideleccion_tabla>"
#define RUTA_HASH_IDDISTRITO_REGS "<ruta_hash_iddistrito_regs>"
#define RUTA_HASH_IDDISTRITO_BLOQ_LIB "<ruta_hash_iddistrito_bloq_lib>"
#define RUTA_HASH_IDDISTRITO_TABLA "<ruta_hash_iddistrito_tabla>"
#define RUTA_HASH_IDCARGO_REGS "<ruta_hash_idcargo_regs>"
#define RUTA_HASH_IDCARGO_BLOQ_LIB "<ruta_hash_idcargo_bloq_lib>"
#define RUTA_HASH_IDCARGO_TABLA "<ruta_hash_idcargo_tabla>"
#define RUTA_HASH_IDVOTANTE_REGS "<ruta_hash_idvotante_regs>"
#define RUTA_HASH_IDVOTANTE_BLOQ_LIB "<ruta_hash_idvotante_bloq_lib>"
#define RUTA_HASH_IDVOTANTE_TABLA "<ruta_hash_idvotante_tabla>"
#define RUTA_HASH_IDCANDIDATO_REGS "<ruta_hash_idcandidato_regs>"
#define RUTA_HASH_IDCANDIDATO_BLOQ_LIB "<ruta_hash_idcandidato_bloq_lib>"
#define RUTA_HASH_IDCANDIDATO_TABLA "<ruta_hash_idcandidato_tabla>"
#define RUTA_HASH_IDLISTA_REGS "<ruta_hash_idlista_regs>"
#define RUTA_HASH_IDLISTA_BLOQ_LIB "<ruta_hash_idlista_bloq_lib>"
#define RUTA_HASH_IDLISTA_TABLA "<ruta_hash_idlista_tabla>"
#define RUTA_ARBOL_REPORTE_DISTRITO "<ruta_arbol_reporte_distrito>"
#define RUTA_ARBOL_REPORTE_ELECCION "<ruta_arbol_reporte_eleccion>"
#define RUTA_ARBOL_REPORTE_LISTA "<ruta_arbol_reporte_lista>"

#define CANT_CONF 45

#include <string>

using namespace std;

class Configuracion {
private:
		string constantes[CANT_CONF];
		string valores[CANT_CONF];
		Configuracion();
public:
		static const string URL_VOTANTES;
		static const string URL_DISTRITOS;
		static const string URL_ELECCION;
		static const string URL_LISTA;
		static const string URL_CONTEO;
		static const string URL_CANDIDATO;
		static const string URL_CARGO;
		static const string URL_ADMINISTRADOR;
		static const string URL_IDs;

		static Configuracion *getConfig();
        ~Configuracion();
        string getValorPorPrefijo(string prefijo);
};

#endif /* CONFIGURACION_H_ */

