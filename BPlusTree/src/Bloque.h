#ifndef BLOQUE_H_
#define BLOQUE_H_

using namespace std;

///Clase abstracta.
class Bloque{
protected:
	///Atributo común a todos los bloques.
	unsigned int tamanio;
	///Atributo común a todos los bloques.
	unsigned int espacioLibre;

public:
	///Devuelve la cantidad de registros en el bloque.
	virtual int getCantidadRegistros()=0;

	///Guarda el bloque junto con sus metadatos en un archivo binario en la ruta indicada.
	virtual bool Persistir(string rutaArchivo, unsigned int offset)=0;

	///Levanta el bloque a memoria.
	virtual Bloque* Leer(string rutaArchivo, unsigned int offset)=0;
};

#endif /* BLOQUE_H_ */
