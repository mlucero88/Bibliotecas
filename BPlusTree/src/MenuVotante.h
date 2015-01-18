/*
 * MenuVotante.h
 *
 *  Created on: 07/10/2011
 *      Author: Miguel Torres
 */

#ifndef MENUVOTANTE_H_
#define MENUVOTANTE_H_

#include <iostream>
#include <cstdlib>
#include "Votante.h"
#include "Eleccion.h"
#include "Lista.h"
#include "Candidato.h"
#include "DataGetter.h"
#include "ABMEntidades/ABMentidades.h"
#include <vector>

using namespace std;

class MenuVotante {
private:

	Votante *votante;

	Votante* getVotante();

	/*
	 * Muestra el Menu Princiapal de Votante
	 */
	void opciones();

	void mostrarMenuElecciones();
	Eleccion* elegirEleccion();
	void mostrarMenuDatos();
	void mostrarListasPresentadas(Eleccion *eleccion);

	/*
	 * Confirma la votacion devolviendo TRUE si se confirma la
	 * Lista a votar.
	 */
	bool confirmarVotacion(Eleccion *eleccion,Lista *lista);
	Lista* elegirLista(Eleccion *eleccion);

	/*
	 * Guarda los cambios de los datos del Votante modificados
	 */
	void guardarCambios();


	void cambiarClave();
	void cambiarDomicilio();

public:
	MenuVotante(Votante *votante);

	virtual ~MenuVotante();
};

#endif /* MENUVOTANTE_H_ */
