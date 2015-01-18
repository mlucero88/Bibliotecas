/*
 * DataGetter.cpp
 *
 *  Created on: 13/10/2011
 *      Author: administrador
 */

#include "DataGetter.h"

DataGetter::DataGetter() {}

DataGetter::~DataGetter() {}


bool DataGetter::getListasPorEleccion(vector<Lista*> & vecListas, Eleccion &eleccion){
	/* Chequeo si el vector de Listas esta vacio, si no lo vacio */
	if(vecListas.size() != 0) {
		int cantidad = vecListas.size();
		for(int i=0;i<cantidad;i++){
			if(vecListas[i] != NULL) {
				delete vecListas[i];
				vecListas[i] = NULL;
			}
		}
		vecListas.clear();
	}

	/* Obtengo la clave que quiero buscar */
	string claveInicial = Utilidades::obtenerClaveEleccion(eleccion.getFecha(),eleccion.getCargo().getCargoPrincipal());

	/* Pongo la clave hasta la que quiero buscar */
	string claveFinal = claveInicial + "&";

	/* Abro el arbol de Lista */
	ArbolBMas *arbolLista = new ArbolBMas();
	arbolLista->abrir((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_ARBOL_LISTA));

	/* Busco en el arbol todos los ids de listas con la fecha y cargo pedidos */
	list<RegistroArbol *> IDsListas;
	if (!arbolLista->buscar(IDsListas, claveInicial, claveFinal)) return false;		// Si no habia ninguna coincidencia, se devuelve false
	arbolLista->cerrar();
	delete arbolLista;
	arbolLista = 0;

	/* Busco en el hash de id_lista/offset todas las listas obtenidas */
    string arch_registros = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDLISTA_REGS));
    string arch_bloq_libres = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDLISTA_BLOQ_LIB));
    string arch_tabla = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDLISTA_TABLA));
    hash_extensible *hash_lista = new hash_extensible(arch_registros,arch_bloq_libres,arch_tabla);

    Lista aAgregar;
    DataAccess dataAccess;
    RegistroArbol *registroEnLista = NULL;
    RegistroIndice aBuscarID(" ",0);
   	RegistroIndice *returnRegID = NULL;
    list<RegistroArbol *>::iterator it;
    for (it = IDsListas.begin(); it != IDsListas.end(); it++){
     	registroEnLista = *it;
     	aBuscarID.setClave(Utilidades::toString(registroEnLista->getOffset()));
     	returnRegID = hash_lista->buscar(&aBuscarID);
     	if (returnRegID == NULL) throw VotoElectronicoExcepcion("No se encontro el id de la lista");
     	try{
         	dataAccess.Leer(aAgregar,returnRegID->getOffset());
         	vecListas.push_back(new Lista(aAgregar));
     	}
     	catch(string str){
     		cout << endl << str << endl;
     		/* No se agrega la lista porque tiene informacion erronea */
     	}
    }
    delete hash_lista;
    hash_lista = NULL;
    if (vecListas.size() == 0) return false;
    return true;
}


bool DataGetter::getEleccionesPorFechayDistrito(vector<Eleccion*> & vecElecciones,string fecha, Votante &votante) {

	/* Chequeo si el vector de Elecciones esta vacio, si no lo vacio */
	if(vecElecciones.size() > 0) {
		int cantidad = vecElecciones.size();
		for(int i=0;i<cantidad;i++){
			if(vecElecciones[i] != NULL) {
				delete vecElecciones[i];
				vecElecciones[i] = NULL;
			}
		}
		vecElecciones.clear();
	}

	/* Obtengo la clave que quiero buscar */
	string claveInicial = Utilidades::indexarFecha(fecha);

	/* Pongo la clave hasta la que quiero buscar */
	string claveFinal = claveInicial + "&";

	/* Abro el arbol de Eleccion */
	ArbolBMas *arbolEleccion = new ArbolBMas();
	arbolEleccion->abrir((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_ARBOL_ELECCION));

	/* Busco en el arbol todos los ids de elecciones con la fecha pedida */
	list<RegistroArbol *> IDsElecciones;
	bool retorno = false;
    if (!arbolEleccion->buscar(IDsElecciones, claveInicial, claveFinal)) retorno = true;	// Si no habia ninguna coincidencia, se devuelve false
    arbolEleccion->cerrar();
    delete arbolEleccion;
    arbolEleccion = NULL;
    if (retorno) return false;

    /* A partir de la lista de IDs de elecciones se descartan las elecciones en las que ya participo el votante */
    int cantidadEleccionesVotante = votante.getElecciones().size();
    vector<long int> vecIDs;
    long int aux;
    bool encontrado = false;
    int j = 0;
    RegistroArbol *registroEnLista = NULL;
    list<RegistroArbol *>::iterator it;
    for (it = IDsElecciones.begin(); it != IDsElecciones.end(); it++){
    	registroEnLista = *it;
    	aux = registroEnLista->getOffset();
    	j = 0;
    	encontrado = false;
    	while ((!encontrado) && (j != cantidadEleccionesVotante)){
    		if (aux == votante.getElecciones()[j]->getId()) {
    			encontrado = true;
    		} else j++;
    	}
    	if (!encontrado) vecIDs.push_back(aux);
    }
    if (vecIDs.size() == 0) return false;		// Se descartaron todas las elecciones xq el votante ya habia votado en todas.

    /* Busco en el hash de id_eleccion/offset todas las elecciones restantes, y comparo si el distrito del votante pertenece a la eleccion */
    /* Si pertenece, se agrega a esa eleccion para retornar en el vector de elecciones */
    string arch_registros = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDELECCION_REGS));
    string arch_bloq_libres = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDELECCION_BLOQ_LIB));
    string arch_tabla = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDELECCION_TABLA));
    hash_extensible *hash_eleccion = new hash_extensible(arch_registros,arch_bloq_libres,arch_tabla);
    int cantidadIDs = vecIDs.size();
    RegistroIndice aBuscarID(" ",0);
   	RegistroIndice *returnRegID = NULL;
   	DataAccess dataAccess;
   	Eleccion aAgregar;
   	string nombreDistrito = votante.getDistrito().getNombre();
   	int cantidadDistritos;

    for(int k=0; k<cantidadIDs;k++) {
    	aBuscarID.setClave(Utilidades::toString(vecIDs[k]));
    	returnRegID = hash_eleccion->buscar(&aBuscarID);
    	if (returnRegID == NULL) throw VotoElectronicoExcepcion("No se encontro el id de la eleccion");
    	try{
    		dataAccess.Leer(aAgregar,returnRegID->getOffset());
   	  		j = 0;
   	  		encontrado = false;
   	  		cantidadDistritos = aAgregar.getDistritos().size();
   	  		while ((!encontrado) && (j != cantidadDistritos)){
   	  			if (aAgregar.getDistritos()[j]->getNombre() == nombreDistrito) {
   	  				encontrado = true;
   	  				vecElecciones.push_back(new Eleccion(aAgregar));
   	  			} else j++;
   	  		}
    	}
    	catch(string str){
    		cout << endl << str << endl;
    		/* No se agrega la eleccion porque tiene informacion erronea */
    	}
    }
    delete hash_eleccion;
    hash_eleccion = NULL;
    if (vecElecciones.size() == 0) return false;
    return true;
}


