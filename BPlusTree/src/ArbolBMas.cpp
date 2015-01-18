/*
 * ArbolBMas.cpp
 *
 *  Created on: 08/10/2011
 *      Author: facundo
 */

#include "ArbolBMas.h"

//==============================================================================
ArbolBMas::ArbolBMas() {
    _raiz = 0;
    _estaAbierto = false;
    _tamanioMaximoCache = 512;
    _addrRaiz = 0;
}

//==============================================================================
ArbolBMas::~ArbolBMas() {
    if(_estaAbierto)
        cerrar();
}

//==============================================================================
bool ArbolBMas::agregar( const string &clave, const long &offset )
{
    if ( !estaAbierto() ) return false;

    if ( !_raiz )
    {
        int addr = getAddrRaiz();
        if ( !addr )
        {
            _raiz = getNodoNuevo();
            _raiz->_flags = Nodo::NodoCambiado;
            setAddrRaiz( _raiz->_addr );
        }
        else
        {
            if ( !cargarNodo( &_raiz, addr ) )
            {
                cout<<"ERROR: No estaba cargada ni se pudo cargar la raiz al insertar"<<endl;
                return false;
            }
        }
    }

    if ( !_raiz ) return false;

    // Busco el nodo destino
    bool econtrado = false;
    string copiaClave(clave);
    long copiaOffset(offset);
    int indiceRetornado = -1, indicePadre = 0;

    Nodo *nodo = encontrarNodo( _raiz, copiaClave, indiceRetornado, indicePadre, econtrado );

    if ( econtrado || !nodo )
    {
        // No se permiten claves duplicadas
        liberarCache();
        return false;
    }

    if ( !nodo->estaLleno() )
    {
        // Lo agrego al nodo, ya q no esta lleno
        nodo->agregarRegistro( copiaClave, copiaOffset );
        liberarCache();
        return true;
    }
    else
    {
        //Si el nodo esta lleno tengo q partirlo en dos, mitades
        //Tener contador de cantidad de registros en el nodo
        RegistroArbol medio(clave,offset);
        Nodo *nodoDerecho = 0;
        Nodo *padre = 0;
        if ( !cargarNodo( &padre, nodo->_padre ) )
        {
            liberarCache();
            return false;
        }

        // Parto el nodo y obtengo el intermedio (medio)
        if ( !partirNodo( nodo, medio, &nodoDerecho ) )
        {
            liberarCache();
            return false;
        }

        while ( 0 != padre )
        {
            // Agrego el intermedio al padre
            //Lo repito hasta que el padre no necesite ser particionado
            if ( padre->estaLleno() )
            {
                if ( !partirNodo( padre, medio, &nodoDerecho ) )
                {
                    liberarCache();
                    return false;
                }

                nodo = padre;

                // Me mueve hacia la raiz
                if ( !cargarNodo( &padre, padre->_padre ) )
                {
                    liberarCache();
                    return false;
                }
            }
            else
            {
                padre->agregarRegistro( medio );
                padre->_flags = Nodo::NodoCambiado;
                nodoDerecho->_padre = padre->_addr;
                nodoDerecho->_flags = Nodo::NodoCambiado;
                break;
            }
        }

        //Si la raiz se partio, entonces creo una nueva raiz q tenga un elemento y dos punteros
        if ( !padre )
        {
            // El nodo raiz!
            _raiz = getNodoNuevo();
            setAddrRaiz( _raiz->_addr );
            nodo->_padre = _raiz->_addr;
            nodo->_flags = Nodo::NodoCambiado;
            nodoDerecho->_padre = _raiz->_addr;
            nodoDerecho->_flags = Nodo::NodoCambiado;

            _raiz->agregarRegistro( medio._clave, medio._offset );
            _raiz->_registros[ 0 ]->_link = nodoDerecho->_addr;
            _raiz->_menor = nodo->_addr;
        }
    }

    liberarCache();
    return true;
}

//==============================================================================
void ArbolBMas::ImprimirCache()
{
    cout<<"/////////////Comienza imprecion de cache//////////////"<<endl;

    for(int i=0; i<_cache.size(); i++)
    {
        _cache[i]->Imprimir();
        cout<<endl;
    }

    cout<<"/////////////Fin imprecion de cache//////////////"<<endl;
}

//==============================================================================
bool ArbolBMas::eliminar( const string &clave )
{
    long basura;
    return eliminar(clave, basura);
}

//==============================================================================
bool ArbolBMas::eliminar( const string &clave, long &offsetRetornado )
{
    if ( !_raiz )
    {
        return false;
    }

    // Buscar nodo objetivo
    bool econtrado = false;
    int indice = -1, indicePadre = 0;

    Nodo *nodo = encontrarNodo( _raiz, clave, indice, indicePadre, econtrado );

    if ( !econtrado || !nodo )
    {
        // Clave no encontrada
        liberarCache();
        return false;
    }
    else
    {
        offsetRetornado = nodo->_registros[ indice ]->_offset;
    }

    if ( !nodo->tieneHijos() )
    {
        nodo->removerEnPosicion( indice );
        if ( !rebalancear( nodo, indicePadre ) )
        {
            liberarCache();
            return false;
        }
    }
    else
    {
        if ( !retirar( nodo, indice ) )
        {
            liberarCache();
            return false;
        }
    }

    liberarCache();
    return true;
}

