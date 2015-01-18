#ifndef THREAD_H
#define	THREAD_H

#include <pthread.h>

namespace POSIX {

/**
 * @brief Clase que encapsula e implementa los métodos básicos de un POSIX
 * thread
 * @details Estados de un hilo:
 * @details EN EJECUCION: el hilo está ejecutandose, haciendo uso del
 * procesador
 * @details EN ESPERA: el hilo tiene instrucciones por ejecutar, pero se
 * encuentra en espera por el uso del procesador
 * @details FINALIZADO: el hilo no tiene instrucciones por ejecutar, pero sigue
 * en posesión de los recursos que le fueron concedidos (id). Este es el caso
 * de los hilos de tipo JOINABLE, que deben ser liberados por
 * Thread::esperarThread
 * @details VIVO: este estado incluye los anteriores, es decir, implica que
 * está o en ejecución o en espera o finalizado. Se utiliza para indicar que
 * posee un id válido y no se puede volver a iniciar hasta que pase a estado
 * MUERTO.
 * @details MUERTO: este estado es el opuesto excluyente a VIVO. Implica que el
 * hilo o nunca fue iniciado o ya finalizó su ejecución y sus recursos fueron
 * liberados. Indica que el id del hilo se encuentra con valor 0 y puede volver
 * a iniciarse.
 */
class Thread {

public:

	class Atributos;

	typedef pthread_t thread_id;

	/**
	 * @brief Método que inicia el hilo con los atributos actuales que posee
	 * @pre El Thread debe estar en estado MUERTO
	 * @post Si se inicia exitosamente, el hilo es puesto en ejecución y
	 * llama al método Thread::ejecutar, pasándole a dicho método el parámetro
	 * @a parametro
	 * @post El hilo pasa al estado VIVO y se le asigna un id válido
	 * @param parametro Puntero a algún tipo de dato que se le quiera pasar
	 * al hilo iniciado
	 * @throw MultiHiloExcepcion con código:
	 * @throw THREAD_EN_USO El Thread ya estaba VIVO
	 * @throw SIN_RECURSOS El sistema no posee recursos para iniciar el Thread
	 * @throw ATTR_INVALIDOS Los atributos del Thread son inválidos
	 */
	void iniciar(void *parametro = NULL) /* throw (MultiHiloExcepcion) */;

	/**
	 * @brief Método que espera la finalización del Thread @a aEsperar y libera
	 * los recursos que consumió
	 * @details El hilo en ejecución (invocante) se bloquea hasta que el Thread
	 * @a aEsperar finalice su ejecución, luego continua con la suya.
	 * El puntero @a retorno se modifica para apuntar a la información que fue
	 * retornada por la finalizacion del Thread
	 * @pre El Thread @a aEsperar debe ser de tipo JOINABLE, no debe ser
	 * sí mismo (el hilo actual) y debe estar en estado VIVO
	 * @post Libera los recursos, dejando al Thread en estado MUERTO
	 * @param aEsperar Thread a esperar
	 * @param retorno Valor de retorno del Thread @a aEsperar
	 * @throw MultiHiloExcepcion con código:
	 * @throw THREAD_NO_JOINABLE El Thread @a aEsperar no es de tipo JOINABLE
	 * @throw JOIN_SELF_ERROR El Thread @a aEsperar es el hilo en ejecución
	 * @throw THREAD_ID_INVALIDO El Thread @a aEsperar no está VIVO
	 */
	static void esperarThread(Thread &aEsperar,
			void* &retorno) /* throw (MultiHiloExcepcion) */;

	/**
	 * @brief Método que manda un pedido de cancelación de ejecución del Thread
	 * @a aCancelar
	 * @pre El Thread @a aCancelar no debe ser sí mismo (el hilo actual) y
	 * debe estar en estado VIVO
	 * @post Deja al hilo en estado MUERTO
	 * @param aCancelar Thread a cancelar
	 * @throw MultiHiloExcepcion con código:
	 * @throw CANCEL_SELF_ERROR El Thread @a aCancelar es el hilo en ejecución
	 * @throw THREAD_ID_INVALIDO:El Thread @a aCancelar no está VIVO
	 */
	static void cancelarEjecucionThread(
			Thread &aCancelar) /* throw (MultiHiloExcepcion) */;

	/**
	 * @brief Método que setea los Atributos @a atributos al Thread
	 * @pre El Thread debe estar en estado MUERTO
	 * @post Se modifican los atributos del Thread
	 * @param atributos Atributos a setearle
	 * @throw MultiHiloExcepcion con código:
	 * @throw THREAD_EN_USO El Thread está en estado VIVO
	 */
	void setAtributos(
			const Atributos &atributos) /* throw (MultihiloExcepcion) */;

	/**
	 * @brief Método que retorna los Atributos del Thread, solo para consulta
	 * @return Los atributos del thread
	 */
	const Atributos& getAtributos() const;

	/**
	 * @brief Método que retorna el id del Thread
	 * @return El id del Thread. Un valor 0 indica que se encuentra en estado
	 * MUERTO
	 */
	thread_id getId() const;

	/**
	 * @brief Método que retorna el id del hilo en ejecución
	 * @return El id del hilo en ejecución
	 */
	static thread_id getIdEnEjecucion();

