#include "MultiHiloExcepcion.h"
#include <sstream>

namespace POSIX {

MultiHiloExcepcion::MultiHiloExcepcion(const char *motivo,
		CodigoError codigo) throw() {
	this->motivo.assign(motivo);
	error = codigo;
}

MultiHiloExcepcion::~MultiHiloExcepcion() throw() {
}

// TODO revisar qué arrojan los what(). probé uno y decia cualq cosa
const char* MultiHiloExcepcion::what() const throw() {
	std::stringstream ss;
	ss << error;
	std::string msgError("MultiHiloExcepcion | Codigo: ");
	msgError.append(ss.str());
	msgError.append(" - ");
	msgError.append(codigoAstring());
	msgError.append(" | Motivo: ");
	msgError.append(motivo);
	return msgError.c_str();
}

MultiHiloExcepcion::CodigoError MultiHiloExcepcion::getCodigoError() const throw() {
	return error;
}

std::string MultiHiloExcepcion::codigoAstring() const {
	switch (error) {
	case THREAD_EN_USO:
		return std::string("THREAD_EN_USO");
	case SIN_RECURSOS:
		return std::string("SIN_RECURSOS");
	case ATTR_INVALIDOS:
		return std::string("ATTR_INVALIDOS");
	case THREAD_NO_JOINABLE:
		return std::string("THREAD_NO_JOINABLE");
	case JOIN_SELF_ERROR:
		return std::string("JOIN_SELF_ERROR");
	case CANCEL_SELF_ERROR:
		return std::string("CANCEL_SELF_ERROR");
	case THREAD_ID_INVALIDO:
		return std::string("THREAD_ID_INVALIDO");
	}
	return std::string("CODIGO DESCONOCIDO");
}

}