//==============================================================================
bool ArbolBMas::buscar( const string &clave, long &offset )
{
    if ( !_raiz )
    {
        return false;
    }

    int indice = 0, indicePadre = 0;
    bool econtrado = false;
    Nodo *nodo = encontrarNodo( _raiz, clave, indice, indicePadre, econtrado );

    if ( !nodo || indice < 0 )
    {
        liberarCache();
        return false;
    }

    if ( econtrado )
    {
        // Clave econtrada
        offset = nodo->_registros[ indice ]->getOffset();
        liberarCache();
        return true;
    }

    liberarCache();

    // Clave no econtrada
    return false;
}

//==============================================================================
bool ArbolBMas::buscar( list<RegistroArbol *> &listaRetornada, const string &claveInicial, bool esBusquedaPrecisa, const string &condicion )
{
    return buscar(listaRetornada, claveInicial, esBusquedaPrecisa, true, condicion );
}

//==============================================================================
bool ArbolBMas::buscar( list<RegistroArbol *> &listaRetornada, const string &claveInicial, bool esBusquedaPrecisa)
{
    return buscar(listaRetornada, claveInicial, esBusquedaPrecisa, false, "" );
}

//==============================================================================
bool ArbolBMas::buscar( list<RegistroArbol *> &listaRetornada, const string &claveInicial, const string &condicion )
{
    return buscar(listaRetornada, claveInicial, false, true, condicion );
}

//==============================================================================
bool ArbolBMas::buscar( list<RegistroArbol *> &listaRetornada, const string &claveInicial, bool esBusquedaPrecisa, bool habilitarCondicion, const string &condicion ) //TChecker &condition )
{
    listaRetornada.clear();

    // Econtrar el nodo objetivo
    bool econtrado = false;
    int indice = -1, inidicePadre = 0;
    Nodo *nodo = encontrarNodo( _raiz, claveInicial, indice, inidicePadre, econtrado );

    if ( ( esBusquedaPrecisa && !econtrado ) || !nodo )
    {
        liberarCache();
        return false;
    }

    if ( !registrosDesdeNodo( nodo, listaRetornada, indice, claveInicial, habilitarCondicion, condicion ) )
    {
        liberarCache();
        return false;
    }

    liberarCache();
    return true;
}

//==============================================================================
bool ArbolBMas::getTodosLosRegistros( list<RegistroArbol *> &listaRetornada )
{
    if ( !registrosDesdeNodo( _raiz, listaRetornada ) )
    {
        liberarCache();
        return false;
    }

    liberarCache();
    return true;
}

//==============================================================================
bool ArbolBMas::cambiarOffset( const string &clave, const long &nuevoOffset )
{
    if ( !_raiz )
    {
        return false;
    }

    int indice = 0, indicePadre = 0;
    bool econtrado = false;
    Nodo *nodo = encontrarNodo( _raiz, clave, indice, indicePadre, econtrado );

    if ( !nodo || indice < 0 )
    {
        liberarCache();
        return false;
    }

    if ( econtrado )
    {
        // Clave econtrada
        nodo->_registros[ indice ]->_offset = nuevoOffset;
        nodo->_flags = Nodo::NodoCambiado;
        liberarCache();
        return true;
    }

    liberarCache();
    return false;
}

//==============================================================================
bool ArbolBMas::cambiarClave( const string &clave, const string &nuevaClave )
{
    long offset = -1;

    if(buscar(clave, offset))
        if(eliminar(clave))
            if(agregar(nuevaClave, offset))
                return true;
        
    return false;
}

//==============================================================================
bool ArbolBMas::registrosDesdeNodo(Nodo *nodo, list<RegistroArbol *> &listaRetornada, int indiceElemento, const string &claveInicial, bool habilitarCondicion, const string &condicion )
{
    if ( !nodo ) return true;

    if ( -1 == indiceElemento )
    {
        nodo->buscarRegistro( claveInicial, indiceElemento );
        if ( -1 == indiceElemento )
        {
                return false;
        }
    }

    int i = indiceElemento;

    while ( i < nodo->count() )
    {
        if ( !((nodo->_registros[ i ]->_clave >= condicion) && habilitarCondicion) )
        {
            listaRetornada.push_back( new RegistroArbol( *(nodo->_registros[ i ] )) );

            if ( nodo->_registros[ i ]->_link )
            {
                // Examino cada item hijo
                Nodo *hijo = 0;
                bool estabaEnCache = nodoEstaEnCache( nodo->_registros[ i ]->_link );
                if ( !cargarNodo( &hijo, nodo->_registros[ i ]->_link ) ) return false;
                registrosDesdeNodo( hijo, listaRetornada, habilitarCondicion, condicion );
                if ( hijo && !estabaEnCache ) liberarNodo( hijo->_addr );
            }
        }
        else
        {
            break;
        }

        // Me muevo hacia el final del nodo
        i++;
    }

    if ( !nodo->_padre )
    {
        // Se ha llegado al final de la busqueda
        return true;
    }

    Nodo *padre = 0;
    bool estabaEnCache = nodoEstaEnCache( nodo->_padre );
    if ( !cargarNodo( &padre, nodo->_padre ) )
    {
        return false;
    }

    bool ret = registrosDesdeNodo( padre, listaRetornada, -1, claveInicial, habilitarCondicion, condicion );
    if ( padre && !estabaEnCache ) liberarNodo( padre->_addr );
    return ret;
}


