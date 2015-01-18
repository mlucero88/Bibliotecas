/*
 * CargaInicial.h
 *
 *  Created on: 19/10/2011
 *      Author: facundo
 */

#ifndef CARGAINICIAL_H_
#define CARGAINICIAL_H_

#include "Utilidades.h"
#include "./Tests/UtilidadesTests.h"
#include "./ABMEntidades/ABMentidades.h"
#include "./ABMEntidades/ConsultaEntidades.h"
#include "Votante.h"

using namespace std;

class CargaInicial {
private:
	static const int cantidadVotantes = 100;
	static const int dniInicial = 33000000;

	CargaInicial();
	virtual ~CargaInicial();
	static void getVotantes(vector<Votante *> &votantes, vector<Distrito> &distritos);
public:
	static void ejecutar();
	static void getVotantes(vector<Votante *> &votantes);
	static vector<string> getFechasElecciones();
};

#endif /* CARGAINICIAL_H_ */
