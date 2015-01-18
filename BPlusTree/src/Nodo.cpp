/*
 * Nodo.cpp
 *
 *  Created on: 02/10/2011
 *      Author: martin
 */

#include "Nodo.h"

//==============================================================================
Nodo::Nodo()
{
	this->_addr = 0;
	_menor = 0 ;
	_tamanio = 0;
	_padre = 0;
	_flags = 0;
}

//==============================================================================
Nodo::~Nodo() {
//    if(!_registros.empty())
//        limpiar();
}

//==============================================================================
int Nodo::agregarRegistro( const string &clave, const long &offset )
{
    bool fueAgregado = false;
    RegistroArbol * registro = new RegistroArbol(clave, offset);

    int i = 0;
    for ( i = 0; i < _tamanio; i++ )
    {
        if ( _registros[ i ]->getClave() > clave )
        {
            // Inserto en entre medio de uno menor y uno mayor q la clave
            //pasaron como parametro
            _registros.insert( _registros.begin( ) + i, registro);
            _registros[ i ]->_link = 0;
            fueAgregado = true;
            break;
        }
    }

    if ( !fueAgregado )
    {
        //Si no fue insertado entre medio lo meto al final
        _registros.push_back(registro);
        i = _tamanio;
    }

    _tamanio++;
    _flags = Nodo::NodoCambiado;

    return i;
}

//==============================================================================
int Nodo::agregarRegistro( RegistroArbol & elemento )
{
    int posicion = agregarRegistro( elemento.getClave(), elemento.getOffset() );
    _registros[ posicion ]->_link = elemento._link;
    return posicion;
}

//==============================================================================
int Nodo::agregarRegistroAlFinal( RegistroArbol & elemento )
{
    //RegistroArbol * registro = new RegistroArbol(elem.getClave(), elem.getOffset());

    _registros.push_back(&elemento);

    _tamanio++;
    _flags = Nodo::NodoCambiado;
    return (_tamanio - 1);
}

////	===================================================================
void Nodo::Guardar(fstream &ofs)
{
    //Situar el stream en la posicion absoluta addr_
    ofs.seekp(_addr);
    unsigned int capacidadEnBytesAux = _tamanioMaximoNodo;

    //ofs.write(reinterpret_cast<char *>(&capacidadEnBytesAux), sizeof(capacidadEnBytesAux));
    //ofs.write(reinterpret_cast<char *>(&tamanioEnBytesOcupados), sizeof(tamanioEnBytesOcupados));

    ofs.write(reinterpret_cast<char *>(&(_tamanio)), sizeof((_tamanio)));
    ofs.write(reinterpret_cast<char *>(&(_addr)), sizeof((_addr)));
    ofs.write(reinterpret_cast<char *>(&(_menor)), sizeof((_menor)));
    ofs.write(reinterpret_cast<char *>(&(_padre)), sizeof((_padre)));


//    ofs<<capacidadEnBytesAux<<"|";
//    ofs<<_tamanio<<"|";
//    ofs<<_addr<<"|";
//    ofs<<_menor<<"|";
//    ofs<<_padre<<"|";

    for(unsigned int i = 0; i < _registros.size(); i++)
    {
        _registros[i]->Guardar(ofs);
    }

    //ofs<<"||";
    long posicionStream = ofs.tellp();
    long cantidadALimpiar = _tamanioMaximoNodo - (posicionStream - _addr);
    char byteLimpio = 0;

    for(long i = 0; i<cantidadALimpiar; i++)
    {
        //ofs<<" ";
    	ofs.write(reinterpret_cast<char *>(&(byteLimpio)), sizeof((byteLimpio)));
    }
}

//==============================================================================
Nodo * Nodo::Leer(fstream &ifs)
{
    unsigned int capacidadEnBytesAux = 0;
    Nodo * nodo = new Nodo();

    //ifs.read(reinterpret_cast<char *>(&(capacidadEnBytesAux)), sizeof((capacidadEnBytesAux)));
    //ifs.read(reinterpret_cast<char *>(&(nodo->tamanioEnBytesOcupados)), sizeof((nodo->tamanioEnBytesOcupados)));

    ifs.read(reinterpret_cast<char *>(&(nodo->_tamanio)), sizeof((nodo->_tamanio)));
    ifs.read(reinterpret_cast<char *>(&(nodo->_addr)), sizeof((nodo->_addr)));
    ifs.read(reinterpret_cast<char *>(&(nodo->_menor)), sizeof((nodo->_menor)));
    ifs.read(reinterpret_cast<char *>(&(nodo->_padre)), sizeof((nodo->_padre)));

//    char delimitador;
//
//    ifs>>(capacidadEnBytesAux)>>delimitador;
//    ifs>>(nodo->_tamanio)>>delimitador;
//    ifs>>(nodo->_addr)>>delimitador;
//    ifs>>(nodo->_menor)>>delimitador;
//    ifs>>(nodo->_padre)>>delimitador;


    for(int i = 0; i < nodo->_tamanio; i++)
    {
        nodo->_registros.push_back(RegistroArbol::Leer(ifs));
    }

    //ifs>>delimitador>>delimitador;

    return nodo;
}

//==============================================================================
void Nodo::removerEnPosicion(int indice){
    _registros.erase(_registros.begin() + indice);
    _tamanio--;
    _flags = NodoCambiado;
}

//==============================================================================
void Nodo::removerEnPosicion(int indice, RegistroArbol & removido ){
    //Antes de remover devuelve por parametro una referencia al elemento
    //q quiero borrar en esa posicion
    removido = *_registros[indice];
    removerEnPosicion(indice);
}

//==============================================================================
void Nodo::limpiar()
{
    _tamanio = 0;
    _flags = 0;

    RegistroArbol * registro;

    while(!_registros.empty())
    {
        registro = _registros.back();
        _registros.pop_back();
        delete registro;
    }
}

//==============================================================================
bool Nodo::buscarRegistro( const string &clave, int &indice ) const
{
    int lb = 0, ub = _tamanio - 1;

    while( lb <= ub )
    {
        indice = ( lb + ub ) >> 1;

        if( clave < _registros[ indice ]->getClave() )
        {
            ub = indice - 1;
        }
        else
        {
            if( clave > _registros[ indice ]->getClave() )
            {
                lb = indice + 1;
            }
            else
            {
                return true;
            }
        }
    }

    indice = lb;
    return false;
}

//==============================================================================
bool Nodo::tieneHijos() const
{
    return _menor != 0;
}

//==============================================================================
bool Nodo::estaVacio() const
{
    return _tamanio == 0;
}

//==============================================================================
bool Nodo::estaLleno() const
{
    return _tamanio >= _cantidadMaximaRegistros;
}

//==============================================================================
int Nodo::count() const
{
    return _tamanio;
}

//==============================================================================
void Nodo::setFlag(int flags)
{
    _flags = flags;
}

//==============================================================================
int Nodo::getFlag()
{
    return _flags;
}

//==============================================================================
void Nodo::setAddr(int addr)
{
    _addr = addr;
}

//==============================================================================
int Nodo::getAddr()
{
    return _addr;
}

//==============================================================================
void Nodo::Imprimir()
{
    cout << "Capacidad en bytes: " << _tamanioMaximoNodo << endl;
    cout << "Cantidad de Registros: " << _tamanio << endl;
    cout << "Address: " << _addr << endl;

    cout << "Registros del Nodo: " << endl;
    for(unsigned int i = 0; i < _registros.size(); i++)
    {
        cout << "Registro Nro" << i+1 << endl;
        _registros[i]->Imprimir();
        cout<< endl;
    }
}