//==============================================================================
bool ArbolBMas::registrosDesdeNodo(Nodo *nodo, list<RegistroArbol *> &listaRetornada, bool habilitarCondicion, const string &condicion )
{
    if ( !nodo ) return true;

    if ( nodo->_menor )
    {
        Nodo *menor = 0;

        bool estabaEnCache = nodoEstaEnCache( nodo->_menor );
        if ( !cargarNodo( &menor, nodo->_menor ) ) return false;

        if ( !registrosDesdeNodo( menor, listaRetornada ) )
        {
            // Fin
            return false;
        }

        if ( menor && !estabaEnCache ) liberarNodo( menor->_addr );
    }

    for ( int i = 0; i < nodo->count(); i++ )
    {
        //Aplico la condicion, si estaba habilitada
        if ( ((nodo->_registros[ i ]->_clave >= condicion) && habilitarCondicion) )
        {
            // Paro condicion alcanzada
            return false;
        }

        listaRetornada.push_back( new RegistroArbol( *(nodo->_registros[ i ]) ) );

        if ( nodo->_registros[ i ]->_link )
        {
            Nodo *hijo = 0;
            bool estabaEnCache = nodoEstaEnCache( nodo->_registros[ i ]->_link );
            if ( !cargarNodo( &hijo, nodo->_registros[ i ]->_link ) ) return false;
            if ( !registrosDesdeNodo( hijo, listaRetornada ) )
            {
                // Final del arbol o condicion alcanzada
                return false;
            }
            if ( hijo && !estabaEnCache ) liberarNodo( hijo->_addr );
        }
    }

    return true;
}

//==============================================================================
bool ArbolBMas::registrosDesdeNodo(Nodo *nodo, list<RegistroArbol *> &listaRetornada )
{
    if ( !nodo ) return true;

    if ( nodo->_menor )
    {
        Nodo *menor = 0;

        bool estabaEnCache = nodoEstaEnCache( nodo->_menor );
        if ( !cargarNodo( &menor, nodo->_menor ) ) return false;
        if ( !registrosDesdeNodo( menor, listaRetornada ) ) return false;
        if ( menor && !estabaEnCache ) liberarNodo( menor->_addr );
    }

    for ( int i = 0; i < nodo->count(); i++ )
    {
        listaRetornada.push_back( new RegistroArbol( *(nodo->_registros[ i ] )) );

        if ( nodo->_registros[ i ]->_link )
        {
            Nodo *hijo = 0;

            bool estabaEnCache = nodoEstaEnCache( nodo->_registros[ i ]->_link );
            if ( !cargarNodo( &hijo, nodo->_registros[ i ]->_link ) ) return false;
            if ( !registrosDesdeNodo( hijo, listaRetornada ) ) return false;
            if ( hijo && !estabaEnCache ) liberarNodo( hijo->_addr );
        }
    }

    return true;
}

//==============================================================================
void ArbolBMas::cerrar()
{
    _tamanioMaximoCache = 0;
    limpiarCache();
    if(_raiz != NULL)
        delete _raiz;
    _raiz = 0;
    _estaAbierto = false;
    cerrarControlador();
}

