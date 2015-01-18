/*
 * ArbolBMas.h
 *
 *  Created on: 08/10/2011
 *      Author: facundo
 */

#ifndef ArbolBMas_H_
#define ArbolBMas_H_

#include "RegistroArbol.h"
#include "Nodo.h"
#include <string>
#include <list>

using namespace std;

/// Arbol B+ con administracion de chache.
/**
 * Este arbol B+ está preparado para contener objetos de la clase Nodo, este, a
 * su vez contiene objetos de la clase RegistroArbol.
 * Los datos son almacenados en disco. Para funcionar requiere de los siguientes
 * archivos:
 *
 * - Nodos: es donde se guardan los nodos con los registros.
 *
 * Ejemplo de uso:
 *  1.- ArbolBMas * arbol = new ArbolBMas();
 *  2.- arbol->abrir(url);
 *  3.- arbol->agregar(clave, offset);
 *  4.- arbol->buscar(clave, offsetEcontrado);
 *  5.- arbol->cerrar();
 *  6.- delete arbol;
*/

class ArbolBMas {
public:
    ArbolBMas();
    virtual ~ArbolBMas();

    bool abrir(string url);

    /// Pre: el arbol fue creado. Post: devuelve true si el arbol esta abierto
    bool estaAbierto();

    /// Pre: el arbol fue creado. Post: se agrega un registro con clave y offset pasado por parametro
    /**Si no se puede agregar o ya existia un registro con esa clave, devuelve false */
    bool agregar( const string &clave, const long &offset );

    /// Pre: el arbol fue creado. Post: se elimina un registro por clave
    /**Si no se puede eliminar o no existia el registro, devuelve false */
    bool eliminar( const string &clave );

    /// Pre: el arbol fue creado. Post: se elimina un registro por clave y devuelve por parametro su offset
    /**Si no se puede eliminar o no existia el registro, devuelve false */
    bool eliminar( const string &clave, long &offset );

    /// Pre: el arbol fue creado. Post: se busca un registro por clave y devuelve por parametro su offset
    /**Si no encuentra el registro, devuelve false */
    bool buscar( const string &clave, long &offset );

    /// Pre: el arbol fue creado. Post: se busca un registro por clave y devuelve por parametro una lista de registros cuyas claves esten entre claveInicial y condicion
    /**Si esBusquedaPrecisa es true y claveInicial no existe en el arbol, devuelve una lista vacia, sino devuelve la lista y true.
     * Si esBusquedaPresisa es false devuelve una lista con los inmediatamente mayores a claveIncial hasta condicion*/
    bool buscar( list<RegistroArbol *> &retList, const string &claveInicial, bool esBusquedaPrecisa, const string &condicion );

    /// Pre: el arbol fue creado. Post: se busca un registro por clave y devuelve por parametro una lista de registros cuyas claves sean igual o mayor a claveInicial
    /**Si esBusquedaPrecisa es true y claveInicial no existe en el arbol, devuelve una lista vacia, sino devuelve la lista y true.
     * Si esBusquedaPresisa es false devuelve una lista con los inmediatamente mayores a claveIncial*/
    bool buscar( list<RegistroArbol *> &retList, const string &claveInicial, bool esBusquedaPrecisa);

    /// Pre: el arbol fue creado. Post: se busca un registro por clave y devuelve por parametro una lista de registros cuyas claves esten entre claveInicial y condicion
    /**Si no habia registros que devolver, devuelve false*/
    bool buscar( list<RegistroArbol *> &retList, const string &claveInicial, const string &condicion );

    /// Pre: el arbol fue creado. Post: devuelve todos los registros del arbol de manera ordenada
    /**Si no habia registros que devolver, devuelve false*/
    bool getTodosLosRegistros(list<RegistroArbol *> &retList );

    /// Pre: el arbol fue creado. Post: cambia el offset del registro que tiene la clave pasada como parametro*/
    /**Se verifica que el registro exista, si el cambio se realiza con exito se retorna true*/
    bool cambiarOffset( const string &clave, const long &nuevoOffset );

    /// Pre: el arbol fue creado. Post: cambia la clave del registro que tiene la clave pasada como parametro*/
    /**Se verifica que el registro exista y se reoragniza el arbol si es necesario, si el cambio se realiza con exito se retorna true*/
    bool cambiarClave( const string &clave, const string &nuevaClave );

    /// Pre: el arbol fue creado. Post: cierra el arbol y libera todos los recursos*/
    void cerrar();

private:
    //====================== Seccion de Atributos ==========================
    Nodo * _raiz;
    fstream _storage;
    long _tamanioMaximoCache; //son cantidad de nodos en RAM
    long _addrRaiz;
    //vector<Nodo *> freeChunks;
    vector<Nodo *> _cache;
    bool _estaAbierto;

    const static long _finCamposControl = 512;

    //======================= Metodos Internos =============================

    int getRegistroDelMedio( Nodo *nodo, RegistroArbol &elem );

    bool partirNodo( Nodo *nodo, RegistroArbol &medio, Nodo **nodoDerecho );

    Nodo* encontrarNodo( Nodo *nodo, const string &clave, int &retIndice, int &indicePadre, bool &econtrado );

    bool hayEspacioLibre();

    bool rebalancear( Nodo *nodo, int indicePadre );
    bool combinarNodos( Nodo *nodoIzquierdo, Nodo *nodoDerecho );
    bool retirar( Nodo *nodo, int indiceItem );

    Nodo* getNodoMasDerecho( Nodo *subarbol, string &mayorClave, long &mayorOffset );
    Nodo* getNodoMasIzquierdo( Nodo *subarbol, string &menorClave, long &menorOffset );

    // Metodos de busqueda internos, buscan registros a partir de un nodo el buscado + todos sus mayores
    bool buscar( list<RegistroArbol *> &retList, const string &claveInicial, bool esBusquedaPrecisa, bool habilitarCondicion, const string &claveHasta );
    bool registrosDesdeNodo( Nodo *nodo, list<RegistroArbol *> &retList, int indiceElem, const string &claveInicial, bool habilitarCondicion, const string &claveHasta );
    bool registrosDesdeNodo( Nodo *nodo, list<RegistroArbol *> &retList, bool habilitarCondicion, const string &claveHasta );
    bool registrosDesdeNodo( Nodo *nodo, list<RegistroArbol *> &retList );


    ///
    /// Limpia completamente el arbol
    //bool clear();

    // Operaciones de Storage
    //bool allocFreeSpace();
    //bool readAllOffsets( Nodo *nodo );

    ///
    /// Carga el nodo especificado
    bool cargarNodo( Nodo **nodo, int addr );
    Nodo* getNodoNuevo();
    void borrarNodo( Nodo *nodo );
    ///
    /// Libera un solo nodo
    void liberarNodo( int addr );
    void guardarNodo( Nodo *nodo );
    long getAddrRaiz();
    void setAddrRaiz( long addr );
    Nodo * getRaiz();

    int tamanioStorage();

    // Metodos para el manejo del cache
    void limpiarCache();
    void liberarCache();
    void ImprimirCache();
    Nodo * buscarEnCache(int addr);
    Nodo * buscarEnCache(int addr, int &pos);
    bool nodoEstaEnCache( int addr );
    void borrarDeCache(int indice);

    void impactarCambios();
    void cerrarControlador();
};

#endif /* ArbolBMas_H_ */
