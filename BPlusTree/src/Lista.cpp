/*
 * Lista.cpp
 *
 *  Created on: 19/09/2011
 *      Author: facundo
 */

#include "Lista.h"

/* Constructor que no se deberia usar. No guardar algo contruido asi. Desp se saca si no es necesario.*/
Lista::Lista()
{
	this->_eleccion = NULL;
	this->_nombre = "";
	this->_id = -1;
}


Lista::Lista(string nombre, Eleccion eleccion)
{

	this->_eleccion = new Eleccion(eleccion);
	this->_nombre = nombre;

	this->_id = 0;
}

Lista::Lista(const Lista &lista) {
	this->_id = lista._id;
	this->_nombre = lista._nombre;
	this->_eleccion = new Eleccion(*(lista._eleccion));
}


Lista::~Lista() {
	if (this->_eleccion != NULL) {
		delete this->_eleccion;
		this->_eleccion =NULL;
	}
}


long Lista::getId() {return _id;}


Eleccion& Lista::getEleccion() {return *(this->_eleccion);}


string Lista::getNombre() {return this->_nombre;}


void Lista::setNombre(string &nombre)
{
	this->_nombre = nombre;
}

void Lista::setEleccion(Eleccion eleccion){
    if (this->_eleccion != NULL) delete this->_eleccion;
	this->_eleccion = new Eleccion(eleccion);
}

void Lista::Imprimir()
{
	cout<<"Id Lista: " <<_id << endl;
	cout<< "Nombre Lista: " << _nombre << endl;
	cout << "Eleccion a la que se presenta: "<<endl;
	(*(_eleccion)).Imprimir();
	cout<<endl;
}


unsigned long int Lista::Guardar(ofstream & ofs)
{
	unsigned long int offset = ofs.tellp();

	//Comienzo escritura de atributos
	ofs.write(reinterpret_cast<char *>(&_id), sizeof(_id));
	Utilidades::stringToFile(_nombre, ofs);

	//Se escribe la referencia a la Eleccion guardando su id
	long idEleccion = (*(_eleccion)).getId();
	ofs.write(reinterpret_cast<char *>(&idEleccion), sizeof(idEleccion));

	return offset;
}


bool Lista::Leer(ifstream & ifs, unsigned long int offset)
{
	// Elimino atributos de la instancia
	if (this->_eleccion != NULL) {
		delete this->_eleccion;
		this->_eleccion = NULL;
	}

	// Me posiciono en el archivo
	ifs.seekg(offset,ios::beg);

	//Comienzo lectura de atributos
	ifs.read(reinterpret_cast<char *>(&_id), sizeof(_id));
	_nombre = Utilidades::stringFromFile(ifs);

	// Leo el id de la eleccion
	long idEleccion = 0;
	ifs.read(reinterpret_cast<char *>(&idEleccion), sizeof(idEleccion));

	// Busco en el hash id_eleccion/offset el offset de ese idEleccion
	string idElec = Utilidades::toString(idEleccion);
	string arch_registros_elec((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDELECCION_REGS));
	string arch_bloq_libres_elec((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDELECCION_BLOQ_LIB));
	string arch_tabla_elec((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDELECCION_TABLA));
	hash_extensible *hashIDElecciones = new hash_extensible(arch_registros_elec,arch_bloq_libres_elec,arch_tabla_elec);
	RegistroIndice EleccionBuscar(idElec,0);
	RegistroIndice *returnEleccion = hashIDElecciones->buscar(&EleccionBuscar);
	if (returnEleccion == NULL) {
		delete hashIDElecciones;
		hashIDElecciones = NULL;
		throw VotoElectronicoExcepcion("No se encuentra el id de eleccion en el hash. Se recomienda eliminar este registro (Razon: la eleccion fue dada de baja)");
		return false;
	}
	offset = returnEleccion->getOffset();
	delete hashIDElecciones;
	hashIDElecciones = NULL;

	// Leo la eleccion del archivo de elecciones
	DataAccess dataAccess;
	Eleccion eleccion;
	bool excepcion = false;
	try {
		dataAccess.Leer(eleccion,offset);
	}
	catch(string str){
		cout << endl << str << endl;
		excepcion = true;
	}
	if (excepcion) {
		throw VotoElectronicoExcepcion("No se pudo levantar correctamente la lista. Se recomienda eliminar este registro");
		return false;
	}

	_eleccion = new Eleccion(eleccion);

	return true;
}


inline string Lista::getURLArchivoDatos() {
	return (*Configuracion::getConfig()).getValorPorPrefijo(Configuracion::URL_LISTA);
}


int Lista::getTamanioEnDisco(){
	int tamanio=0;

	tamanio+=sizeof(this->_id);

	int size_nombre = this->_nombre.size();
	tamanio+=sizeof(size_nombre);
	tamanio+=sizeof(char)*size_nombre;

	long idEleccion = (*(_eleccion)).getId();
	tamanio+=sizeof(idEleccion);

	return tamanio;
}


string Lista::getClassName() {return "Lista";}