//==============================================================================
bool ArbolBMas::rebalancear( Nodo *nodo, int indicePadre )
{
    int minimo = nodo->_cantidadMaximaRegistros >> 1;

    if ( nodo->count() >= minimo )
    {
        // El nodo esta balanceado
        return true;
    }

    Nodo *padre = 0;
    if ( !cargarNodo( &padre, nodo->_padre ) )
    {
        return false;
    }

    Nodo *nodoIzquierdo = 0, *nodoDerecho = 0;
    Nodo *nodoCombinado = 0;

    if ( 0 == indicePadre - 1 )
    {
        if ( !cargarNodo( &nodoIzquierdo, padre->_menor ) ) return false;
    }
    else if ( indicePadre >= 2 )
    {
        if ( !cargarNodo( &nodoIzquierdo, padre->_registros[ indicePadre - 2 ]->_link ) ) return false;
    }

    if ( padre && indicePadre < padre->count() )
    {
        if ( !cargarNodo( &nodoDerecho, padre->_registros[ indicePadre ]->_link ) ) return false;
    }

    if ( nodoIzquierdo && nodoIzquierdo->count() > minimo )
    {
        // Chequeo el izquierdo
        RegistroArbol registroPadre( *(padre->_registros[ indicePadre - 1 ]) );
        registroPadre._link = nodo->_menor;
        nodo->_menor = nodoIzquierdo->_registros[ nodoIzquierdo->count() - 1 ]->_link;
        nodo->agregarRegistro( registroPadre );
        nodo->_flags = Nodo::NodoCambiado;

        if ( nodo->_menor )
        {
            Nodo *menor = 0;
            if ( !cargarNodo( &menor, nodo->_menor ) ) return false;
            menor->_padre = nodo->_addr;
            menor->_flags = Nodo::NodoCambiado;
        }

        RegistroArbol registroMasGrande;
        nodoIzquierdo->removerEnPosicion( nodoIzquierdo->count() - 1, registroMasGrande );
        padre->_registros[ indicePadre - 1 ]->_clave = registroMasGrande._clave;
        padre->_registros[ indicePadre - 1 ]->_offset = registroMasGrande._offset;
        padre->_flags = Nodo::NodoCambiado;
    }
    else if ( nodoDerecho && nodoDerecho->count() > minimo )
    {
        // Chequeo el derecho
        RegistroArbol registroPadre( *(padre->_registros[ indicePadre ]) );
        registroPadre._link = nodoDerecho->_menor;
        nodo->agregarRegistro( registroPadre );

        if ( nodoDerecho->_menor )
        {
            Nodo *menor = 0;
            if ( !cargarNodo( &menor, nodoDerecho->_menor ) ) return false;
            menor->_padre = nodo->_addr;
            menor->_flags = Nodo::NodoCambiado;
        }

        RegistroArbol registroMasChico;
        nodoDerecho->removerEnPosicion( 0, registroMasChico );
        nodoDerecho->_menor = registroMasChico._link;
        padre->_registros[ indicePadre ]->_clave = registroMasChico._clave;
        padre->_registros[ indicePadre ]->_offset = registroMasChico._offset;
        padre->_flags = Nodo::NodoCambiado;
    }
    else
    {
        // Combino los nodos
        if ( nodoIzquierdo )
        {
            int indice = nodoIzquierdo->agregarRegistro( padre->_registros[ indicePadre - 1 ]->_clave, padre->_registros[ indicePadre - 1 ]->_offset );
            nodoIzquierdo->_registros[ indice ]->_link = nodo->_menor;
            if ( nodo->_menor )
            {
                Nodo *menor = 0;
                if ( !cargarNodo( &menor, nodo->_menor ) ) return false;
                menor->_padre = nodoIzquierdo->_addr;
                menor->_flags = Nodo::NodoCambiado;
            }

            if ( !combinarNodos( nodoIzquierdo, nodo ) ) return false;
            nodoCombinado = nodoIzquierdo;
            padre->removerEnPosicion( indicePadre - 1 );
        }
        else if ( nodoDerecho )
        {
            int indice = nodo->agregarRegistro( padre->_registros[ indicePadre ]->_clave, padre->_registros[ indicePadre ]->_offset );
            nodo->_registros[ indice ]->_link = nodoDerecho->_menor;

            if ( nodoDerecho->_menor )
            {
                Nodo *menor = 0;
                if ( !cargarNodo( &menor, nodoDerecho->_menor ) ) return false;
                menor->_padre = nodo->_addr;
                menor->_flags = Nodo::NodoCambiado;
            }

            if ( !combinarNodos( nodo, nodoDerecho ) ) return false;
            nodoCombinado = nodo;
            padre->removerEnPosicion( indicePadre );
        }

        indicePadre = -1;

        if ( padre && padre->_padre )
        {
            // Busco el indice del padre
            Nodo *pp = 0;
            if ( !cargarNodo( &pp, padre->_padre ) ) return false;

            if ( pp->_menor == padre->_addr )
            {
                indicePadre = 0;
            }
            else
            {
                // Checkeo el padre del padre
                for ( int i = 0; i < pp->count(); i++ )
                {
                    if ( pp->_registros[ i ]->_link == padre->_addr )
                    {
                        indicePadre = i + 1;
                        break;
                    }
                }
            }

            if ( indicePadre >= 0 )
            {
                if ( !rebalancear( padre, indicePadre ) ) return false;
            }
        }
        else
        {
            // Esta es la raiz
            if ( padre && padre->estaVacio() )
            {
                borrarNodo( _raiz );
                _raiz = nodoCombinado;
                setAddrRaiz( _raiz->_addr );
                nodoCombinado->_padre = 0;
                nodoCombinado->_flags = Nodo::NodoCambiado;
            }
        }
    }

    return true;
}

