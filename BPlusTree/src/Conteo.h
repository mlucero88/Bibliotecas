/*
 * Conteo.h
 *
 *  Created on: 24/09/2011
 *      Author: facundo
 */


#ifndef CONTEO_H_
#define CONTEO_H_

#include <string>
#include "Grabable.h"
#include "Lista.h"
#include "Logueable.h"
#include "Distrito.h"
#include "Eleccion.h"
#include "Configuracion.h"
#include "hash_extensible.h"
#include "DataAccess.h"


using namespace std;

class Conteo : public Grabable {
private:
	Distrito *_distrito;
	Lista *_lista;
	Eleccion *_eleccion;
	long int _cantidad;

public:
	Conteo(Lista lista, Distrito distrito);
	Conteo(const Conteo &conteo);
    Conteo();
	virtual ~Conteo();
	void incrementar();
	long getVotos();
	Lista& getLista();
	Distrito& getDistrito();

	int getTamanioEnDisco();

	Eleccion& getEleccion(){ return *(this->_eleccion);}

	unsigned long int Guardar(ofstream & ofs);

	bool Leer(ifstream & ifs, unsigned long int offset);

	string getURLArchivoDatos();

	string getClassName();

    void Imprimir();

    void Imprimir(ofstream &ofs);
};

#endif /* CONTEO_H_ */
