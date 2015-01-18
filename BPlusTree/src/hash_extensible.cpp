#include "hash_extensible.h"
#include <cstdlib>


hash_extensible::hash_extensible(string nombre_arch_bloques, string nombre_arch_bloques_libres, string nombre_arch_tabla) {

	this->nombre_arch_bloques = nombre_arch_bloques;
	this->nombre_arch_bloques_libres = nombre_arch_bloques_libres;
	this->nombre_arch_tabla = nombre_arch_tabla;

    /**Trata de abrir los archivos para comprobar si existen. Si es así no hace nada, si no,
     * los crea y los inicializa.
     */
    bool no_existen = false;

    fstream archTabla(this->nombre_arch_tabla.c_str(), ios::binary | ios::in);
    if(!archTabla.is_open()) no_existen = true;
    else archTabla.close();

    fstream archBloques(this->nombre_arch_bloques.c_str(), ios::binary | ios::in);
    if(!archBloques.is_open()) no_existen = true;
    else archBloques.close();

    fstream archLibres(this->nombre_arch_bloques_libres.c_str(), ios::binary | ios::in);
    if(!archLibres.is_open()) no_existen = true;
    else archLibres.close();

    if(no_existen){
        //escribe el tamaño y la primera dirección de la tabla de dispersión
        archTabla.open(this->nombre_arch_tabla.c_str(), ios::binary | ios::out);
        unsigned int tamanio_tabla = 1;
        archTabla.seekp(0);
        archTabla.write((char *)&tamanio_tabla, sizeof(unsigned int));
        unsigned int direccion = 0;
        archTabla.write((char *)&direccion, sizeof(unsigned int));
        archTabla.close();

        //escribe el primer bloque en el hash
        unsigned int tam = obtener_tamanio_tabla();
        BloqueHash *primer_bloque = new BloqueHash(tam);
        primer_bloque->Persistir(this->nombre_arch_bloques.c_str(), 0);
        delete primer_bloque;

        //inicializa el archivo de bloques libres con cantidad = 0
        archLibres.open(this->nombre_arch_bloques_libres.c_str(), ios::binary | ios::out);
        unsigned int cantidad = 0;
        archLibres.seekp(0);
        archLibres.write((char *)&cantidad, sizeof(unsigned int));
        archLibres.close();
    }
}

////////////////////////////

unsigned int hash_extensible::funcion_hashing(RegistroIndice *registro){

	string clave = registro->getClave();
	unsigned int tam_tabla = obtener_tamanio_tabla();

	int valor = 0;
    for(unsigned int i = 0; i < clave.length() ; i++ )
        valor = 37 * valor + clave.at( i );

    valor %= tam_tabla;
    if( valor < 0 )
        valor += tam_tabla;

    return valor + 1;
	// suma uno para no tener en cuenta la primera posición de la tabla,
	// donde se guarda la longitud de la misma
}

///////////////////////////

unsigned int hash_extensible::obtener_offset_bloque(unsigned int posicion_en_tabla_dispersion){

	fstream archTabla(this->nombre_arch_tabla.c_str(), ios::binary | ios::in);
	archTabla.seekg(posicion_en_tabla_dispersion*sizeof(unsigned int));
	unsigned int nro_de_bloque;
	archTabla.read((char *)&nro_de_bloque, sizeof(unsigned int));
	archTabla.close();

	int tamanio_bloque = BloqueHash::getTamanioBloques();
	return nro_de_bloque*tamanio_bloque;
}

////////////////////////////

void hash_extensible::persistir_vector(vector<unsigned int> *vec, string nombre_archivo){

    fstream archVector;
	archVector.open(nombre_archivo.c_str(), ios::binary | ios::out);

	archVector.seekp(0);
	for(unsigned int i = 0; i <= (*vec)[0]; i++)
		archVector.write((char *)&(*vec)[i], sizeof(unsigned int));

	archVector.close();
}

////////////////////////////