//==============================================================================
bool ArbolBMas::combinarNodos( Nodo *nodoIzquierdo, Nodo *nodoDerecho )
{
    if ( nodoIzquierdo->count() + nodoDerecho->count() > nodoIzquierdo->_cantidadMaximaRegistros )
    {
        // No hay espacio para combinar
        return true;
    }

    for ( int i = 0; i < nodoDerecho->count(); i++ )
    {
        nodoIzquierdo->agregarRegistroAlFinal(*(nodoDerecho->_registros[ i ]));

        if ( nodoDerecho->_registros[ i ]->_link )
        {
            Nodo *link = 0;
            if ( !cargarNodo( &link, nodoDerecho->_registros[ i ]->_link ) ) return false;
            link->_padre = nodoIzquierdo->_addr;
            link->_flags = Nodo::NodoCambiado;
        }

        nodoIzquierdo->_flags = Nodo::NodoCambiado;
    }

    borrarNodo( nodoDerecho );
    return true;
}

//==============================================================================
bool ArbolBMas::retirar( Nodo *nodo, int indiceItem )
{
    Nodo *subarbolIzquierdo = 0, *subarbolDerecho = 0;

    // Obtener subarboles
    if ( 0 == indiceItem )
    {
        if ( !cargarNodo( &subarbolIzquierdo, nodo->_menor ) ) return false;
    }
    else
    {
        if ( !cargarNodo( &subarbolIzquierdo, nodo->_registros[ indiceItem - 1 ]->_link ) ) return false;
    }

    if ( !cargarNodo( &subarbolDerecho, nodo->_registros[ indiceItem ]->_link ) ) return false;
    if ( !subarbolIzquierdo || !subarbolDerecho ) return false;

    if ( subarbolIzquierdo->count() > subarbolDerecho->count() )
    {
        // El subarbol izquierdo es mayor
        if ( subarbolIzquierdo->tieneHijos() )
        {
            string mayorClave;
            long offset;
            Nodo *nodoMasGrande = getNodoMasDerecho( subarbolIzquierdo, mayorClave, offset );
            nodo->_registros[ indiceItem ]->_clave = mayorClave;
            nodo->_registros[ indiceItem ]->_offset = offset;
            nodo->_flags = Nodo::NodoCambiado;
            nodoMasGrande->removerEnPosicion( nodoMasGrande->count() - 1 );

            Nodo *padreMasGrande = 0;
            if ( !cargarNodo( &padreMasGrande, nodoMasGrande->_padre ) ) return false;
            if ( !rebalancear( nodoMasGrande, padreMasGrande->count() ) ) return false;
        }
        else
        {
            nodo->_registros[ indiceItem ]->_clave = subarbolIzquierdo->_registros[ subarbolIzquierdo->count() - 1 ]->_clave;
            nodo->_registros[ indiceItem ]->_offset = subarbolIzquierdo->_registros[ subarbolIzquierdo->count() - 1 ]->_offset;
            nodo->_flags = Nodo::NodoCambiado;
            subarbolIzquierdo->removerEnPosicion( subarbolIzquierdo->count() - 1 );
            if ( !rebalancear( subarbolIzquierdo, indiceItem ) ) return false;
        }
    }
    else
    {
        // El subarbol derecho es mayor
        if ( subarbolDerecho->tieneHijos() )
        {
            string claveMasChica;
            long offset;
            Nodo *nodoMasChico = getNodoMasIzquierdo( subarbolDerecho, claveMasChica, offset );
            nodo->_registros[ indiceItem ]->_clave = claveMasChica;
            nodo->_registros[ indiceItem ]->_offset = offset;
            nodo->_flags = Nodo::NodoCambiado;
            nodoMasChico->removerEnPosicion( 0 );
            if ( !rebalancear( nodoMasChico, 0 ) ) return false;
        }
        else
        {
            nodo->_registros[ indiceItem ]->_clave = subarbolDerecho->_registros[ 0 ]->_clave;
            nodo->_registros[ indiceItem ]->_offset = subarbolDerecho->_registros[ 0 ]->_offset;
            nodo->_flags = Nodo::NodoCambiado;
            subarbolDerecho->removerEnPosicion( 0 );
            if ( !rebalancear( subarbolDerecho, indiceItem + 1 ) ) return false;
        }
    }

    return true;
}

//==============================================================================
Nodo* ArbolBMas::getNodoMasDerecho( Nodo *subarbol, string &claveMasGrande, long &offset )
{
    if ( subarbol->_registros[ subarbol->count() - 1 ]->_link )
    {
        Nodo *linkMasDerecho = 0;
        if ( !cargarNodo( &linkMasDerecho, subarbol->_registros[ subarbol->count() - 1 ]->_link ) )
        {
            return 0;
        }

        return getNodoMasDerecho( linkMasDerecho, claveMasGrande, offset );
    }
    else
    {
        claveMasGrande = subarbol->_registros[ subarbol->count() - 1 ]->_clave;
        offset = subarbol->_registros[ subarbol->count() - 1 ]->_offset;
        return subarbol;
    }
}

//==============================================================================
Nodo* ArbolBMas::getNodoMasIzquierdo( Nodo *subarbol, string &claveMasChica, long &offset )
{
    if ( subarbol->_menor )
    {
        Nodo *linkMasInzquierdo = 0;
        if ( !cargarNodo( &linkMasInzquierdo, subarbol->_menor ) )
        {
                return 0;
        }

        return getNodoMasIzquierdo( linkMasInzquierdo, claveMasChica, offset );
    }
    else
    {
        claveMasChica = subarbol->_registros[ 0 ]->_clave;
        offset = subarbol->_registros[ 0 ]->_offset;
        return subarbol;
    }
}

