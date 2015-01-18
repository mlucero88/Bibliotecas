/*
 * menu.h
 *
 *  Created on: Oct 1, 2011
 *      Author: pablo
 */

#ifndef MENU_H_
#define MENU_H_

#include <string.h>
#include <vector>
#include <iostream>
#include <sstream>
#include "MenuVotante.h"
#include "ABMEntidades/ABMentidades.h"
#include "ABMEntidades/ConsultaEntidades.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include "DataAccess.h"
#include "DataGetter.h"
#include "Informe.h"
#include "./Eleccion.h"
#include "./Distrito.h"
#include "./Lista.h"
#include "./Criptografía/AlgoVigenere/Vigenere.h"
#include "./Criptografía/Kasiski.h"

using namespace std;

class Menu {

public:
	void mostrarAyuda();

	void pedirUsuario();

	void pedirContrasena();

	bool accesoAdmin();

	bool accesoUsuario();

	bool loggin();

	void adminInformes();

	void adminMain();

	void adminCandidato();

	void adminAdministrador();

	void adminDistrito();

	void adminCargo();

	void adminEleccion();

	void adminVotante();

	void adminLista();

	void adminVigenere();

	static Menu *getMenu();

private:
	string user;
	string pass;
	string salir;

	Menu();

	void listarEntidad(vector<string> entidades);
	void obtenerGrabables(vector<Grabable *> &grabables,string tipo,vector<unsigned int> &offsets,string nom_archivo);
	Grabable* instanciarGrabable(string tipo);
	virtual ~Menu();

};

#endif /* MENU_H_ */
