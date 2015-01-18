#ifndef BLOQUEHASH_H_
#define BLOQUEHASH_H_

#include <iostream>
#include <fstream>
#include <list>
#include "Bloque.h"
#include "RegistroIndice.h"

#define TAM_BLOQUE_HASH 512 //Bytes

using namespace std;

///Bloques utilizados en el hash extensible.
/**Hereda publicamente de la clase abstracta Bloque. */
/**Su tamaño se define en el encabezado BloqueHash.h y están preparados para guardar objetos de la clase RegistroIndice. */
class BloqueHash: public Bloque{
private:
	unsigned int tamanioDispersion;
	list<RegistroIndice *> registros;

public:
	///Crea un bloque sin registros.
	BloqueHash(unsigned int tamanioDispersion);

	///Libera la memoria reservada para almacenar los registros del bloque
	~BloqueHash();

	///Devuelve el tamaño que tienen todos los bloques de la clase BloqueHash.
	static int getTamanioBloques();

	///Devuelve el tamaño de dispersión del bloque.
	int getTamanioDispersion();


	int getCantidadRegistros();

	///Establece el tamaño de dispersión del bloque.
	void setTamanioDispersion(int tamanioDispersion);

	/// Pre: el bloque fue creado. Post: devuelve la información del registro de índice.
	/**Si no encuentra el registro devuelve NULL */
	RegistroIndice* Buscar(RegistroIndice *registro);

	/// Pre: el bloque fue creado. Post: se almacena el registro de índice.
	/**Si el registro no entra en el bloque, devuelve false (se produce un desborde). */
	bool Insertar(RegistroIndice *registro);

	/// Pre: el bloque fue creado. Post: se elimina el registro de índice, y su información es devuelta por parámetro.
	/**Si no encuentra el registro, devuelve false */
	bool Eliminar(RegistroIndice *registro);

	///Pre: el bloque fue creado. Post: extrae la lista de registros del bloque devuelve un puntero a una copia de ella.
	/**Al igual que el resto de los métodos, se encarga de actualizar el espacio libre del bloque luego de vaciarlo. */
	list<RegistroIndice *>* VaciarBloque();


	bool Persistir(string rutaArchivo, unsigned int offset);


	/**El puntero obtenido debe ser casteado a un puntero a BloqueHash para poder tratarlo como tal. */
	Bloque* Leer(string rutaArchivo, unsigned int offset);

	/// Crea un archivo de texto plano en el que se muestra la estructura del bloque.
	/**Debe recibir por parámetro un archivo ya abierto y con su puntero de escritura posicionado en donde se
	 * quiera escribir la información del bloque.
	 */
	void Imprimir(fstream *archImpresion);

        /// Devuelve un vector con los offsets de todos los registros que hay en el bloque
        vector<unsigned int> Listar();
};

#endif /* BLOQUEHASH_H_ */
