/*
 * Utilidades.cpp
 *
 *  Created on: 21/09/2011
 *      Author: facundo
 */

#include "Utilidades.h"

Utilidades::Utilidades() {

}

Utilidades::~Utilidades() {

}

string Utilidades::getFechaYHora()
{
	// current date/time based on current system
	   time_t now = time(0);
	   tm *ltm = localtime(&now);

	   string year = toString(1900 + ltm->tm_year);
	   string month = toString(1 + ltm->tm_mon);
	   string day = toString(ltm->tm_mday);
	   string time = toString(1 + ltm->tm_hour) + ":" + toString(1 + ltm->tm_min) + ":" + toString(1 + ltm->tm_sec);

	   return time + " - " + day + "-" + month + "-" + year;
}

string Utilidades::indexarFecha(string fecha) {
	string fechaIndexada;
	if (fecha.length() == 10) {
		fechaIndexada.append(fecha,6,4).append(fecha,3,2).append(fecha,0,2);
		return fechaIndexada;
	}
	if (fecha.length() == 8) {
		fechaIndexada.append(fecha,4,4).append(fecha,2,2).append(fecha,0,2);
		return fechaIndexada;
	}
	return "aaaammdd";
}


/* Reemplaza los espacios de una clave por & */
void Utilidades::formatearClave(string &clave) {
	size_t posicion;
	do {
		posicion = clave.find(" ");
		if (posicion != -1) clave[posicion] = '&';
	} while (posicion != -1);
}

string Utilidades::toString(long int a)
{
	stringstream ss;
	ss << a;
	return ss.str();
}

/* Retorna un DNI entre 1.000.000 y 39.999.999*/
unsigned int Utilidades::getDNIaleatorio(int semilla) {
	srand(semilla);
	unsigned int unidad = rand() % 10;
	unsigned int decena = (rand() % 10) * 10;
	unsigned int centena = (rand() % 10) * 100;
	unsigned int u_de_mil = (rand() % 10) * 1000;
	unsigned int d_de_mil = (rand() % 10) * 10000;
	unsigned int c_de_mil = (rand() % 10) * 100000;
	unsigned int d_de_millon = (rand() % 4) * 10000000;
	unsigned int u_de_millon;
	if (d_de_millon == 0) u_de_millon = 1000000;
		else u_de_millon =  (rand() % 10) * 1000000;
	return (unidad+decena+centena+u_de_mil+d_de_mil+c_de_mil+u_de_millon+d_de_millon);
}


void Utilidades::stringToFile(std::string & str, std::ofstream & ofs)
{
	int size = str.size();

	ofs.write(reinterpret_cast<char *>(&size), sizeof(size));
	ofs.write(str.c_str(), sizeof(char) * size);
}

void Utilidades::stringToFile(std::string & str, std::fstream & ofs)
{
	int size = str.size();

	ofs.write(reinterpret_cast<char *>(&size), sizeof(size));
	ofs.write(str.c_str(), sizeof(char) * size);
}

string Utilidades::stringFromFile(std::ifstream & ifs)
{
    int size = 0;
    char buffer[500]; //MAX_STRING_BUFFER

    ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
    ifs.read(buffer, sizeof(char) * size);

    buffer[size] = '\0';
    string nombre(buffer);	//Por lo que probe el constructor copia le pone el "\0";

    return nombre;
}


string Utilidades::stringFromFile(std::fstream & ifs)
{
    int size = 0;
    char buffer[500]; //MAX_STRING_BUFFER

    ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
    ifs.read(buffer, sizeof(char) * size);

    buffer[size] = '\0';
    string nombre(buffer);	//Por lo que probe el constructor copia le pone el "\0";

    return nombre;
}


void Utilidades::sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}


string Utilidades::obtenerClaveDistrito(const string &nombreDistrito) {
	string clave = nombreDistrito;
	Utilidades::formatearClave(clave);
	return clave;
}


string Utilidades::obtenerClaveCargo(const string &cargoPrincipal) {
	string clave = cargoPrincipal;
	Utilidades::formatearClave(clave);
	return clave;
}


string Utilidades::obtenerClaveEleccion(const string &fecha, const string &cargoPrincipal) {
	string clave = Utilidades::indexarFecha(fecha) + "$" + cargoPrincipal;
	Utilidades::formatearClave(clave);
	return clave;
}
string Utilidades::obtenerClaveLista(const string &fecha, const string &cargoPrincipal, const string &nombreLista) {
	string clave = Utilidades::indexarFecha(fecha) + "$" + cargoPrincipal + "$" + nombreLista;
	Utilidades::formatearClave(clave);
	return clave;
}


string Utilidades::obtenerClaveVotante(const int &DNI) {
	string clave = Utilidades::toString(DNI);
	Utilidades::formatearClave(clave);
	return clave;
}


string Utilidades::obtenerClaveCandidato(const string &fecha, const string &cargoPrincipal, const string &nombreLista, const int &DNI) {
	string clave = Utilidades::indexarFecha(fecha) + "$" + cargoPrincipal + "$" + nombreLista + "$" + Utilidades::toString(DNI);
	Utilidades::formatearClave(clave);
	return clave;
}


string Utilidades::toString(int a)
{
	stringstream ss;
	ss << a;
	return ss.str();
}

string Utilidades::toString(string::size_type a)
{
	stringstream ss;
	ss << a;
	return ss.str();
}

long Utilidades::toLong(string a)
{
	stringstream ss(a);
	long n;
	ss >> n;
	return n;
}

int Utilidades::toInt(string a)
{
	stringstream ss(a);
	int n;
	ss >> n;
	return n;
}

string Utilidades::obtenerClaveAdministrador(const string &usuario) {
	string clave = usuario;
	Utilidades::formatearClave(clave);
	return clave;
}