unsigned int hash_extensible::extraer_nro_nuevo_bloque(){
	//- toma el menor numero de bloque del archivo de bloques libres y lo elimina del archivo
	//- si no hay bloques libres, devuelve el número de bloque en uso más alto + 1
	unsigned int num_bloque;
	unsigned int posicion_num_bloque;
	vector<unsigned int> *lista_bloques = cargar_archivo_bloques_libres();

	if ((*lista_bloques)[0] != 0){
		num_bloque = (*lista_bloques)[1];
		posicion_num_bloque = 1;
		for(unsigned int i = 2; i < lista_bloques->size(); i++){
			if ((*lista_bloques)[i] < num_bloque){
				num_bloque = (*lista_bloques)[i];
				posicion_num_bloque = i;
			}
		}
		(*lista_bloques)[posicion_num_bloque] = (*lista_bloques)[(*lista_bloques)[0]];
		(*lista_bloques)[0]--;
		persistir_vector(lista_bloques, this->nombre_arch_bloques_libres.c_str());
		delete lista_bloques;
		return num_bloque;
	}
	delete lista_bloques;

	vector<unsigned int> *tabla_disp = cargar_tabla_dispersion();
	int max = -1;
	for(unsigned int i = 1; i <= (*tabla_disp)[0]; i++)
		if((int)(*tabla_disp)[i] > max) max = (*tabla_disp)[i];

    delete tabla_disp;
	return max + 1;
}

/////////////////////////////

vector<unsigned int>* hash_extensible::cargar_archivo_bloques_libres(){

	fstream archLibres(this->nombre_arch_bloques_libres.c_str(), ios::binary | ios::in);
	archLibres.seekg(0);
	vector<unsigned int> *vector_aux = new vector<unsigned int>;
	unsigned int aux;
	archLibres.read((char *)&aux, sizeof(unsigned int));
	vector_aux->reserve(sizeof(unsigned int)*(aux + 1));
	(*vector_aux)[0] = aux;

	for(unsigned int i = 1; i <= (*vector_aux)[0]; i++){
		archLibres.read((char *)&aux, sizeof(unsigned int));
		(*vector_aux)[i] = aux;
	}

	archLibres.close();
	return vector_aux;
}

////////////////////////////

vector<unsigned int>* hash_extensible::cargar_tabla_dispersion(){

	fstream archTabla(this->nombre_arch_tabla.c_str(), ios::binary | ios::in);
	archTabla.seekg(0);
	vector<unsigned int> *vector_aux = new vector<unsigned int>;
	unsigned int aux;
	archTabla.read((char *)&aux, sizeof(unsigned int));
	vector_aux->reserve(sizeof(unsigned int)*(aux + 1));
	(*vector_aux)[0] = aux;

	for(unsigned int i = 1; i <= (*vector_aux)[0]; i++){
		archTabla.read((char *)&aux, sizeof(unsigned int));
		(*vector_aux)[i] = aux;
	}

	archTabla.close();
	return vector_aux;
}

///////////////////////

unsigned int hash_extensible::obtener_tamanio_tabla(){

	fstream archTabla(this->nombre_arch_tabla.c_str(), ios::binary | ios::in);
	archTabla.seekg(0);
	unsigned int aux;
	archTabla.read((char *)&aux, sizeof(unsigned int));
	archTabla.close();
	return aux;
}

//////////////////////

RegistroIndice* hash_extensible::buscar(RegistroIndice *registro){
	//obtiene la posicion del bloque donde está el registro
	unsigned int offset_bloque = obtener_offset_bloque(funcion_hashing(registro));

	//lee el bloque a memoria para buscar el registro.
	//Si no está lanza excepción, si está lo devuelve por parámetro
	BloqueHash *bloque_aux = new BloqueHash(0);
	bloque_aux = (BloqueHash*)bloque_aux->Leer(this->nombre_arch_bloques.c_str(), offset_bloque);

	RegistroIndice *reg_aux;
	reg_aux = bloque_aux->Buscar(registro);
	delete bloque_aux;
	return reg_aux;
}

////////////////////////

