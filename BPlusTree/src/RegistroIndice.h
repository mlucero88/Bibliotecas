#ifndef REGISTROINDICE_H_
#define REGISTROINDICE_H_

#include <iostream>
#include <fstream>

using namespace std;

///Registros utilizados por los índices implementados con el árbol B+ y el hash extensible.
class RegistroIndice{
protected:
	string clave;
	unsigned int offset;

public:

	///Crea un nuevo registro asignandole clave y offset.
	RegistroIndice(string clave, unsigned int offset);

	///Devuelve la clave del registro.
	string getClave();

	///Establece la clave del registro.
	void setClave(string clave);

	///Devuelve el offset del registro.
	unsigned int getOffset();

	///Establece el offset del registro.
	void setOffset(unsigned int offset);

	///Devuelve el tamaño (en bytes) que tendría el registro una vez persistido.
	virtual unsigned int getTamanioEnDisco();

	///Guarda el bloque junto con sus metadatos en un archivo.
	/**Debe recibir por parámetro un archivo ya abierto y con su puntero de escritura posicionado en donde
	 * se quiera persistir el registro.
	 */
	void Persistir(fstream *archivo);

	///Levanta el registro a memoria.
	RegistroIndice* Leer(fstream *archivo);

	///Crea un archivo de texto plano en el que se muestra la información del registro.
	/**Debe recibir por parámetro un archivo ya abierto y con su puntero de escritura posicionado en donde se
	 * quiera escribir el registro.
	 */
	void Imprimir(fstream *archImpresion);
};

#endif /* REGISTROINDICE_H_ */
