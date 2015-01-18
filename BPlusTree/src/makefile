OBJS = main.o Cargo.o Conteo.o Distrito.o Eleccion.o Grabable.o Candidato.o Configuracion.o Informe.o Lista.o Logger.o Logueable.o \
Votante.o Utilidades.o VotoElectronicoExcepcion.o BloqueHash.o RegistroIndice.o Menu.o DataAccess.o hash_extensible.o MenuVotante.o \
RegistroArbol.o Nodo.o ArbolBMas.o ManejoIDs.o ABMentidades.o Tester.o Test.o TestCargo.o TestDataAccess.o TestDistrito.o TestEleccion.o TestVotante.o \
TestCandidato.o TestLista.o TestABMentidades.o TestPersistencias.o DataGetter.o ConsultaEntidades.o UtilidadesTests.o\
CargaInicial.o VotanteAleatorio.o Kasiski.o Vigenere.o Factorizacion.o TestFactorizacion.o Administrador.o RSA.o TestRomperRSA.o TestRSA.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

VotoElectronico: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o VotoElectronico

main.o: main.cpp Menu.h Distrito.h Cargo.h DataAcces.h Tests/Tester.h RegistroArbol.h Nodo.h ArbolBMas.h Criptografía/RSA/TestRomperRSA.h
	$(CC) $(CFLAGS) main.cpp

Cargo.o: Cargo.cpp Cargo.h ManejoIDs.h
	$(CC) $(CFLAGS) Cargo.cpp

Cargo.h: Grabable.h Logueable.h Utilidades.h Configuracion.h

Distrito.o: Distrito.cpp Distrito.h
	$(CC) $(CFLAGS) Distrito.cpp

Distrito.h: Grabable.h Logueable.h ManejoIDs.h Utilidades.h VotoElectronicoExcepcion.h Configuracion.h

Eleccion.o: Eleccion.cpp Eleccion.h ManejoIDs.h
	$(CC) $(CFLAGS) Eleccion.cpp

Eleccion.h: Cargo.h Distrito.h RegistroIndice.h hash_extensible.h Configuracion.h

Grabable.o: Grabable.cpp Grabable.h
	$(CC) $(CFLAGS) Grabable.cpp

Candidato.o: Candidato.cpp Candidato.h ManejoIDs.h
	$(CC) $(CFLAGS) Candidato.cpp

Candidato.h: Grabable.h Logueable.h Votante.h Cargo.h

Configuracion.o: Configuracion.cpp Configuracion.h
	$(CC) $(CFLAGS) Configuracion.cpp

Informe.o: Informe.cpp Informe.h DataAccess.h
	$(CC) $(CFLAGS) Informe.cpp

Informe.h: Eleccion.h Distrito.h Lista.h Conteo.h

Lista.o: Lista.cpp Lista.h ManejoIDs.h
	$(CC) $(CFLAGS) Lista.cpp

Lista.h: Grabable.h Logueable.h Eleccion.h

Logger.o: Logger.cpp Logger.h
	$(CC) $(CFLAGS) Logger.cpp

Logger.h: Logueable.h Utilidades.h VotoElectronicoExcepcion.h

Logueable.o: Logueable.cpp Logueable.h
	$(CC) $(CFLAGS) Logueable.cpp

Votante.o: Votante.cpp Votante.h ManejoIDs.h
	$(CC) $(CFLAGS) Votante.cpp

Votante.h: Grabable.h Logueable.h Distrito.h Eleccion.h Lista.h DataAccess.h Criptografía/RSA/RSA.h

Conteo.o: Conteo.cpp Conteo.h
	$(CC) $(CFLAGS) Conteo.cpp

Conteo.h: Grabable.h Lista.h Logueable.h Distrito.h Eleccion.h Configuracion.h hash_extensible.h DataAccess.h

Utilidades.o: Utilidades.cpp Utilidades.h
	$(CC) $(CFLAGS) Utilidades.cpp

VotoElectronicoExcepcion.o: VotoElectronicoExcepcion.cpp VotoElectronicoExcepcion.h
	$(CC) $(CFLAGS) VotoElectronicoExcepcion.cpp

RegistroIndice.o: RegistroIndice.cpp RegistroIndice.h
	$(CC) $(CFLAGS) RegistroIndice.cpp

BloqueHash.o: BloqueHash.cpp BloqueHash.h
	$(CC) $(CFLAGS) BloqueHash.cpp