void hash_extensible::duplicar_tabla(unsigned int posicion_en_tabla){

	vector<unsigned int> *tabla_en_memoria = cargar_tabla_dispersion();
	unsigned int tamanio_tabla = (*tabla_en_memoria)[0];
	tabla_en_memoria->reserve(tamanio_tabla*2 + 1);

	//duplica la tabla en memoria
	for(unsigned int i = 1; i <= tamanio_tabla; i++)
		(*tabla_en_memoria)[i + tamanio_tabla] = (*tabla_en_memoria)[i];

	//lee el nro. de bloque desbordado, y en la primera mitad de la
	//tabla lo reemplaza por el nuevo número de bloque
	unsigned int nro_bloque_desbordado = (*tabla_en_memoria)[posicion_en_tabla];
	unsigned int num_nuevo_bloque = extraer_nro_nuevo_bloque();
	(*tabla_en_memoria)[posicion_en_tabla] = num_nuevo_bloque;

	//duplica el tamaño de la tabla y guarda la tabla duplicada en disco
	(*tabla_en_memoria)[0] *= 2;
	persistir_vector(tabla_en_memoria, this->nombre_arch_tabla.c_str());
	delete tabla_en_memoria;

	//en el archivo de bloques inserta uno nuevo, y al nuevo bloque y al
	//desbordado les asigna tamanio dispersion = tamanio tabla
	BloqueHash *nuevo_bloque = new BloqueHash(obtener_tamanio_tabla());
	nuevo_bloque->Persistir(this->nombre_arch_bloques.c_str(), num_nuevo_bloque*BloqueHash::getTamanioBloques());
	delete nuevo_bloque;

	unsigned int offset_bloque = nro_bloque_desbordado * BloqueHash::getTamanioBloques();
	BloqueHash bloque_aux(0);
	BloqueHash *bloque_desbordado;
	bloque_desbordado = (BloqueHash*) bloque_aux.Leer(this->nombre_arch_bloques.c_str(), offset_bloque);
	bloque_desbordado->setTamanioDispersion(obtener_tamanio_tabla());
	// Borra todos los registros del bloque y los guarda en memoria
	list<RegistroIndice *> *registros_a_redispersar = bloque_desbordado->VaciarBloque();

	//vuelve a guardar el bloque, ahora sin registros y con su nuevo tamaño de dispersión:
	bloque_desbordado->Persistir(this->nombre_arch_bloques.c_str(), offset_bloque);
	delete bloque_desbordado;

	//redispersa los registros del bloque desbordado
	RegistroIndice *registroEnLista = NULL;
	list<RegistroIndice *>::iterator it;
	for (it = registros_a_redispersar->begin(); it != registros_a_redispersar->end(); it++){
		registroEnLista = *it;
		guardar(registroEnLista);
	}
	delete registros_a_redispersar;
}

/////////////////////////

void hash_extensible::incrementar_tabla(unsigned int posicion_en_tabla){
	//lee y duplica el tamaño de dispersión del bloque desbordado
	unsigned int offset_bloque = obtener_offset_bloque(posicion_en_tabla);
	BloqueHash *bloque_desbordado = new BloqueHash(0);
	bloque_desbordado = (BloqueHash*)bloque_desbordado->Leer(this->nombre_arch_bloques.c_str(), offset_bloque);
	unsigned int nuevo_tamanio_dispersion = bloque_desbordado->getTamanioDispersion()*2;
	bloque_desbordado->setTamanioDispersion(nuevo_tamanio_dispersion);

	//Borra todos los registros del bloque y los guarda en memoria
	list<RegistroIndice *> *registros_a_redispersar = bloque_desbordado->VaciarBloque();
	//vuelve a guardar el bloque, ahora sin registros y con su nuevo tamaño de dispersión:
	bloque_desbordado->Persistir(this->nombre_arch_bloques.c_str(), offset_bloque);
	delete bloque_desbordado;

	//inserta un nuevo bloque con el nuevo tamaño de dispersión
	BloqueHash *nuevo_bloque = new BloqueHash(nuevo_tamanio_dispersion);
	unsigned int num_nuevo_bloque = extraer_nro_nuevo_bloque();
	nuevo_bloque->Persistir(this->nombre_arch_bloques.c_str(), num_nuevo_bloque*BloqueHash::getTamanioBloques());
	delete nuevo_bloque;

	//y asigna su referencia a todas las posiciones de la tabla
	//que estén a distancia n*nuevo_tamanio_dispersion
	vector<unsigned int> *tabla_disp = cargar_tabla_dispersion();
	unsigned int posicion = posicion_en_tabla;
	unsigned int distancia = 0;
	unsigned int tamanio_tabla = (*tabla_disp)[0];
	do{
		if(distancia%nuevo_tamanio_dispersion == 0) (*tabla_disp)[posicion] = num_nuevo_bloque;

		if(posicion + 1 > tamanio_tabla) posicion = 1;
		else posicion++;
		distancia++;
	}while(posicion != posicion_en_tabla);
	persistir_vector(tabla_disp, this->nombre_arch_tabla.c_str());

	//redispersa los registros del bloque desbordado
	RegistroIndice *registroEnLista = NULL;
	list<RegistroIndice *>::iterator it;
	for (it = registros_a_redispersar->begin(); it != registros_a_redispersar->end(); it++){
		registroEnLista = *it;
		guardar(registroEnLista);
	}
}