//==============================================================================
bool ArbolBMas::partirNodo( Nodo *nodo,	RegistroArbol &medio, Nodo **nodoDerecho )
{
    RegistroArbol insertElem(medio);

    int indiceMedio = getRegistroDelMedio( nodo, medio );

    Nodo nodoIzquierdoTmp;
    *nodoDerecho = getNodoNuevo();

    // Reemplazo el elemento
    nodo->removerEnPosicion( indiceMedio );
    nodo->agregarRegistro( insertElem );

    ( *nodoDerecho )->_menor = medio._link;
    if ( medio._link )
    {
            Nodo *link = 0;
            if ( !cargarNodo( &link, medio._link ) ) return false;
            link->_padre = ( *nodoDerecho )->_addr;
            link->_flags = Nodo::NodoCambiado;
    }
    nodoIzquierdoTmp._menor = nodo->_menor;


    for ( int i = 0; i < nodo->count(); i++ )
    {
        if ( nodo->_registros[ i ]->getClave() < medio.getClave() )
        {
            nodoIzquierdoTmp.agregarRegistroAlFinal(*nodo->_registros[ i ]);
        }
        else
        {
            ( *nodoDerecho )->agregarRegistroAlFinal(*nodo->_registros[ i ]);

            if ( nodo->_registros[ i ]->_link )
            {
                Nodo *link = 0;

                bool estabaEnCache = nodoEstaEnCache( nodo->_registros[ i ]->_link );
                if ( !cargarNodo( &link, nodo->_registros[ i ]->_link ) ) return false;
                link->_padre = ( *nodoDerecho )->_addr;
                link->_flags = Nodo::NodoCambiado;
                if ( link && !estabaEnCache ) liberarNodo( link->_addr );
            }
        }
    }

    // Preparo el resultado
    ( *nodoDerecho )->_padre = nodo->_padre;
    nodoIzquierdoTmp._padre = nodo->_padre;
    nodoIzquierdoTmp._addr = nodo->_addr;
    *nodo = nodoIzquierdoTmp;

    ( *nodoDerecho )->_flags = Nodo::NodoCambiado;
    nodo->_flags = Nodo::NodoCambiado;

    medio._link = ( *nodoDerecho )->_addr;

    return true;
}

//==============================================================================
int ArbolBMas::getRegistroDelMedio( Nodo *nodo, RegistroArbol &elem )
{
    int posicionMedio = nodo->count() >> 1;
    elem = *nodo->_registros[ posicionMedio ];
    return posicionMedio;
}

//==============================================================================
bool ArbolBMas::nodoEstaEnCache( int addr )
{
    return (buscarEnCache(addr) != NULL);
}

//==============================================================================
Nodo* ArbolBMas::encontrarNodo( Nodo *nodo, const string &clave, int &indiceRetornado, int &indicePadre, bool &econtrado )
{
    if ( !nodo ) return 0;

    econtrado = false;
    indiceRetornado = -1;

    if ( nodo->buscarRegistro( clave, indiceRetornado ) )
    {
        // Key econtrado
        econtrado = true;
        return nodo;
    }

    int link = 0;

    if ( indiceRetornado > 0 )
    {
        link = nodo->_registros[ indiceRetornado - 1 ]->_link;
    }
    else
    {
        link = nodo->_menor;
    }

    if ( !link )
    {
        // No hay mas hijos
        return nodo;
    }
    else if ( link == nodo->_addr )
    {
        //En caso de un indice corrupto
        return 0;
    }

    Nodo *nextLink = 0;
    if ( !cargarNodo( &nextLink, link ) ) return 0;

    indicePadre = indiceRetornado;
    return encontrarNodo(nextLink, clave, indiceRetornado, indicePadre, econtrado);
}

