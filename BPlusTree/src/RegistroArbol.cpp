/*
 * RegistroArbol.cpp
 *
 *  Created on: 02/10/2011
 *      Author: martin
 */

#include "RegistroArbol.h"

//==============================================================================
RegistroArbol::RegistroArbol() {
    this->_clave = "";
    this->_offset = 0;
    this->_link = 0;
}

//==============================================================================
RegistroArbol::RegistroArbol(std::string clave, long offset) {
    this->_clave = clave;
    this->_offset = offset;
    this->_link = 0;
}

//==============================================================================
RegistroArbol::RegistroArbol(RegistroArbol & registro)
{
    this->_clave = registro._clave;
    this->_offset = registro._offset;
    this->_link = registro._link;
}

//==============================================================================
RegistroArbol::~RegistroArbol() {
}

//==============================================================================
string RegistroArbol::getClave() {
    return this->_clave;
}

//==============================================================================
long RegistroArbol::getOffset() {
    return this->_offset;
}

//==============================================================================
void RegistroArbol::setClave(string clave)
{
    this->_clave = clave;
}

//==============================================================================
void RegistroArbol::setOffset(long offset)
{
    this->_offset = offset;
}

//==============================================================================
void RegistroArbol::setLink(long link)
{
    this->_link = link;
}

//==============================================================================
long RegistroArbol::getLink()
{
    return this->_link;
}

//==============================================================================
long RegistroArbol::getTamanioEnDisco() {
    long tamanioClave = _clave.size() * sizeof(char);
    long tamanioOffset = sizeof(_offset);
    long tamanioTotal = sizeof(tamanioTotal) + tamanioClave + tamanioOffset;

    return tamanioTotal; //en bytes
}

//==============================================================================
RegistroArbol * RegistroArbol::Leer(fstream & ifs) {
    long tamanioEnDisco;
    string claveAux;
    long offsetAux;
    long linkAux;
    //ifs.read(reinterpret_cast<char *>(&tamanioEnDisco), sizeof(tamanioEnDisco));
    claveAux = Utilidades::stringFromFile(ifs);
    ifs.read(reinterpret_cast<char *>(&offsetAux), sizeof(offsetAux));
    ifs.read(reinterpret_cast<char *>(&linkAux), sizeof(linkAux));

//    char delimitador;
//    ifs>>tamanioEnDisco>>delimitador;
//    claveAux = Utilidades::stringFromFile(ifs);
//    ifs>>offsetAux>>delimitador;
//    ifs>>linkAux>>delimitador>>delimitador;

    RegistroArbol * registro = new RegistroArbol(claveAux, offsetAux);
    registro->_link = linkAux;

    return registro;
}

//==============================================================================
void RegistroArbol::Guardar(fstream & ofs) {
    //long tamanioEnDisco = getTamanioEnDisco();
    //ofs.write(reinterpret_cast<char *>(&tamanioEnDisco), sizeof(tamanioEnDisco));
    Utilidades::stringToFile(_clave, ofs);
    ofs.write(reinterpret_cast<char *>(&_offset), sizeof(_offset));
    ofs.write(reinterpret_cast<char *>(&_link), sizeof(_link));

//    long tamanioEnDisco = getTamanioEnDisco();
//    ofs<<tamanioEnDisco<<"|";
//    ofs<<_clave<<"|";
//    ofs<<_offset<<"|";
//    ofs<<_link<<"||";
}

//==============================================================================
void RegistroArbol::Imprimir(){

    cout << "(" << getTamanioEnDisco() << "|" << _clave << "|" << _offset << "|" << _link<< ")";
}
