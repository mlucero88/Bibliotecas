/*
 * Candidato.cpp
 *
 *  Created on: 19/09/2011
 *      Author: facundo
 */

#include "Candidato.h"

/* Constructor que no se deberia usar. No guardar algo contruido asi. Desp se saca si no es necesario.*/
Candidato::Candidato() {
	this->_id = -1;
	this->_listaPropia =  NULL;
	this->_votante = NULL;
}


Candidato::Candidato(Votante votante, Lista lista) {
	this->_id = 0;
	this->_listaPropia =  new Lista(lista);
	this->_votante = new Votante(votante);
}


Candidato::Candidato(const Candidato &candidato) {
	this->_id = candidato._id;
	this->_listaPropia = new Lista(*(candidato._listaPropia));
	this->_votante = new Votante(*(candidato._votante));
}


Candidato::~Candidato() {
	if (this->_listaPropia != NULL)	{
		delete this->_listaPropia;
		this->_listaPropia = NULL;
	}
	if (this->_votante != NULL)	{
		delete this->_votante;
		this->_votante = NULL;
	}
}


long Candidato::getId() {return _id;}


int Candidato::getDNI(){return this->_votante->getDNI();}


string Candidato::getNombreYApellido(){return this->_votante->getNombreYApellido();}


Cargo& Candidato::getCargo() {return (this->_listaPropia->getEleccion().getCargo());}


Lista& Candidato::getLista() {return *(this->_listaPropia);}

int Candidato::getTamanioEnDisco(){
	int tamanio=0;

	tamanio+=sizeof(this->_id);
	tamanio+=sizeof(this->_listaPropia->getId());
	tamanio+=sizeof(this->_votante->getId());

	return tamanio;
}


void Candidato::setLista(Lista lista) {
	if (this->_listaPropia != NULL) delete this->_listaPropia;
	this->_listaPropia = new Lista(lista);
}


void Candidato::setCargo(Cargo cargo) {
	this->_listaPropia->getEleccion().setCargo(cargo);
}


void Candidato::Imprimir()
{
	cout<<"Id Candidato: " <<_id <<endl;
	cout << "Informacion del candidato: " << endl;
	(*(this->_votante)).Imprimir();

	cout<<"Lista del candidato: "<<endl;
	(*(_listaPropia)).Imprimir();
	cout<<endl;
}


unsigned long int Candidato::Guardar(ofstream & ofs) {

	unsigned long int offset = ofs.tellp();

	//Comienzo escritura de atributos
	ofs.write(reinterpret_cast<char *>(&_id), sizeof(_id));

	long idLista = (*(_listaPropia)).getId();
	ofs.write(reinterpret_cast<char *>(&idLista), sizeof(idLista));

	//Se escribe la referencia al Votante guardando su id
	long idVotante = (*(_votante)).getId();
	ofs.write(reinterpret_cast<char *>(&idVotante), sizeof(idVotante));

	return offset;
}


bool Candidato::Leer(ifstream & ifs, unsigned long int offset)
{
	// Elimino atributos de la instancia
	if (this->_listaPropia != NULL)	{
		delete this->_listaPropia;
		this->_listaPropia = NULL;
	}
	if (this->_votante != NULL)	{
		delete this->_votante;
		this->_votante = NULL;
	}

	// Me posiciono en el archivo
	ifs.seekg(offset,ios::beg);

	//Comienzo lectura de atributos
	ifs.read(reinterpret_cast<char *>(&_id), sizeof(_id));

	// Leo el id de la lista asociada
	long idLista = 0;
	ifs.read(reinterpret_cast<char *>(&idLista), sizeof(idLista));

	// Busco en el hash id_lista/offset el offset de ese idLista
	string idLis = Utilidades::toString(idLista);
	string arch_registros_lis((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDLISTA_REGS));
	string arch_bloq_libres_lis((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDLISTA_BLOQ_LIB));
	string arch_tabla_lis((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDLISTA_TABLA));
	hash_extensible *hashIDListas = new hash_extensible(arch_registros_lis,arch_bloq_libres_lis,arch_tabla_lis);
	RegistroIndice ListaBuscar(idLis,0);
	RegistroIndice *returnLista = hashIDListas->buscar(&ListaBuscar);
	if (returnLista == NULL) {
		delete hashIDListas;
		hashIDListas = NULL;
		throw VotoElectronicoExcepcion("No se encuentra el id de lista en el hash. Se recomienda eliminar este registro (Razon: la lista fue dada de baja)");
		return false;
	}
	offset = returnLista->getOffset();
	delete hashIDListas;
	hashIDListas = NULL;

	// Leo la lista del archivo de listas
	DataAccess dataAccess;
	Lista lista;
	bool excepcion = false;
	try{
		dataAccess.Leer(lista,offset);
	}
	catch(string str){
		cout << endl << str << endl;
		excepcion = true;
	}
	if (excepcion) {
		throw VotoElectronicoExcepcion("No se pudo levantar correctamente el candidato. Se recomienda eliminar este registro");
		return false;
	}

	_listaPropia = new Lista(lista);


	//leo el id del votante
	long idVotante = 0;
	ifs.read(reinterpret_cast<char *>(&idVotante), sizeof(idVotante));

	// Busco en el hash id_lista/offset el offset de ese idLista
	string idVot = Utilidades::toString(idVotante);
	string arch_registros_vot((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDVOTANTE_REGS));
	string arch_bloq_libres_vot((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDVOTANTE_BLOQ_LIB));
	string arch_tabla_vot((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDVOTANTE_TABLA));
	hash_extensible *hashIDVotantes = new hash_extensible(arch_registros_vot,arch_bloq_libres_vot,arch_tabla_vot);
	RegistroIndice VotanteBuscar(idVot,0);
	RegistroIndice *returnVotante = hashIDVotantes->buscar(&VotanteBuscar);
	if (returnVotante == NULL) {
		delete hashIDVotantes;
		hashIDVotantes = NULL;
		throw VotoElectronicoExcepcion("No se encuentra el id de votante en el hash. Se recomienda eliminar este registro (Razon: el votante fue dada de baja)");
	}
	offset = returnVotante->getOffset();
	delete hashIDVotantes;
	hashIDVotantes = NULL;

	// Leo el votante del archivo de votantes
	Votante votante;
	try{
		dataAccess.Leer(votante,offset);
	}
	catch(string str){
		cout << endl << str << endl;
		excepcion = true;
	}
	if (excepcion) {
		throw VotoElectronicoExcepcion("No se pudo levantar correctamente el candidato. Se recomienda eliminar este registro");
		return false;
	}

	_votante = new Votante(votante);
	return true;
}


inline string Candidato::getURLArchivoDatos() {
	string url((*Configuracion::getConfig()).getValorPorPrefijo(Configuracion::URL_CANDIDATO));
	return url;
}


string Candidato::getClassName() {return "Candidato";}