////////////////////////

void hash_extensible::expandir_hash(unsigned int posicion_en_tabla){

	unsigned int offset_bloque = obtener_offset_bloque(posicion_en_tabla);
	BloqueHash bloque_aux(0);
	BloqueHash *unBloque;
	unBloque = (BloqueHash*)bloque_aux.Leer(this->nombre_arch_bloques.c_str(), offset_bloque);
	unsigned int tamanio_dispersion = unBloque->getTamanioDispersion();
	delete unBloque;
	unsigned int tamanio_tabla = obtener_tamanio_tabla();

	if (tamanio_dispersion == tamanio_tabla) duplicar_tabla(posicion_en_tabla);
	else if (tamanio_dispersion < tamanio_tabla) incrementar_tabla(posicion_en_tabla);
}

///////////////////////////

void hash_extensible::guardar(RegistroIndice *registro){

	unsigned int offset_bloque = obtener_offset_bloque(funcion_hashing(registro));
	BloqueHash bloque_aux(0);
	BloqueHash *unBloque;
	unBloque = (BloqueHash*) bloque_aux.Leer(this->nombre_arch_bloques.c_str(), offset_bloque);

	/* trata de insertar el registro y salir */
	if(unBloque->Insertar(registro)){
		unBloque->Persistir(this->nombre_arch_bloques.c_str(), offset_bloque);
		delete unBloque;
		return;
	}
	delete unBloque;

	/* si el bloque estaba lleno y no se pudo insertar el registro,
	 * agrega otro bloque, modifica la tabla, redispersa, guarda y termina */
	expandir_hash(funcion_hashing(registro));
	guardar(registro);

	/* NOTA: para conservar la unicidad, bloque::insertar(registro) debe estár
	 * implementado de forma tal que si el registro ya estaba en el bloque,
	 * el viejo es reemplazado por el nuevo */
}

//////////////////////////

bool hash_extensible::borrar(RegistroIndice *registro){

	unsigned int offset_bloque = obtener_offset_bloque(funcion_hashing(registro));
	BloqueHash bloque_aux(0);
	BloqueHash *unBloque;
	unBloque = (BloqueHash*)bloque_aux.Leer(this->nombre_arch_bloques.c_str(), offset_bloque);

	if(unBloque != NULL){
		if(unBloque->Eliminar(registro)){
			unBloque->Persistir(this->nombre_arch_bloques.c_str(), offset_bloque);

			/** Si el borrado fue exitoso, luego revisa si el bloque quedó vacío para tratar de liberarlo */
			if(unBloque->getCantidadRegistros() == 0)
				reducir_hash(funcion_hashing(registro));

			delete unBloque;
			return true;
		}
		delete unBloque;
	}
	return false;
}

//////////////////////////

