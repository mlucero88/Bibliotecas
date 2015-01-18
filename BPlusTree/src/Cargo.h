#ifndef CARGO_H_
#define CARGO_H_

#include <string>
#include <vector>

#include "Grabable.h"
#include "Logueable.h"
#include "Utilidades.h"
#include "Configuracion.h"

using namespace std;

class Cargo : public Grabable {
private:
	long _id;
	string cargoPrincipal;
	vector<string> cargosSecundarios;
public:
	Cargo();
	Cargo(string cargoPrincipal);
	virtual ~Cargo();
	Cargo(const Cargo &cargo);
	bool operator==(const Cargo &cargo);

	//Getters
	string getCargoPrincipal();
	vector<string> getCargosSecundarios();
	long getId();

	//Setters
	void setId(long id) {this->_id=id;}
	void agregarCargoSecundario(string cargo);
	void modificarCargoPrincipal(string nuevo_cargo){this->cargoPrincipal=nuevo_cargo;}

	//Metodos de la interfaz Guardable
	unsigned long int Guardar(ofstream & ofs);
	bool Leer(ifstream & ifs, unsigned long int offset);
	int getTamanioEnDisco();
	void Imprimir();

	void vaciarVectorCargosSecundarios();

	inline string getURLArchivoDatos();

	//Metodos interfaz Logueable
	string getClassName();
};

#endif /* CARGO_H_ */
