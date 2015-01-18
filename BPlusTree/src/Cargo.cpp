#include "Cargo.h"


/* Constructor que no se deberia usar. No guardar algo contruido asi. Desp se saca si no es necesario.*/
Cargo::Cargo(){
	this->cargoPrincipal = "";
	this->_id = -1;
}


Cargo::Cargo(string cargoPrincipal){
	this->cargoPrincipal = cargoPrincipal;
	this->_id = 0;
}


Cargo::~Cargo() {
	this->vaciarVectorCargosSecundarios();
}


Cargo::Cargo(const Cargo &cargo){
	this->_id = cargo._id;
	this->cargoPrincipal = cargo.cargoPrincipal;
	int cantidad = cargo.cargosSecundarios.size();
	this->cargosSecundarios = vector<string>();
	string a_insertar;
	for(int i=0;i<cantidad;i++)	{
		a_insertar = cargo.cargosSecundarios[i];
		this->cargosSecundarios.push_back(a_insertar);
	}
}


bool Cargo::operator==(const Cargo &cargo) {
	if (this->_id != cargo._id) return false;
	if (this->cargoPrincipal != cargo.cargoPrincipal) return false;
	int cantidad = this->cargosSecundarios.size();
	if (cantidad != cargo.cargosSecundarios.size()) return false;
	for(int i=0;i<cantidad;i++) {
		if (this->cargosSecundarios[i] != cargo.cargosSecundarios[i]) return false;
	}
	return true;
}


string Cargo::getCargoPrincipal() {return this->cargoPrincipal;}


vector<string> Cargo::getCargosSecundarios() {return this->cargosSecundarios;}


long Cargo::getId() {return _id;}


void Cargo::agregarCargoSecundario(string cargo) {this->cargosSecundarios.push_back(cargo);}


void Cargo::vaciarVectorCargosSecundarios() {this->cargosSecundarios.clear();}


//Implementacion de interfaz Guardable
unsigned long int Cargo::Guardar(ofstream & ofs)
{
	unsigned long int offset = ofs.tellp();

	//Comienzo escritura de atributos
	ofs.write(reinterpret_cast<char *>(&_id), sizeof(_id));
	Utilidades::stringToFile(cargoPrincipal, ofs);

	//Grabo la cantidad de cargos secundarios q tiene
	string::size_type cantidadCargosSecundarios = cargosSecundarios.size();
	ofs.write(reinterpret_cast<char *>(&cantidadCargosSecundarios), sizeof(cantidadCargosSecundarios));

	//Comienzo a grabar los cargos secundarios
	for(string::size_type i = 0; i < cantidadCargosSecundarios; i++)
		Utilidades::stringToFile(cargosSecundarios[i], ofs);

	return offset;
}


bool Cargo::Leer(ifstream & ifs, unsigned long int offset)
{
	// Elimino atributos de la instancia
	this->vaciarVectorCargosSecundarios();

	// Me posiciono en el archivo
	ifs.seekg(offset,ios::beg);

	//Comienzo lectura de atributos
	ifs.read(reinterpret_cast<char *>(&_id), sizeof(_id));
	cargoPrincipal = Utilidades::stringFromFile(ifs);

	//Leo la cantidad de cargos secundarios q tiene
	string::size_type cantidadCargosSecundarios = 0;
	ifs.read(reinterpret_cast<char *>(&cantidadCargosSecundarios), sizeof(cantidadCargosSecundarios));

	// Elimino los cargos secundarios si tenia:
	if (this->cargosSecundarios.size() != 0)
		this->cargosSecundarios.erase(this->cargosSecundarios.begin(),this->cargosSecundarios.end());
	//Comienzo a leer los cargos secundarios
	for(string::size_type i = 0; i < cantidadCargosSecundarios; i++)
		cargosSecundarios.push_back(Utilidades::stringFromFile(ifs));

	return true;
}


void Cargo::Imprimir()
{
	cout<<"Id Cargo: " << _id <<endl;
	cout<<"Cargo Principal: " << cargoPrincipal <<endl;
	for(string::size_type i=0; i<cargosSecundarios.size(); i++)
		cout<< "Cargo Secundario " << i+1 << ": " << cargosSecundarios[i] <<endl;
	cout << endl;
}

int Cargo::getTamanioEnDisco(){

	int tamanio=0;

	tamanio+=sizeof(this->_id);

	int size = this->cargoPrincipal.size();

	tamanio+=sizeof(size);
	tamanio+=sizeof(char)*size;

	string::size_type cantidadCargosSecundarios = cargosSecundarios.size();
	tamanio+=sizeof(cantidadCargosSecundarios);

	int aux;
	for(int i=0;i<cantidadCargosSecundarios;i++){
		aux = this->cargosSecundarios[i].size();
		tamanio+=sizeof(aux);
		tamanio+=sizeof(char)*aux;
	}
	return tamanio;
}


string Cargo::getURLArchivoDatos() {return (*Configuracion::getConfig()).getValorPorPrefijo(Configuracion::URL_CARGO);}


string Cargo::getClassName() {return "Cargo";}