BloqueHash.h: Bloque.h RegistroIndice.h

Menu.o: Menu.cpp Menu.h DataAccess.h
	$(CC) $(CFLAGS) Menu.cpp

Menu.h: MenuVotante.h ABMEntidades/ABMentidades.h ABMEntidades/ConsultaEntidades.h

MenuVotante.o: MenuVotante.cpp MenuVotante.h DataGetter.h
	$(CC) $(CFLAGS) MenuVotante.cpp

MenuVotante.h: Votante.h Eleccion.h Lista.h

DataAccess.o: DataAccess.cpp DataAccess.h
	$(CC) $(CFLAGS) DataAccess.cpp

DataAcces.h: Grabable.h Logueable.h Logger.h Utilidades.h VotoElectronicoExcepcion.h Configuracion.h Conteo.h

DataGetter.o: DataGetter.cpp DataGetter.h
	$(CC) $(CFLAGS) DataGetter.cpp

DataGetter.h: Distrito.h Conteo.h Votante.h Eleccion.h Lista.h

hash_extensible.o: hash_extensible.cpp hash_extensible.h
	$(CC) $(CFLAGS) hash_extensible.cpp

hash_extensible.h: BloqueHash.h RegistroIndice.h

Tester.o: Tests/Tester.cpp Tests/Tester.h Tests/Test.h Tests/TestCargo.h Tests/TestDataAccess.h Tests/TestDistrito.h \
Tests/TestEleccion.h Tests/TestVotante.h Tests/TestCandidato.h Tests/TestLista.h Tests/TestABMentidades.h \
Tests/TestPersistencias.h
	$(CC) $(CFLAGS) Tests/Tester.cpp

Test.o: Tests/Test.cpp Tests/Test.h
	$(CC) $(CFLAGS) Tests/Test.cpp

TestCargo.o: Tests/TestCargo.cpp Tests/TestCargo.h DataAccess.h
	$(CC) $(CFLAGS) Tests/TestCargo.cpp

Tests/TestCargo.h: Tests/Test.h Cargo.h

TestDataAccess.o: Tests/TestDataAccess.cpp
	$(CC) $(CFLAGS) Tests/TestDataAccess.cpp

Tests/TestDataAccess.h: Tests/Test.h DataAccess.h

TestDistrito.o: Tests/TestDistrito.cpp Tests/TestDistrito.h
	$(CC) $(CFLAGS) Tests/TestDistrito.cpp

Tests/TestDistrito.h: Tests/Test.h Distrito.h

TestEleccion.o: Tests/TestEleccion.cpp Tests/TestEleccion.h DataAccess.h
	$(CC) $(CFLAGS) Tests/TestEleccion.cpp

Tests/TestEleccion.h: Tests/Test.h Eleccion.h Cargo.h Distrito.h

TestLista.o: Tests/TestLista.cpp Tests/TestLista.h
	$(CC) $(CFLAGS) Tests/TestLista.cpp

Tests/TestLista.h: Tests/Test.h Lista.h Eleccion.h Cargo.h Distrito.h

TestVotante.o: Tests/TestVotante.cpp Tests/TestVotante.h DataAccess.h
	$(CC) $(CFLAGS) Tests/TestVotante.cpp

Tests/TestVotante.h: Tests/Test.h Votante.h Distrito.h

TestCandidato.o: Tests/TestCandidato.cpp Tests/TestCandidato.h Eleccion.h Distrito.h
	$(CC) $(CFLAGS) Tests/TestCandidato.cpp

Tests/TestCandidato.h: Tests/Test.h Candidato.h Cargo.h Lista.h

TestABMentidades.o: Tests/TestABMentidades.cpp Tests/TestABMentidades.h ABMEntidades/ConsultaEntidades.h
	$(CC) $(CFLAGS) Tests/TestABMentidades.cpp

Tests/TestABMentidades.h: Tests/Test.h ABMEntidades/ABMentidades.h ABMEntidades/ConsultaEntidades.h

TestPersistencias.o: Tests/TestPersistencias.cpp Tests/TestPersistencias.h
	$(CC) $(CFLAGS) Tests/TestPersistencias.cpp

Tests/TestPersistencias.h: Tests/Test.h Distrito.h Cargo.h Eleccion.h DataAccess.h hash_extensible.h RegistroIndice.h \
Configuracion.h

