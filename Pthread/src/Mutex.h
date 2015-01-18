#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>

namespace POSIX {

/**
 * @brief Clase para manejar los mutex de POSIX threads. Tiene una interfaz muy
 * simple, con los métodos básicos y solo se manejan mutex creados con los
 * atributos por defecto
 */

class Mutex {
public:

	/**
	 * @brief Instancia un mutex con los atributos por defecto
	 */
	Mutex();


	/**
	 * @brief Método para bloquear el mutex e impedir a otros threads acceder a
	 * código salvaguardado por el mismo mutex hasta que sea liberado
	 * @pre El hilo actual no está en posesión del mutex (no lo bloqueó)
	 * @post Si el mutex se encuentra liberado, el hilo actual se apropia del
	 * mismo y lo bloquea. Si ya se encontraba bloqueado, el hilo queda a la
	 * espera de que se le pueda asignar el mutex
	 * @return <tt>true</tt> si no hay errores al intentar tomar el mutex
	 * @return <tt>false</tt> si el hilo ya está en posesión del mutex antes
	 * del llamado al método
	 */
	bool bloquear();

	/**
	 * @brief Método para intentar bloquear el mutex e impedir a otros threads
	 * acceder a código salvaguardado por el mismo mutex hasta que sea liberado
	 * @post Si el mutex se encuentra liberado, el hilo actual se apropia del
	 * mismo y lo bloquea. Si ya se encontraba bloqueado, se retorna false y
	 * el hilo actual sigue su ejecución
	 * @return <tt>true</tt> si se logra bloquear el mutex
	 * @return <tt>false</tt> si el mutex se encontraba ya bloqueado
	 */
	bool intentarBloquear();

	/**
	 * @brief Método para liberar el mutex por parte del hilo en ejecución
	 * @pre El hilo actual debe estar en posesión del mutex
	 * @post Si el mutex se encuentra bloqueado por el hilo en ejecución, se
	 * libera
	 * @return <tt>true</tt> si se libera mutex
	 * @return <tt>false</tt> si el hilo no estaba en posesión del mutex o el
	 * mutex ya estaba liberado
	 */
	bool desbloquear();

	/**
	 * @brief Hace esperar al thread en ejecución hasta que reciba la señal
	 * enviada por Mutex::signal
	 * @post El thread en ejecución se bloquea hasta recibir la señal de
	 * desbloqueo
	 */
	void wait();

	/**
	 * @brief Método que envía una señal al los threads que están bloqueados
	 * por el método Mutex::wait, avisándoles que se desbloqueen
	 * @post Los threads que se encuentran en espera por el método Mutex::wait
	 * se desbloquean y continuan su ejecución
	 */
	void signal();

	/**
	 * @brief Libera los recursos
	 * @pre Mutex no se encuentra bloqueado
	 */
	~Mutex();

private:

	pthread_mutex_t mutex;
	pthread_cond_t condVar;
};
}

#endif
