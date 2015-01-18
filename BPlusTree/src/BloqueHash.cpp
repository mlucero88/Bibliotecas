#include <vector>

#include "BloqueHash.h"

BloqueHash::BloqueHash(unsigned int tamanioDispersion){
	this->tamanioDispersion = tamanioDispersion;
	this->tamanio = TAM_BLOQUE_HASH;
	this->espacioLibre = this->tamanio - sizeof(this->espacioLibre) - sizeof(this->tamanioDispersion);
}

///////////////////////

BloqueHash::~BloqueHash(){
	while(!this->registros.empty())
		this->registros.pop_back();
}
/////////////////////////

int BloqueHash::getTamanioDispersion(){
	return this->tamanioDispersion;
}

/////////////////////////

int BloqueHash::getTamanioBloques(){
	return TAM_BLOQUE_HASH;
}

//////////////////////////

int BloqueHash::getCantidadRegistros(){
	return this->registros.size();
}

/////////////////////////

void BloqueHash::setTamanioDispersion(int tamanioDispersion){
	this->tamanioDispersion = tamanioDispersion;
}

///////////////////////////

RegistroIndice* BloqueHash::Buscar(RegistroIndice *registro){
	//- recorrer la lista de registros del bloque
	//- comparar la clave de "registro" con las claves de
	//los registros de la lista
	RegistroIndice *registroEnLista = NULL;
	list<RegistroIndice *>::iterator it;
	for (it = this->registros.begin(); it != this->registros.end(); it++){
		registroEnLista = *it;

		//- si coinciden las claves, crear registro aux con clave y dato
		//correspondiente, devolver y salir
		if(registroEnLista->getClave() == registro->getClave()){
			return registroEnLista;
		}
	}
	//- devolver NULL si no se encuentra.
	return NULL;
}

//////////////////////////

bool BloqueHash::Insertar(RegistroIndice *registro){
	/* Validación: si el registro ya estaba en la lista, lo sobreescribe
	 * (lo elimina, actualizando espacioLibre, para luego insertar el nuevo)
	 */
	this->Eliminar(registro);
	/* Fin de la validación
	 */

	//- revisa cuál sería el tamaño del registro ya persistido y lo
	//compara con espacioLibre para ver que si la inserción es posible
	if(registro->getTamanioEnDisco() <= this->espacioLibre){
	//- si entra, lo agrega a la lista de registros del bloque, actualiza el espacio libre y devuelve true
	//- si no, devuelve false sin modificar el bloque (desborde)
		this->registros.push_back(registro);
		this->espacioLibre -= registro->getTamanioEnDisco();
		return true;
	}
	return false;
}

//////////////////////////

bool BloqueHash::Eliminar(RegistroIndice *registro){
	//- recorrer la lista de registros del bloque hasta
	//encontrar "registro".
	//- borrarlo de la lista, actualizar espacioLibre y devolver true
	//- si no estaba, devolver false

	RegistroIndice *registroEnLista = NULL;
	list<RegistroIndice *>::iterator it;
	for (it = this->registros.begin(); it != this->registros.end(); it++){
		registroEnLista = *it;
		if(registroEnLista->getClave() == registro->getClave()){
			registro->setOffset(registroEnLista->getOffset());
			this->espacioLibre += registroEnLista->getTamanioEnDisco();
			this->registros.erase(it);
			return true;
		}
	}
	return false;
}

////////////////////////

bool BloqueHash::Persistir(string rutaArchivo, unsigned int offset){
	//graba el bloque en el archivo.
	//Si no pudo grabar, devuelve false

	fstream archBloques(rutaArchivo.c_str(), ios_base::out | ios_base::in | ios_base::binary);
	if(!archBloques.is_open()){
        archBloques.open(rutaArchivo.c_str(), ios_base::out);
        archBloques.close();
        archBloques.open(rutaArchivo.c_str(), ios_base::out | ios_base::in | ios_base::binary);
	}else if(!archBloques.is_open()) return false;

	archBloques.seekp(offset);
	unsigned int tamDispersion = this->getTamanioDispersion();
	archBloques.write((char*)&tamDispersion, sizeof(unsigned int));
	unsigned int espLibre = this->espacioLibre;
	archBloques.write((char*)&espLibre, sizeof(unsigned int));

	RegistroIndice *registro = NULL;
	list<RegistroIndice *>::iterator it;
	for (it = this->registros.begin(); it != this->registros.end(); it++){
		registro = *it;
		registro->Persistir(&archBloques);
		//delete registro;
	}
    archBloques.flush();
	archBloques.close();
	return true;
}
////////////////////

Bloque* BloqueHash::Leer(string rutaArchivo, unsigned int offset){

	//levanta los datos a memoria
	fstream archBloques(rutaArchivo.c_str(), ios_base::in | ios_base::binary);
    if(!archBloques.is_open()) return NULL;

	archBloques.seekg(offset);
	unsigned int tamDispersion;
	archBloques.read((char*)&tamDispersion, sizeof(tamDispersion));
	BloqueHash *BloqueAux = new BloqueHash(tamDispersion);

	unsigned int espLibre;
	archBloques.read((char*)&espLibre, sizeof(espLibre));
	BloqueAux->espacioLibre = espLibre;

	//mientras no llegue al final del chorizo de bytes, lee los registros
	RegistroIndice *registro = NULL;
	while(archBloques.tellg() != offset + BloqueAux->tamanio - BloqueAux->espacioLibre){
		registro = registro->Leer(&archBloques);
		BloqueAux->registros.push_back(registro);
	}
	archBloques.close();
	return BloqueAux;
}

///////////////////////

list<RegistroIndice *>* BloqueHash::VaciarBloque(){

	//- devolver los registros del bloque y vaciar
	// la lista original
	//- actualizar el espacio libre del bloque (siempre en memoria)

	list<RegistroIndice *> *aux = new list<RegistroIndice *>;
	RegistroIndice *registroEnLista = NULL;
	list<RegistroIndice *>::iterator it;

	for (it = this->registros.begin(); it != this->registros.end(); it++){
		registroEnLista = *it;
		aux->push_back(registroEnLista);
		this->espacioLibre += registroEnLista->getTamanioEnDisco();
	}
	this->registros.clear();

	return aux;
}

/////////////////////

void BloqueHash::Imprimir(fstream *archImpresion){

    *archImpresion << "TD: " << this->getTamanioDispersion() << " ";
    *archImpresion << "EL: " << this->espacioLibre << endl;
    *archImpresion << "Registros: ";

    RegistroIndice *registroEnLista = NULL;
    list<RegistroIndice *>::iterator it;
    for (it = this->registros.begin(); it != this->registros.end(); it++){
    	registroEnLista = *it;
        registroEnLista->Imprimir(archImpresion);
	*archImpresion << " ";
    }

    archImpresion->flush();
}

///////////////////////

vector<unsigned int> BloqueHash::Listar(){

    vector<unsigned int> *offsets = new vector<unsigned int>;

    RegistroIndice *registroEnLista = NULL;
    list<RegistroIndice *>::iterator it;
    for (it = this->registros.begin(); it != this->registros.end(); it++){
    	registroEnLista = *it;
        offsets->push_back(registroEnLista->getOffset());
    }

    return *offsets;
}