//==============================================================================
bool ArbolBMas::abrir(string url)//(int maxTamanioCache
{
    //Trata de abrir los archivos para comprobar si existen. Si es así no hace nada, si no,
    //los crea y los inicializa.
    _storage.open(url.c_str(), ios::binary | ios::in);
    if(!_storage.is_open())
    {
        //Si no existia lo creo
        _storage.open(url.c_str(), ios::binary | ios::out);
        _storage.close();
    }
    else
        _storage.close();


    _storage.open(url.c_str(), ios::in | ios::out | ios::binary);


    if (!_storage.is_open())
    {
        limpiarCache();
        delete _raiz;
        _raiz = 0;
        cerrarControlador();
        cout<<"NO SE PUDO ABRIR EL ARCHIVO DEL ARBOL"<<endl;

        return false;
    }

    ////1-Si el arbol esta vacio, creo un nodo raiz
    ////2.-Si el arbol no esta vacio, cargo el nodo raiz
    ////	2.1-Abro el archivo de nodos borrados o libres y me situo al principio
    if(tamanioStorage() == 0)
    {
        _storage.seekp(0);
        //Escribo la direccion de la raiz cuando se crea por primera vez el arbol
        //_storage<<_finCamposControl<<"|";	//Es la direccion cuando se crea por primera vez la raiz
        long finCamposControl = ArbolBMas::_finCamposControl;
        _storage.write(reinterpret_cast<char *>(&(finCamposControl)), sizeof((finCamposControl)));


        _raiz = new Nodo();
        _raiz->_addr = finCamposControl;
        setAddrRaiz(_raiz->_addr);
        _raiz->_flags = Nodo::NodoCambiado;

        _cache.push_back(_raiz);

        //Creo el espacio para los campos de control q faltan
        long tamanioALimpiar = finCamposControl - sizeof(finCamposControl); //Modo texto _finCamposControl-4;
        char byteLimpio = 0;

        for(int i = 0; i < tamanioALimpiar; i++) //finCamposControl-4 por q es modo texto sino va sin el 4
        {
        	//_storage<<" ";
        	_storage.write(reinterpret_cast<char *>(&(byteLimpio)), sizeof((byteLimpio)));
        }
        _storage.seekp(0);

        guardarNodo(_raiz);
    }
    else
    {
        int addr = getAddrRaiz();
        if ( addr )
        {
            if ( !cargarNodo( &_raiz, addr ) )
            {
                return false;
            }
        }
    }

    _estaAbierto = true;

    return true;
}

//==============================================================================
bool ArbolBMas::estaAbierto()
{
    return (_storage.is_open());
}

//==============================================================================
int ArbolBMas::tamanioStorage()
{
    _storage.seekg(0, ios::end);
    int tamanio = _storage.tellg();

    //Si lo de abajo lo comento y funciona, borrarlo
    _storage.seekg(0, ios::beg);

    return tamanio;
}

//==============================================================================
Nodo* ArbolBMas::getNodoNuevo()
{
    int addr = 0;

    // ARBOL: freeChunks = nodos liberados q se guardan en un archivo
    if ( hayEspacioLibre() )
    {
        //freeChunks_.erase( freeChunks_.begin() );

        //addr = Obtener direccion de nodo libre del primero q esta en la lista o ultimo y borrarlo
    }
    else
    {
        addr = tamanioStorage();
    }

    Nodo *nodoNuevo = new Nodo();
    nodoNuevo->_addr = addr;
    nodoNuevo->_flags = Nodo::NodoCambiado;

    guardarNodo( nodoNuevo );
    _cache.push_back(nodoNuevo);

    return nodoNuevo;
}

//==============================================================================
bool ArbolBMas::hayEspacioLibre(){
    return false;
}


//Guarda en el archivo de espacio libre, los espacios q se generan
//bool ArbolBMas::allocFreeSpace()
//{
//	gint64 fullSize = storage_->size() - sizeof( int )*3;
//	if ( !fullSize ) return true;
//
//	qint32 nodoCount = fullSize / sizeof( Nodo );
//	gint64 offset = sizeof( int )*3;
//
//	for ( int i = 0; i < nodoCount; i++ )
//	{
//		freeChunks_.insert( offset );
//		offset += sizeof( Nodo );
//	}
//
//	if ( !root_ ) return false;
//
//	if ( !readAllOffsets( root_ ) ) return false;
//	freeChunks_.erase( root_->addr_ );
//	releaseCache();
//	return true;
//}

//	===================================================================
//	StreamBTreeController< nodoSize, string, long >::readAllOffsets
//bool Stream::readAllOffsets(Nodo *nodo )
//{
//	if ( !nodo ) return true;
//
//	if ( nodo->less_ )
//	{
//		Nodo *less;
//		if ( !loadnodo( &less, nodo->less_ ) ) return false;
//		freeChunks_.erase( nodo->less_ );
//		readAllOffsets( less );
//		if ( less ) releasenodo( less->addr_ );
//
//		// Iterate other children if we have a less one
//		for ( int i = 0; i < nodo->count(); i++ )
//		{
//			Nodo *kid = 0;
//
//			if ( nodo->elems_[ i ].link_ )
//			{
//				if ( !loadnodo( &kid, nodo->elems_[ i ].link_ ) ) return false;
//			}
//			freeChunks_.erase( nodo->elems_[ i ].link_ );
//
//			if ( kid )
//			{
//				readAllOffsets( kid );
//				releasenodo( kid->addr_ );
//			}
//		}
//	}
//
//	return true;
//}

//==============================================================================
void ArbolBMas::borrarNodo( Nodo* nodo )
{
    int indice = -1;

    if(buscarEnCache(nodo->_addr, indice))
    {
        if(indice >= 0)
        {
            borrarDeCache(indice);
        }
    }

    //freeChunks_.insert( nodo->addr_ );
    delete nodo;
}

//==============================================================================
Nodo * ArbolBMas::buscarEnCache(int addr)
{
    for(unsigned int i = 0; i < _cache.size(); i++)
    {
        if(_cache[i]->_addr == addr)
                return _cache[i];
    }

    return NULL;
}

