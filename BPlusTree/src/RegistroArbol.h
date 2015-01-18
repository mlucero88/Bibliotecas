/*
 * RegistroArbol.h
 *
 *  Created on: 02/10/2011
 *      Author: martin
 */

#ifndef REGISTROARBOL_H_
#define REGISTROARBOL_H_

#include <string>
#include <fstream>
#include "Utilidades.h"
#include "VotoElectronicoExcepcion.h"

using namespace std;

class RegistroArbol{
    friend class ArbolBMas;
    friend class Nodo;
public:
    RegistroArbol();
    RegistroArbol(string clave, long offset);
    RegistroArbol(RegistroArbol & registro);
    virtual ~RegistroArbol();

    string getClave();
    long getOffset();
    void setClave(string offset);
    void setOffset(long offset);
    long getTamanioEnDisco();
    static RegistroArbol * Leer(fstream & archivo);
    void Guardar(fstream & archivo);
    void Imprimir();
    void setLink(long link);
    long getLink();

private:
    //====================== Seccion de Atributos ==========================
    string _clave;
    long _offset;
    long _link; //Al nodo que contiene el registro inmediatemente mayor
};


#endif /* REGISTROARBOL_H_ */
