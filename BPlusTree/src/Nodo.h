/*
 * Nodo.h
 *
 *  Created on: 02/10/2011
 *      Author: facundo
 */

#ifndef NODO_H_
#define NODO_H_

#include "RegistroArbol.h"
#include <string>
#include <vector>

using namespace std;

class Nodo {
    friend class ArbolBMas;
public:
    Nodo();
    virtual ~Nodo();
    unsigned int getCantidadRegistros();

    void Guardar(fstream &ofs);
    static Nodo * Leer(fstream &ifs);

    int agregarRegistro( const string & clave, const long & offset);
    int agregarRegistro(RegistroArbol & registro);
    int agregarRegistroAlFinal( RegistroArbol & elemento );

    void removerEnPosicion(int indice);
    void removerEnPosicion(int indice, RegistroArbol & removido );

    void limpiar();

    bool buscarRegistro( const string &clave, int &indice ) const;

    bool tieneHijos() const;
    bool estaVacio() const;
    bool estaLleno() const;

    int count() const;

    void setFlag(int flags);
    int getFlag();

    void setAddr(int addr);
    int getAddr();

    void Imprimir();

    enum Flags
    {
        NodoCambiado = 1
    };

private:
    //====================== Seccion de Atributos ==========================
    int _addr;
    int _menor;
    int _padre;
    char _flags;
    int _tamanio;
    vector<RegistroArbol *> _registros; //Settear el NodeSize de acuerdo a la cantidad de red

	//Calcular un tamanioMaximoNodo de acuerdo al valor de cantidadMaximaRegistros
    const static unsigned int _tamanioMaximoNodo = 500;
    const static unsigned int _cantidadMaximaRegistros = 3;
};


#endif /* NODO_H_ */
