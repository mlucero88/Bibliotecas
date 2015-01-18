/*
 * Conteo.cpp
 *
 *  Created on: 24/09/2011
 *      Author: facundo
 */

#include "Conteo.h"
#include "ArbolBMas.h"

Conteo::Conteo(Lista lista, Distrito distrito) {
	this->_cantidad = 0;
	this->_distrito = new Distrito(distrito);
	this->_lista = new Lista(lista);
	this->_eleccion = new Eleccion(lista.getEleccion());
}

Conteo::Conteo(const Conteo &conteo){
	this->_distrito = new Distrito(*(conteo._distrito));
	this->_lista = new Lista(*(conteo._lista));
	this->_eleccion = new Eleccion(*(conteo._eleccion));
	this->_cantidad = conteo._cantidad;
}

Conteo::Conteo(){
    this->_distrito = NULL;
    this->_lista = NULL;
    this->_eleccion = NULL;
    this->_cantidad = 0;
}

Conteo::~Conteo() {
	if (this->_distrito != NULL) {
		delete this->_distrito;
		this->_distrito = NULL;
	}
	if (this->_lista != NULL) {
		delete this->_lista;
		this->_lista = NULL;
	}
	if (this->_eleccion != NULL) {
		delete this->_eleccion;
		this->_eleccion = NULL;
	}
}

void Conteo::incrementar(){	this->_cantidad++;}

long Conteo::getVotos(){ return _cantidad; }

Lista& Conteo::getLista(){ return *(this->_lista); }

Distrito& Conteo::getDistrito(){ return *(this->_distrito); }


unsigned long int Conteo::Guardar(ofstream & ofs){

	unsigned long int offset = ofs.tellp();

	// Comienzo escritura de atributos

	//Se escribe la referencia al Distrito guardando su id
	long idDistrito = this->_distrito->getId();
	ofs.write(reinterpret_cast<char *>(&idDistrito), sizeof(idDistrito));

	//Se escribe la referencia al Lista guardando su id
	long idLista = this->_lista->getId();
	ofs.write(reinterpret_cast<char *>(&idLista), sizeof(idLista));

	//Se escribe la referencia al Eleccion guardando su id
	long idEleccion = this->_eleccion->getId();
	ofs.write(reinterpret_cast<char *>(&idEleccion), sizeof(idEleccion));

	ofs.write(reinterpret_cast<char *>(&_cantidad), sizeof(_cantidad));

	return offset;
}

bool Conteo::Leer(ifstream & ifs, unsigned long int offset){

	// Elimino atributos de la instancia
	if (this->_distrito != NULL) {
		delete this->_distrito;
		this->_distrito = NULL;
	}
	if (this->_lista != NULL) {
		delete this->_lista;
		this->_lista = NULL;
	}
	if (this->_eleccion != NULL) {
		delete this->_eleccion;
		this->_eleccion = NULL;
	}

	// Me posiciono en el archivo
	//ifs.seekg(offset,ios::beg);
	ifs.seekg(offset);

	//Comienzo lectura de atributos

	// Leo el id del distrito
	long idDistrito = 0;
	ifs.read(reinterpret_cast<char *>(&idDistrito), sizeof(idDistrito));

	// Busco en el hash id_distrito/offset el offset de ese idDistrito
	string idDist = Utilidades::toString(idDistrito);
	string arch_registros_dist((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDDISTRITO_REGS));
	string arch_bloq_libres_dist((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDDISTRITO_BLOQ_LIB));
	string arch_tabla_dist((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDDISTRITO_TABLA));
	hash_extensible *hashIDDistritos = new hash_extensible(arch_registros_dist,arch_bloq_libres_dist,arch_tabla_dist);
	RegistroIndice DistritoBuscar(idDist,0);
	RegistroIndice *returnDistrito = hashIDDistritos->buscar(&DistritoBuscar);
	if (returnDistrito == NULL) {
		delete hashIDDistritos;
		hashIDDistritos = NULL;
		throw VotoElectronicoExcepcion("No se encuentra el id de distrito en el hash. Se recomienda eliminar este registro (Razon: el distrito fue dado de baja)");
		return false;
	}
	offset = returnDistrito->getOffset();

	// Leo el distrito del archivo de distritos
	DataAccess dataAccess;
	Distrito distrito;
	dataAccess.Leer(distrito,offset);
	_distrito = new Distrito(distrito);
	delete hashIDDistritos;
	hashIDDistritos = NULL;


	// Leo el id de la lista
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
		throw VotoElectronicoExcepcion("No se encuentra el id de lista en el hash. Se recomienda eliminar este registro (Razon: la lista fue dado de baja)");
		return false;
	}
	offset = returnLista->getOffset();
	delete hashIDListas;
	hashIDListas = NULL;

	// Leo la lista del archivo de listas
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
		throw VotoElectronicoExcepcion("No se pudo levantar correctamente el conteo. Se recomienda eliminar este registro");
		return false;
	}

	_lista = new Lista(lista);


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
		throw VotoElectronicoExcepcion("No se encuentra el id de eleccion en el hash. Se recomienda eliminar este registro (Razon: la eleccion fue dado de baja)");
		return false;
	}
	offset = returnEleccion->getOffset();
	delete hashIDElecciones;
	hashIDElecciones = NULL;

	// Leo la eleccion del archivo de elecciones
	Eleccion eleccion;
	try{
		dataAccess.Leer(eleccion,offset);
	}
	catch(string str){
		cout << endl << str << endl;
		excepcion = true;
	}
	if (excepcion) {
		throw VotoElectronicoExcepcion("No se pudo levantar correctamente el conteo. Se recomienda eliminar este registro");
		return false;
	}

	_eleccion = new Eleccion(eleccion);

	ifs.read(reinterpret_cast<char *>(&_cantidad), sizeof(_cantidad));

	return true;
}


int Conteo::getTamanioEnDisco(){
	int tamanio=0;

	tamanio+=sizeof(long)*3;
	tamanio+=sizeof(this->_cantidad);

	return tamanio;
}


inline string Conteo::getURLArchivoDatos(){
	return ((*Configuracion::getConfig()).getValorPorPrefijo(Configuracion::URL_CONTEO));
}

string Conteo::getClassName(){return "Conteo";}


void Conteo::Imprimir(){
	cout << "Conteo:" << endl;
    cout << "Lista: " << endl;
    this->_lista->Imprimir();
    cout << "Distrito: " << endl;
    this->_distrito->Imprimir();
    cout << "Cantidad de votos: " << this->_cantidad << endl;
}

void Conteo::Imprimir(ofstream &ofs)
{
	ofs << "Conteo:" << endl;
	ofs << "Eleccion: " << this->_eleccion->getFecha() << " - " << this->_eleccion->getCargo().getCargoPrincipal() << endl;
	ofs << "--Lista: " << this->_lista->getNombre()<<endl;
    ofs << "--Distrito: " << this->_distrito->getNombre()<<endl;
    ofs << "--Cantidad de votos: " << this->_cantidad << endl;
}