	/**
	 * @brief Método para consultar si el Thread se encuentra en estado
	 * EN EJECUCION
	 * @return <tt>true</tt> El Thread se encuentra en estado EN EJECUCION
	 * @return <tt>false</tt> El Thread se encuentra en estado EN ESPERA,
	 * FINALIZADO o MUERTO
	 */
	bool enEjecucion() const;

	/**
	 * @brief Método para consultar si el Thread está en estado VIVO
	 * @return <tt>true</tt> El Thread está en estado VIVO
	 * @return <tt>false</tt> El Thread está en estado MUERTO
	 */
	bool estaVivo() const;

	/**
	 * @brief Método que setea al Thread como DETACHED
	 * @pre El Thread debe estar en estado MUERTO o estar en estado VIVO y ser
	 * de tipo JOINABLE
	 * @post Se modifican los atributos del Thread para que sea lanzado en modo
	 * DETACHED (si se encontraba MUERTO), o se cambia dinámicamente su modo a
	 * DETACHED (si se encontraba VIVO)
	 * @throw MultiHiloExcepcion con código:
	 * @throw THREAD_NO_JOINABLE El Thread no es de tipo JOINABLE
	 * @throw THREAD_ID_INVALIDO No se pudo encontrar al hilo con dicho id
	 */
	void setDetached() /* throw (MultiHiloExcepcion) */;

	/**
	 * @brief Método que setea al Thread como JOINABLE
	 * @pre El Thread debe estar en estado MUERTO
	 * @post Se modifican los atributos del Thread para que sea lanzado en modo
	 * JOINABLE
	 * @throw MultiHiloExcepcion con código:
	 * @throw THREAD_EN_USO El Thread está en estado VIVO
	 */
	void setJoinable() /* throw (MultiHiloExcepcion) */;

	/**
	 * @brief Operador comparador igual
	 * @param aComparar Thread a comparar
	 * @return <tt>true</tt> > Ambas instancias manejan el mismo hilo
	 */
	bool operator==(const Thread &aComparar);

	/**
	 * @brief Operador comparador desigual
	 * @param aComparar Thread a comparar
	 * @return <tt>true</tt> Ambas instancias no manejan el mismo hilo
	 */
	bool operator!=(const Thread &aComparar);

	/**
	 * @brief Destructor
	 */
	virtual ~Thread();

	/**
	 * @brief Clase para manejar la unión que define los atributos de un hilo
	 * de forma mas fácil, definiendo métodos más intuitivos
	 */
	class Atributos {
	public:

		/**
		 * @brief Construye una instancia con atributos por defecto y categoría
		 * JOINABLE
		 */
		Atributos();

		/**
		 * @brief Constructor copia
		 * @param aCopiar Atributos a copiar
		 */
		Atributos(const Atributos &aCopiar);

		/**
		 * @brief Operador asignación
		 * @param aAsignar Atributos a copiar
		 * @return Referencia a la instancia invocante del método
		 */
		Atributos& operator=(const Atributos &aAsignar);

		/**
		 * @brief Setea los atributos para que sea un hilo de categoría
		 * DETACHED
		 */
		void setDetached();

		/**
		 * @brief Setea los atributos para que sea un hilo de categoría
		 * JOINABLE
		 */
		void setJoinable();

		/**
		 * @brief Método para consultar la categoría de los atributos
		 * @return <tt>true</tt> si es de categoría DETACHED
		 */
		bool isDetached() const;

		/**
		 * @brief Método para consultar la categoría de los atributos
		 * @return <tt>true</tt> si es de categoría JOINABLE
		 */
		bool isJoinable() const;

		/**
		 * @brief Destructor
		 */
		virtual ~Atributos();

	private:

		pthread_attr_t atributos;
		friend void Thread::iniciar(void*);
	};

protected:

	/**
	 * @brief Constructor sin parámetros
	 * @post Deja al objeto Thread instanciado, pero no lo ejecuta. Su id se
	 * instancia con valor 0 (valor inválido: su id correcto se le asigna al
	 * llamar al método Thread::iniciar) y su estado es MUERTO. Los atributos
	 * del hilo son los atributos creados por Thread::Atributos::Atributos
	 */
	Thread();

	/**
	 * @brief Constructor con asignación de atributos
	 * @post Deja al objeto Thread instanciado, pero no lo ejecuta. Su id se
	 * instancia con valor 0 (valor invalido: su id correcto se le asigna al
	 * llamar al método Thread::iniciar) y su estado es MUERTO. Los atributos
	 * del hilo se inicializan con los atributos de @a atributosThread
	 * @param atributos Atributos a setearle al hilo
	 */
	explicit Thread(const Atributos &atributos);

	/**
	 * @brief Método que se ejecuta al llamar a Thread::iniciar, el cual es el
	 * único método que puede y debe invocarlo. Se redefine para darle el
	 * comportamiento deseado al hilo al iniciarlo.
	 * @warning No invocar
	 * @param parametro Parámetro que se le pasa a Thread::iniciar
	 */
	virtual void* ejecutar(void* parametro) = 0;

private:

	void *parametro;
	Atributos atributos;
	thread_id id;
	bool esta_vivo;

	/* Metodo estatico que sirve para lanzar un thread. Es la conexion
	 * entre el metodo "iniciar" y "ejecutar", ya que pthread_create
	 * necesita una funcion de C como argumento */
	static void* lanzador(void *objetoThread);
};
}

#endif
