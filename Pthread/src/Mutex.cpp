#include "Mutex.h"

namespace POSIX {

Mutex::Mutex() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condVar, 0);
}

bool Mutex::bloquear() {
	return pthread_mutex_lock(&mutex) == 0;
}

bool Mutex::intentarBloquear() {
	return pthread_mutex_trylock(&mutex) == 0;
}

bool Mutex::desbloquear() {
	return pthread_mutex_unlock(&mutex) == 0;
}

void Mutex::wait() {
	pthread_cond_wait(&condVar, &mutex);
}

void Mutex::signal() {
	pthread_cond_signal(&condVar);
}

Mutex::~Mutex() {
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&condVar);
}
}