void hash_extensible::reducir_hash(unsigned int posicion_en_tabla){

	vector<unsigned int> *tabla = cargar_tabla_dispersion();
	unsigned int tamanio_tabla = (*tabla)[0];
	unsigned int nro_bloque_vacio = (*tabla)[posicion_en_tabla];

	/* calcula la distancia que hay que moverse a derecha e izquierda de
	 * posicion_en_tabla para ver si los números de bloque consignados en esas
	 * posiciones coinciden y determinar si el bloque puede o no ser liberado*/
	unsigned int offset_bloque = obtener_offset_bloque(posicion_en_tabla);
	BloqueHash bloque_aux(0);
	BloqueHash *bloque_vacio;;
	bloque_vacio = (BloqueHash*)bloque_aux.Leer(this->nombre_arch_bloques.c_str(), offset_bloque);

	unsigned int distancia_final = bloque_vacio->getTamanioDispersion()/2;
	delete bloque_vacio;
	/* caso particular: se vació completamente el hash:
	 * si el bloque que se intenta liberar es el único que hay (tiene td = 1),
	 * no se lo libera, y el resto de los archivos vuelven a cómo eran
	 * inmediatamente después de llamar al constructor
	 */
	if(distancia_final == 0){
        //deja la tabla con una sola referencia al bloque que quedó
        vector<unsigned int> *tabla_disp = cargar_tabla_dispersion();
        unsigned int tamanio_tabla = 1;
        (*tabla_disp)[0] = tamanio_tabla;
        persistir_vector(tabla_disp, this->nombre_arch_tabla);

        //en el archivo de bloques libres pone todos los nros. de bloque
        //menores al nro. de bloque que quedó en uso
        unsigned int num_bloque_en_uso = (*tabla_disp)[1];
        delete tabla_disp;

        vector<unsigned int> *bloques_libres = new vector<unsigned int>;
        bloques_libres->reserve(num_bloque_en_uso + 1);
        (*bloques_libres)[0] = num_bloque_en_uso;
        for(unsigned int i = 1; i < num_bloque_en_uso +1; i++){
            (*bloques_libres)[i] = i - 1;
        }
        persistir_vector(bloques_libres, this->nombre_arch_bloques_libres);
        delete bloques_libres;

        //antes de salir libera memoria que se pidió al principio del método
        delete tabla;
        return;
	}

	unsigned int posicion_adelante = posicion_en_tabla;
	unsigned int posicion_atras = posicion_en_tabla;
	unsigned int distancia_actual = 0;

	do{
		if(posicion_adelante + 1 > tamanio_tabla) posicion_adelante = 1;
		else posicion_adelante++;
		distancia_actual++;
	}while(distancia_actual < distancia_final);

	distancia_actual = 0;
	do{
		if(posicion_atras - 1 < 1) posicion_atras = tamanio_tabla;
		else posicion_atras--;
		distancia_actual++;
	}while(distancia_actual < distancia_final);

	/* si coinciden recorre la tabla de dispersión y en cada posición a distancia
	 * n*distancia_final del bloque vacío asigna el número de bloque que lo reemplaza */
	if((*tabla)[posicion_atras] == (*tabla)[posicion_adelante]){
		unsigned int bloque_a_asignar = (*tabla)[posicion_atras];
		unsigned int distancia = 0;
		unsigned int posicion = posicion_en_tabla;
		do{
			if(distancia%distancia_final == 0) (*tabla)[posicion] = bloque_a_asignar;

			if(posicion + 1 > tamanio_tabla) posicion = 1;
			else posicion++;
			distancia++;
		}while(posicion != posicion_en_tabla);

		persistir_vector(tabla, this->nombre_arch_tabla.c_str());

		//libera el bloque vacío (agrega al archivo de bloques libres)
		vector<unsigned int> *lista_bloques_libres = cargar_archivo_bloques_libres();
		((*lista_bloques_libres)[0])++;
		lista_bloques_libres->reserve((*lista_bloques_libres)[0] + 1);
		(*lista_bloques_libres)[(*lista_bloques_libres)[0]] = nro_bloque_vacio;
		persistir_vector(lista_bloques_libres, this->nombre_arch_bloques_libres.c_str());
		delete lista_bloques_libres;

		//al bloque reemplazante le divide por 2 el tamaño de dispersion
		unsigned int offset_reemplazante = obtener_offset_bloque(posicion_en_tabla);
		BloqueHash bloque_aux(0);
		BloqueHash *bloque_reemplazante;
		bloque_reemplazante = (BloqueHash*)bloque_aux.Leer(this->nombre_arch_bloques.c_str(), offset_reemplazante);
		unsigned int nuevo_tam_dispersion = bloque_reemplazante->getTamanioDispersion()/2;
		bloque_reemplazante->setTamanioDispersion(nuevo_tam_dispersion);
		bloque_reemplazante->Persistir(this->nombre_arch_bloques.c_str(), offset_reemplazante);
		delete bloque_reemplazante;

		//si las dos mitades son iguales trunca la tabla
		bool distintas = false;
		for(unsigned int i = 1; i <= tamanio_tabla/2; i++){
			if((*tabla)[i + tamanio_tabla/2] != (*tabla)[i])
				distintas = true;
		}
		if (!distintas){
			vector<unsigned int> *aux = new vector<unsigned int>;
			aux->reserve(tamanio_tabla/2 +1);
			(*aux)[0] = tamanio_tabla/2;
			for(unsigned int i = 1; i <= tamanio_tabla/2; i++)
				(*aux)[i] = (*tabla)[i];

			persistir_vector(aux, this->nombre_arch_tabla.c_str());
			delete aux;
		}
	}
	delete tabla;
}

////////////////////////////

