#ifndef MULTIHILOEXCEPCION_H_
#define MULTIHILOEXCEPCION_H_

#include <exception>
#include <string>

namespace POSIX {

/**
 * @brief Clase que define las excepciones generadas Thread
 */
class MultiHiloExcepcion: public std::exception {
public:

	/**
	 * @brief Códigos de error
	 */
	enum CodigoError {
		THREAD_EN_USO,			//!< Intento de iniciar un hilo vivo
		SIN_RECURSOS, 			//!< El sistema no posee los recursos
								//!< necesarios para iniciar el hilo
		ATTR_INVALIDOS,         //!< Atributos inválidos
		THREAD_NO_JOINABLE,     //!< Intento de join a un hilo no joinable
		JOIN_SELF_ERROR,        //!< Intento de join consigo mismo
		CANCEL_SELF_ERROR,      //!< Intento de cancel consigo mismo
		THREAD_ID_INVALIDO,     //!< No existe un hilo con ese id
	};

	/**
	 * @brief Constructor
	 * @param motivo Texto descriptivo del error
	 * @param codigo Código numérico del error
	 */
	MultiHiloExcepcion(const char *motivo, enum CodigoError codigo) throw();

	/**
	 * @brief Destructor
	 */
	virtual ~MultiHiloExcepcion() throw();

	/**
	 * @brief Método que obtiene una descripción del error
	 * @return Una descripción del error
	 */
	virtual const char* what() const throw();

	/**
	 * @brief Método que obtiene el código de error
	 * @return El código de error
	 */
	CodigoError getCodigoError() const throw();

private:

	std::string motivo;
	enum CodigoError error;

	std::string codigoAstring() const;
};
}
#endif
