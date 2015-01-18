#include "Thread.h"
#include "MultiHiloExcepcion.h"
#include <cerrno>
#include <cstring>
#include <csignal>

extern int errno;

namespace POSIX {

Thread::Atributos::Atributos() {
	pthread_attr_init(&atributos);
	setJoinable();
}

Thread::Atributos::Atributos(const Atributos &aCopiar) {
	this->atributos = aCopiar.atributos;
}

Thread::Atributos& Thread::Atributos::operator=(const Atributos &aAsignar) {
	this->atributos = aAsignar.atributos;
	return *this;
}

void Thread::Atributos::setDetached() {
	pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_DETACHED);
}

void Thread::Atributos::setJoinable() {
	pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE);
}

bool Thread::Atributos::isDetached() const {
	int detach_state;
	pthread_attr_getdetachstate(&atributos, &detach_state);
	return (detach_state == PTHREAD_CREATE_DETACHED);
}

bool Thread::Atributos::isJoinable() const {
	int detach_state;
	pthread_attr_getdetachstate(&atributos, &detach_state);
	return (detach_state == PTHREAD_CREATE_JOINABLE);
}

Thread::Atributos::~Atributos() {
	pthread_attr_destroy(&atributos);
}

Thread::Thread() {
	id = 0;
	esta_vivo = false;
	parametro = NULL;
}

Thread::Thread(const Atributos &atributos) :
		atributos(atributos) {
	id = 0;
	esta_vivo = false;
	parametro = NULL;
}

void Thread::iniciar(void *parametro) {
	if (estaVivo()) {
		throw MultiHiloExcepcion(strerror(errno),
				MultiHiloExcepcion::THREAD_EN_USO);
	}
	this->parametro = parametro;
	switch (pthread_create(&id, &atributos.atributos, lanzador,
			(void*) this)) {
	case EAGAIN:
		throw MultiHiloExcepcion(strerror(errno),
				MultiHiloExcepcion::SIN_RECURSOS);
	case EINVAL:
		throw MultiHiloExcepcion(strerror(errno),
				MultiHiloExcepcion::ATTR_INVALIDOS);
	}
	esta_vivo = true;
}

void Thread::esperarThread(Thread &aEsperar, void* &retorno) {
	switch (pthread_join(aEsperar.id, &retorno)) {
	case EINVAL:
		throw MultiHiloExcepcion(strerror(errno),
				MultiHiloExcepcion::THREAD_NO_JOINABLE);
	case ESRCH:
		throw MultiHiloExcepcion(strerror(errno),
				MultiHiloExcepcion::THREAD_ID_INVALIDO);
	case EDEADLK:
		throw MultiHiloExcepcion(strerror(errno),
				MultiHiloExcepcion::JOIN_SELF_ERROR);
	}
	aEsperar.esta_vivo = false;
	aEsperar.id = 0;
}

void Thread::cancelarEjecucionThread(Thread &aCancelar) {
	switch (pthread_cancel(aCancelar.id)) {
	case ESRCH:
		throw MultiHiloExcepcion(strerror(errno),
				MultiHiloExcepcion::THREAD_ID_INVALIDO);
	case EDEADLK:
		throw MultiHiloExcepcion(strerror(errno),
				MultiHiloExcepcion::CANCEL_SELF_ERROR);
	}
	aCancelar.esta_vivo = false;
	aCancelar.id = 0;
}

void Thread::setAtributos(const Atributos &atributos) {
	if (estaVivo()) {
		throw MultiHiloExcepcion(strerror(errno),
				MultiHiloExcepcion::THREAD_EN_USO);
	}
	this->atributos = atributos;
}

const Thread::Atributos& Thread::getAtributos() const {
	return atributos;
}

Thread::thread_id Thread::getId() const {
	return id;
}

Thread::thread_id Thread::getIdEnEjecucion() {
	return pthread_self();
}

bool Thread::enEjecucion() const {
	return (pthread_equal(id, pthread_self()));
}

bool Thread::estaVivo() const {
	return esta_vivo;
}

void Thread::setDetached() {
	if (!estaVivo()) {
		atributos.setDetached();
	}
	else {
		switch (pthread_detach(id)) {
		case ESRCH:
			throw MultiHiloExcepcion(strerror(errno),
					MultiHiloExcepcion::THREAD_ID_INVALIDO);
		case EINVAL:
			throw MultiHiloExcepcion(strerror(errno),
					MultiHiloExcepcion::THREAD_NO_JOINABLE);
		}
	}
}

void Thread::setJoinable() {
	if (estaVivo()) {
		throw MultiHiloExcepcion(strerror(errno),
				MultiHiloExcepcion::THREAD_EN_USO);
	}
	atributos.setJoinable();
}

bool Thread::operator==(const Thread &aComparar) {
	return pthread_equal(id, aComparar.id);
}

bool Thread::operator!=(const Thread &aComparar) {
	return !pthread_equal(id, aComparar.id);
}

Thread::~Thread() {
}

void* Thread::lanzador(void *objetoThread) {
	Thread *aLanzar = reinterpret_cast<Thread*>(objetoThread);
	aLanzar->esta_vivo = true;
	void *retorno = aLanzar->ejecutar(aLanzar->parametro);
	if (aLanzar->atributos.isDetached()) {
		/* Si es joinable no puedo cambiarle el id todavia, si lo hago va a
		 * fallar el esperarThread */
		aLanzar->id = 0;
		aLanzar->esta_vivo = false;
	}
	return(retorno);
}

}