void hash_extensible::imprimir(const string nombre_archivo){

	vector<unsigned int> *tabla = cargar_tabla_dispersion();
	vector<unsigned int> *lista_bloqlib = cargar_archivo_bloques_libres();
	unsigned int tamanio_tabla = (*tabla)[0];
	unsigned int tamanio_lista_libres = (*lista_bloqlib)[0];

	fstream archivo_texto;
	archivo_texto.open(nombre_archivo.c_str(), ios::out | ios::trunc);
	archivo_texto << "TD: tamaño de dispersion. EL: espacio libre" << endl << endl << endl;
	archivo_texto << "Tabla:" << endl;
	for(unsigned int i = 1; i <= tamanio_tabla; i++)
		archivo_texto << "  " << (*tabla)[i];
	archivo_texto << endl;
	archivo_texto << "Bloques libres:" << endl;
	for(unsigned int i = 1; i <= tamanio_lista_libres; i++)
		archivo_texto << "  " << (*lista_bloqlib)[i];
	archivo_texto << endl << endl;

	archivo_texto << "Bloques:" << endl;
	int max = -1;
	for(unsigned int i = 1; i <= (*tabla)[0]; i++)
		if((int)(*tabla)[i] > max) max = (*tabla)[i];

	unsigned int offset_bloque = 0;
	BloqueHash bloque_aux(0);
	BloqueHash *unBloque;
	unsigned int num_bloque;
	bool liberado = false;
	while(offset_bloque <= max*BloqueHash::getTamanioBloques()){
		unBloque = (BloqueHash*) bloque_aux.Leer(this->nombre_arch_bloques.c_str(), offset_bloque);
		num_bloque = offset_bloque/BloqueHash::getTamanioBloques();
		//antes de imprimir revisa que el bloque no esté liberado
		unsigned int i = 1;
		while((i <= tamanio_lista_libres) && (!liberado)){
			if((*lista_bloqlib)[i] == num_bloque) liberado = true;
			i++;
		}
		if(liberado){
			offset_bloque += BloqueHash::getTamanioBloques();
			liberado = false;
			delete unBloque;
			continue;
		}
		//imprime el contenido del bloque
		archivo_texto << num_bloque << ": ";
		unBloque->Imprimir(&archivo_texto);
		archivo_texto << endl;

		offset_bloque += BloqueHash::getTamanioBloques();
		delete unBloque;
	}
	archivo_texto.close();
	delete tabla;
	delete lista_bloqlib;
}

///////////////////////////

vector<unsigned int> hash_extensible::listar(){

    //carga la tabla de dispersión y determina cuál es el número
    //de bloque en uso más alto
    vector<unsigned int> *tabla = cargar_tabla_dispersion();

    int max = -1;
    for(unsigned int i = 1; i <= (*tabla)[0]; i++)
    	if((int)(*tabla)[i] > max) max = (*tabla)[i];

    //declaraciones de cosas que uso adentro del while
    unsigned int offset_bloque = 0;
    BloqueHash bloque_aux(0);
    BloqueHash *unBloque;
    unsigned int num_bloque;
    bool liberado = false;
    vector<unsigned int> vec_aux;
    vector<unsigned int> *vec_final = new vector<unsigned int>;
    vector<unsigned int> *lista_bloqlib = cargar_archivo_bloques_libres();
    unsigned int tamanio_lista_libres = (*lista_bloqlib)[0];
    
    //mietras no llegue al final del archivo de bloques
    while(offset_bloque <= max*BloqueHash::getTamanioBloques()){
        //lee un bloque
        unBloque = (BloqueHash*) bloque_aux.Leer(this->nombre_arch_bloques.c_str(), offset_bloque);
        num_bloque = offset_bloque/BloqueHash::getTamanioBloques();
        //revisa que el bloque no esté liberado
        unsigned int i = 1;
        while((i <= tamanio_lista_libres) && (!liberado)){
                if((*lista_bloqlib)[i] == num_bloque) liberado = true;
                i++;
        }
        if(liberado){
                offset_bloque += BloqueHash::getTamanioBloques();
                liberado = false;
                delete unBloque;
                continue;
        }

        //si no estaba liberado, lista los offsets del bloque y los agrega al vector final
        vec_aux = unBloque->Listar();
        vec_final->insert(vec_final->end(), vec_aux.begin(), vec_aux.end());

        //incrementa contadores y limpia variables antes del próximo ciclo
        offset_bloque += BloqueHash::getTamanioBloques();
        vec_aux.clear();
        delete unBloque;
    }

    delete tabla;
    delete lista_bloqlib;

    //ordena el vector y lo devuelve
    sort(vec_final->begin(), vec_final->end());
    return *vec_final;
}