//==============================================================================
Nodo * ArbolBMas::buscarEnCache(int addr, int &pos)
{
    pos = -1;
    for(unsigned int i = 0; i < _cache.size(); i++)
    {
        if(_cache[i]->_addr == addr)
        {
                pos = i;
                return _cache[i];
        }
    }

    return NULL;
}

//==============================================================================
bool ArbolBMas::cargarNodo(Nodo **nodo, int addr )
{
    if ( !addr )
    {
        *nodo = 0;
        return true;
    }

    if((*nodo = buscarEnCache(addr)) == NULL)
    {
        *nodo = new Nodo();
        _storage.seekg( addr );
        *nodo = Nodo::Leer(_storage);

        if ( addr == ( *nodo )->_addr )
        {
            _cache.push_back(*nodo);
        }
    }

    if ( ( *nodo )->_addr != addr )
    {
        cout<<"El storage esta corrupto!"<<endl;
        return false;
    }

    return true;
}

//==============================================================================
void ArbolBMas::guardarNodo( Nodo *nodo )
{
    nodo->_flags = 0;
    nodo->Guardar(_storage);
}

//==============================================================================
void ArbolBMas::liberarNodo( int addr )
{
    int posicionEnCache = -1;

    Nodo * nodo = buscarEnCache(addr, posicionEnCache);

    if( posicionEnCache > 0 )
    {
        if ( Nodo::NodoCambiado == nodo->_flags )
        {
            impactarCambios();
            liberarNodo( addr );
            return;
        }

        if ( nodo != _raiz )
        {
            delete nodo;
            borrarDeCache(posicionEnCache);
        }
    }
}

//==============================================================================
void ArbolBMas::borrarDeCache(int indice)
{
    _cache.erase(_cache.begin() + indice);
}

//==============================================================================
long ArbolBMas::getAddrRaiz()
{
    long addr = 0;

    _storage.seekg(0);
//    char delimitador;
//    _storage>>addr>>delimitador;
    _storage.read( ( char* ) &addr, sizeof( addr ) );

    return addr;
}

//==============================================================================
void ArbolBMas::setAddrRaiz(long addr)
{
    _addrRaiz = addr;
}

//==============================================================================
Nodo * ArbolBMas::getRaiz()
{
    return _raiz;
}

//==============================================================================
void ArbolBMas::cerrarControlador()
{
    _cache.clear();

    //Esto es lo ultimo que debo hacer
    if(_storage.is_open())
        _storage.close();

//	if ( storage_ )
//	{
//		if ( !freeChunks_.empty() )
//		{
//			// Save removed nodos list
//			guint rlistAddr = storage_->size();
//			QByteArray rblock;
//			FreeChunksSet::const_iterator iter = freeChunks_.begin();
//			guint aaddr = 0;
//			for ( ; iter != freeChunks_.end(); iter++ )
//			{
//				aaddr = *iter;
//				rblock.resize( rblock.size() + sizeof( aaddr ) );
//				::memcpy( rblock.data() + rblock.size() - sizeof( aaddr ),
//					( char* ) &aaddr, sizeof( aaddr ) );
//			}
//			freeChunks_.clear();
//
//			storage_->seek( rlistAddr );
//			storage_->write( rblock.data(), rblock.size() );
//
//			storage_->seek( constRemovedAddr );
//			storage_->write( ( char* ) &rlistAddr, sizeof( rlistAddr ) );
//		}
//
//		storage->close();
//		storage_ = 0;
//	}
}

//==============================================================================
void ArbolBMas::impactarCambios()
{
    if ( !estaAbierto() ) return;

    // Grabo los nodos en cache
    int tamanioCache = _cache.size();
    for (int i = 0; i < tamanioCache ;i++)
    {
        if ( Nodo::NodoCambiado == _cache[i]->_flags )
        {
            // Guardo el nodo
            guardarNodo( _cache[i] );
        }
    }

    // Escribo la raiz
    if ( _addrRaiz )
    {
        _storage.seekp(0); //posicion donde guardo direccion de la raiz
        //Aca despues tengo q usar write
        //_storage<<_addrRaiz<<"|";
        _storage.write(reinterpret_cast<char *>(&(_addrRaiz)), sizeof((_addrRaiz)));

        _addrRaiz = 0;
    }
}

//==============================================================================
void ArbolBMas::liberarCache()
{
    //if ( cache_.size() > maxCacheSize_ )
    //{
        limpiarCache();
    //}
}

//==============================================================================
void ArbolBMas::limpiarCache()
{
    impactarCambios();

    //Vacio la cache eliminando los nodos q no sean la raiz
    for(unsigned int i = 0; i <_cache.size(); i++)
    {
        if ( _cache[i] != _raiz )
        {
            delete _cache[i];
        }
    }

    //Limpio el vector cache de referencias invalidas
    _cache.clear();

    //Si la raiz esta en memoria la cargo a la cache
    if ( _raiz )
    {
        _cache.push_back(_raiz);
    }
}
