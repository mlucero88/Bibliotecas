#ifndef ELECCION_H_
#define ELECCION_H_

#include <string>
#include <vector>
#include "Cargo.h"
#include "Distrito.h"
#include "RegistroIndice.h"
#include "hash_extensible.h"
#include "Configuracion.h"
#include "DataAccess.h"


using namespace std;

class Eleccion : public Grabable {
private:
	long _id;
	string _fecha;
	Cargo * _cargo;
	vector<Distrito *> _distritos;

public:
	Eleccion();

	Eleccion(string fecha, Cargo cargo, Distrito primerDistrito);

	Eleccion(const Eleccion &eleccion);
	virtual ~Eleccion();

	long getId();
	Cargo& getCargo();
	string getFecha();
	string getDescripcion();
	void agregarDistrito(Distrito distrito);
	vector<Distrito *> getDistritos();
	void setId(long id) {this->_id=id;}
	void setCargo(Cargo cargo);
	void vaciarVectorDistritos();

	//Interface de Guardable
	unsigned long int Guardar(ofstream & ofs);
	bool Leer(ifstream & ifs, unsigned long int offset);
	void Imprimir();
	void ImprimirDistritos();
	int getTamanioEnDisco();

	inline string getURLArchivoDatos();

	//Metodos interfaz Logueable
	string getClassName();
};

#endif /* ELECCION_H_ */
