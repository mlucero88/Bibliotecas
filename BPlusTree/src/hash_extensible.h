#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include "RegistroIndice.h"
#include "BloqueHash.h"

using namespace std;

#ifndef HASH_EXTENSIBLE_H_
#define HASH_EXTENSIBLE_H_

/// Hash con dispersión extensible modular.
/**
 * Este hash extensible está preparado para contener objetos de la clase RegistroIndice.
 * Los datos son almacenados en disco. Para funcionar requiere de tres archivos:
 *
 * - Bloques: es donde se guardan los bloques con los registros.
 * - Tabla de Dispersion: la tabla utilizada para direccionar al archivo de bloques.
 * - Bloques Libres: guarda los números de bloques que fueron liberados, para usarlos
 *   cuando haya que agregar un bloque nuevo.
 */
class hash_extensible {
	string nombre_arch_bloques;
	string nombre_arch_bloques_libres;
	string nombre_arch_tabla;

	//recibe una posicion (no un offset) en la tabla de dispersión y devuelve
	//el offset del bloque correspondiente en el archivo de hashing
	unsigned int obtener_offset_bloque(unsigned int posicion_en_tabla);

	//Pre: se produjo un desborde en un bloque con td = tt (posicion_en_tabla es la posición
	//del archivo "tabla dispersión" en el que está el nro. del bloque que se desbordó)
	//Post: se duplica el tamaño de la tabla, se agrega un nuevo bloque, y se redispersan
	//los registros del bloque desbordado.
	void duplicar_tabla(unsigned int posicion_en_tabla);

	//Pre: se produjo un desborde en un bloque con td < tt
	//Post: se agrega un nuevo bloque, se reestructura la tabla, y se redispersan
	//los registros del bloque desbordado.
	void incrementar_tabla(unsigned int posicion_en_tabla);

	//Pre: se produjo un desborde en un bloque
	//Post: se agrego un bloque, duplicando o no la tabla, según corresponda
	void expandir_hash(unsigned int posicion_en_tabla);

	//Pre: se vació un bloque (posicion_en_tabla es la posición del archivo "tabla dispersión"
	//en el que está el nro. del bloque que quedó vacío)
	//Post: se intenta liberar el bloque vacío y truncar la tabla.
	void reducir_hash(unsigned int posicion_en_tabla);

	//Pre: se necesita crear un nuevo bloque
	//Post: devuelve el número del nuevo bloque y lo elimina del
	//archivo de bloques libres
	unsigned int extraer_nro_nuevo_bloque();

	unsigned int funcion_hashing(RegistroIndice *registro);

	void persistir_vector(vector<unsigned int> *vec, string nombre_archivo);

	unsigned int obtener_tamanio_tabla();

public:
	/// Crea e inicializa los archivos de bloques, de bloques libres y de tabla de dispersión.
	hash_extensible(string nombre_arch_bloques, string nombre_arch_bloques_libres, string nombre_arch_tabla);

	/// Pre: el hash fue creado. Post: se almacena el registro de índice.
	void guardar(RegistroIndice *registro);

	/// Pre: el hash fue creado. Post: se elimina el registro de índice, y su información es devuelta por parámetro.
	/**Si no encuentra el registro, devuelve false */
	bool borrar(RegistroIndice *registro);

	/// Pre: el hash fue creado. Post: devuelve la información del registro de índice (sin modificarlo).
	/**Si no encuentra el registro devuelve NULL */
	RegistroIndice* buscar(RegistroIndice *registro);

	/// Crea un archivo de texto plano en el que se muestra la estructura del hash
	void imprimir(const string nombre_archivo);

        /// Devuelve un vector ordenado con los offsets de todos los registros que hay en el hash
        vector<unsigned int> listar();

	/// Carga en memoria la tabla de dispersión.
	/**En la primera posición del vector se almacena la cantidad de posiciones de la tabla. */
	vector<unsigned int>* cargar_tabla_dispersion();

	/// Carga en memoria el archivo de bloques libres.
	/**En la primera posición se almacena la cantidad de bloques libres. */
	vector<unsigned int>* cargar_archivo_bloques_libres();
};

#endif /* HASH_EXTENSIBLE_H_ */