UtilidadesTests.o: Tests/UtilidadesTests.cpp Tests/UtilidadesTests.h
	$(CC) $(CFLAGS) Tests/UtilidadesTests.cpp

Tests/UtilidadesTests.h: Distrito.h Cargo.h Eleccion.h Lista.h Votante.h Candidato.h

RegistroArbol.o: RegistroArbol.h RegistroArbol.cpp Utilidades.h
	$(CC) $(CFLAGS) RegistroArbol.cpp

RegistroArbol.h: Utilidades.h VotoElectronicoExcepcion.h

Nodo.o: Nodo.h Nodo.cpp RegistroArbol.h
	$(CC) $(CFLAGS) Nodo.cpp

ArbolBMas.o: ArbolBMas.h ArbolBMas.cpp
	$(CC) $(CFLAGS) ArbolBMas.cpp

ArbolBMas.h: RegistroArbol.h Nodo.h

ManejoIDs.o: ManejoIDs.cpp ManejoIDs.h
	$(CC) $(CFLAGS) ManejoIDs.cpp

ManejoIDs.h: Configuracion.h

CargaInicial.o: CargaInicial.h CargaInicial.cpp
	$(CC) $(CFLAGS) CargaInicial.cpp

ABMentidades.o: ABMEntidades/ABMentidades.cpp ABMEntidades/ABMentidades.h
	$(CC) $(CFLAGS) ABMEntidades/ABMentidades.cpp

ABMEntidades/ABMentidades.h: DataAccess.h Configuracion.h hash_extensible.h Eleccion.h Distrito.h Cargo.h Votante.h \
Lista.h Candidato.h RegistroIndice.h ManejoIDs.h Utilidades.h Administrador.h

ConsultaEntidades.o: ABMEntidades/ConsultaEntidades.cpp ABMEntidades/ConsultaEntidades.h
	$(CC) $(CFLAGS) ABMEntidades/ConsultaEntidades.cpp

ABMEntidades/ConsultaEntidades.h: Grabable.h DataAccess.h hash_extensible.h RegistroIndice.h

VotanteAleatorio.o: VotanteAleatorio/VotanteAleatorio.h VotanteAleatorio/VotanteAleatorio.cpp CargaInicial.h
	$(CC) $(CFLAGS) VotanteAleatorio/VotanteAleatorio.cpp

Kasiski.o: Criptografía/Kasiski.cpp Criptografía/Kasiski.h
	$(CC) $(CFLAGS) Criptografía/Kasiski.cpp

Vigenere.o: Criptografía/AlgoVigenere/Vigenere.cpp Criptografía/AlgoVigenere/Vigenere.h
	$(CC) $(CFLAGS) Criptografía/AlgoVigenere/Vigenere.cpp

RSA.o: Criptografía/RSA/RSA.cpp Criptografía/RSA/RSA.h
	$(CC) $(CFLAGS) Criptografía/RSA/RSA.cpp
	
Criptografía/RSA/RSA.h: Criptografía/Factorizacion.h
	
TestRSA.o: Criptografía/RSA/TestRSA.cpp Criptografía/RSA/TestRSA.h
	$(CC) $(CFLAGS) Criptografía/RSA/TestRSA.cpp

Criptografía/RSA/TestRSA.h: Criptografía/RSA/RSA.h
	
Factorizacion.o: Criptografía/Factorizacion.cpp Criptografía/Factorizacion.h
	$(CC) $(CFLAGS) Criptografía/Factorizacion.cpp
	
TestFactorizacion.o: Criptografía/TestFactorizacion.cpp Criptografía/TestFactorizacion.h
	$(CC) $(CFLAGS) Criptografía/TestFactorizacion.cpp

Criptografía/TestFactorizacion.h: Criptografía/Factorizacion.h Criptografía/RSA/RSA.h

Administrador.o: Administrador.cpp Administrador.h
	$(CC) $(CFLAGS) Administrador.cpp

Administrador.h: Grabable.h Logueable.h Utilidades.h Configuracion.h Criptografía/RSA/RSA.h

TestRomperRSA.o: Criptografía/RSA/TestRomperRSA.cpp Criptografía/RSA/TestRomperRSA.h Criptografía/RSA/RSA.h
	$(CC) $(CFLAGS) Criptografía/RSA/TestRomperRSA.cpp

Criptografía/RSA/TestRomperRSA.h: Criptografía/RSA/RSA.h