void DataGetter::getConteosPorDistrito(vector<Conteo *> & conteos, Distrito& distrito){

    //obtiene los offsets de los conteos requeridos
    ArbolBMas *arbol = new ArbolBMas();
    arbol->abrir((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_ARBOL_REPORTE_DISTRITO));

    string claveInicial = distrito.getNombre();
    Utilidades::formatearClave(claveInicial);
    string claveFinal = claveInicial + "&";

    list<RegistroArbol *> offsetsConteos;
    arbol->buscar(offsetsConteos, claveInicial, claveFinal);

    //a partir de la lista de offsets crea un vector de Conteo* y lo devuelve
    DataAccess dataAccess;
    RegistroArbol *registroEnLista = NULL;
    list<RegistroArbol *>::iterator it;
    Conteo contAux;
    conteos.clear();
    for (it = offsetsConteos.begin(); it != offsetsConteos.end(); it++){
    	registroEnLista = *it;
    	try {
            dataAccess.Leer(contAux, registroEnLista->getOffset());
            conteos.push_back(new Conteo(contAux));
    	}
    	catch(string str){
    		cout << endl << str << endl;
    		/* No se agrega el conteo porque tiene informacion erronea */
    	}

    }
}


void DataGetter::getConteosPorLista(vector<Conteo *> & conteos, Lista& lista){

    //obtiene los offsets de los conteos requeridos
    ArbolBMas *arbol = new ArbolBMas();
    arbol->abrir((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_ARBOL_REPORTE_LISTA));

    string fecha = Utilidades::indexarFecha(lista.getEleccion().getFecha());
    string claveInicial = fecha +"$" +lista.getEleccion().getCargo().getCargoPrincipal() +"$" +lista.getNombre();
    Utilidades::formatearClave(claveInicial);
    string claveFinal = claveInicial + "&";

    list<RegistroArbol *> offsetsConteos;
    arbol->buscar(offsetsConteos, claveInicial, false, claveFinal);

    //a partir de la lista de offsets crea un vector de Conteo* y lo devuelve
    DataAccess dataAccess;
    RegistroArbol *registroEnLista = NULL;
    list<RegistroArbol *>::iterator it;
    Conteo contAux;
    conteos.clear();
    for (it = offsetsConteos.begin(); it != offsetsConteos.end(); it++){
    	registroEnLista = *it;
    	try {
            dataAccess.Leer(contAux, registroEnLista->getOffset());
            conteos.push_back(new Conteo(contAux));
    	}
    	catch(string str){
    		cout << endl << str << endl;
    		/* No se agrega el conteo porque tiene informacion erronea */
    	}
    }
}


void DataGetter::getConteosPorEleccion(vector<Conteo *> & conteos, Eleccion& eleccion){
    
    //obtiene los offsets de los conteos requeridos
    ArbolBMas *arbol = new ArbolBMas();
    arbol->abrir((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_ARBOL_REPORTE_ELECCION));

    string fecha = Utilidades::indexarFecha(eleccion.getFecha());
    string claveInicial = fecha +"$" +eleccion.getCargo().getCargoPrincipal();
    Utilidades::formatearClave(claveInicial);
    string claveFinal = claveInicial + "&";

    list<RegistroArbol *> offsetsConteos;
    arbol->buscar(offsetsConteos, claveInicial, false, claveFinal);

    //a partir de la lista de offsets crea un vector de Conteo* y lo devuelve
    DataAccess dataAccess;
    RegistroArbol *registroEnLista = NULL;
    list<RegistroArbol *>::iterator it;
    Conteo contAux;
    conteos.clear();
    for (it = offsetsConteos.begin(); it != offsetsConteos.end(); it++){
    	registroEnLista = *it;
    	try {
            dataAccess.Leer(contAux, registroEnLista->getOffset());
            conteos.push_back(new Conteo(contAux));
    	}
    	catch(string str){
    		cout << endl << str << endl;
    		/* No se agrega el conteo porque tiene informacion erronea */
    	}
    }
}
