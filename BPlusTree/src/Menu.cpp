/*
 * Menu.cpp
 *
 *  Created on: Oct 1, 2011
 *      Author: pablo
 */

#include "Menu.h"

using namespace std;

Menu::Menu() {
	salir = "salir";
}

Menu *Menu::getMenu() {
	 static Menu instance;
	 return &instance;
}

void Menu::mostrarAyuda() {
    cout << "****************************************************************\n";
    cout << "Voto Electronico - Ayuda\n\n" << "Opciones:\n";
    cout << "-h, --help: Muestra esta ayuda\n";
    cout << "-u, --user: Login al sistema en calidad de usuario\n";
    cout << "-a, --admin: Login al sistema en calidad de administrador\n";
    cout << "-c, --cargainicial: Carga inicial de datos para prueba de la aplicacion\n";
    cout << "-v, --votanteautomatico: Ejecucion del votante automatico para prueba de informes.\n"
    		<<"Ver resultados en ./archivos/resultados_votante_aleatorio.txt\n\n";
    cout << "--romperRSA: Ejecucion de test para probar funcionalidad de ruptura de RSA\n\n";
    cout << "--testVigenere: Ejecucion de test para probar funcionalidad de encriptar/desencriptar con Vigenere\n\n";
    cout << "Consulte el manual para mas informacion\n";
    cout << "****************************************************************\n";
}

void Menu::pedirUsuario() {
    cout<<"Usuario: ";
    cin >> user;
}

void Menu::pedirContrasena() {
	cout<<"Contraseña: ";
	system("stty -echo");
	cin >> pass;
	system("stty echo");
}

bool Menu::accesoAdmin() {

	string userOk="tp";
	string passOk="tp";
	ConsultaEntidades consulta;
	Administrador admin;

	if (consulta.ObtenerRegistro(user,admin)) {
		if (pass == admin.getClave()) {
			adminMain();
			return true;
		}
		cout << "La contraseña es invalida." << endl;
		return false;
	}

	if ( user == userOk && pass == passOk ) {
		cout<< "Accedio al sistema." << endl;
		adminMain();
		return true;
	}

	if ( user == salir || pass == salir) {
		cout <<endl<< "Eligio terminar el programa." << endl;
		return true;
	}

	cout << "El usuario o la contraseña son invalidos." << endl;
	return false;
}

bool Menu::accesoUsuario() {
	DataAccess data_access;
	Votante votante;
	ConsultaEntidades consulta;


	if(consulta.ObtenerRegistro(this->user,votante))
	{
		if(votante.getClave()== pass)
		{
			cout<< "Accedio al sistema." << endl;

			//Se LLama al menu del Votante

			MenuVotante menuVotante(&votante);

			return true;
		}
		else {
			if ( user == salir || pass == salir ) {
				cout << "Eligio terminar el programa." << endl;
				return true;
			} else {
				cout << "El usuario o la contraseña son invalidos." << endl;
				return false;
			}
		}
	} else {
		if ( user == salir || pass == salir ) {
			cout << "Eligio terminar el programa." << endl;
			return true;
		} else {
			cout << "El usuario o la contraseña son invalidos." << endl;
			return false;
		}
	}
}

void Menu::adminMain() {
	char opcion;


	bool invalida=false;
	bool retorno=false;
	do {
		system("clear");
		if (invalida) {
			cout << "Opcion invalida, intente nuevamente." << endl;
		}
		cout << "********************************" << endl;
		cout << "A => Mantenimiento de Administradores." << endl;
		cout << "D => Mantenimiento de Distritos." << endl;
		cout << "V => Mantenimiento de Votantes." << endl;
		cout << "E => Mantenimiento de Elecciones." << endl;
		cout << "C => Mantenimiento de Cargos." << endl;
		cout << "L => Mantenimiento de Listas." << endl;
		cout << "T => Mantenimiento de Candidatos." << endl;
		cout << "R => Informes de Resultados." << endl;
		cout << "G => Encriptar/Desencriptar/Romper Reportes." << endl;
		cout << "S => Salir." << endl << "Opcion: ";
		cin >> opcion;

		retorno=false;
		invalida=false;
		switch ((char)toupper(opcion)) {
		case 'A':
			adminAdministrador();
			retorno=true;
			break;
		case 'T':
			adminCandidato();
			retorno=true;
			break;
		case 'L':
			adminLista();
			retorno=true;
			break;
		case 'E':
			adminEleccion();
			retorno=true;
			break;
		case 'D':
			adminDistrito();
			retorno=true;
			break;
		case 'V':
			adminVotante();
			retorno=true;
			break;
		case 'C':
			adminCargo();
			retorno=true;
			break;
		case 'R':
			adminInformes();
			retorno=true;
			break;
		case 'G':
			adminVigenere();
			retorno=true;
			break;
		case 'S':
			system("clear");
			break;
		default:
			invalida=true;
			break;
		}

	} while ((invalida) || (retorno));
}

void Menu::adminCandidato(){

	char opcion;
	bool invalida=false;
	bool retorno=false;

	ABMentidades abm;
	ConsultaEntidades consulta;

	Candidato candidato;
	Eleccion eleccion;
	string fechaEleccion;
	string cargo;

	Lista lista;
	string nombreLista;
	string claveConsulta;
	int dni;

	vector<Grabable *> *candidatos_grab;
	vector<string> *candidatos_str;
	vector<unsigned int> offsets;

	string arch_registros = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDCANDIDATO_REGS));
	string arch_bloq_libres = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDCANDIDATO_BLOQ_LIB));
	string arch_tabla = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDCANDIDATO_TABLA));
	hash_extensible* hash;

	do {
		system("clear");
		if (invalida) {
			cout << "Opcion invalida, intente nuevamente." << endl;
		}
		cout << "********************************" << endl;
		cout << "L => Listado  de Candidatos." << endl;
		cout << "C => Consulta de un Candidato."<<endl;
		cout << "A => Crear una Candidato." << endl;
		cout << "B => Borrar una Candidato." << endl;
		cout << "M => Modificar una Candidato." << endl;
		cout << "V => Volver." << endl << "Opcion: ";
		cin >> opcion;

		retorno=false;
		invalida=false;
		switch ((char)toupper(opcion)) {
		case 'L':
				/*
				 *
				 * Agregar las descripciones de los candidatos al
				 * vector de strings "candidatos"
				 *
				 */
				hash = new hash_extensible(arch_registros,arch_bloq_libres,arch_tabla);
				offsets =hash->listar();
				delete hash;

				candidatos_str= new vector<string>;
				candidatos_grab= new vector<Grabable *>;

				this->obtenerGrabables((*candidatos_grab),"Candidato",offsets,candidato.getURLArchivoDatos());



				if (candidatos_grab->size()==0){

					cout<<"Aun no hay ningun Distrito Cargada."<<endl;

				}else{
					int cant = candidatos_grab->size();
					Candidato *candidato_aux;

					for(int i=0 ; i<cant ;i++){

						if (candidato_aux = dynamic_cast<Candidato *> ((*candidatos_grab)[i])){
							candidatos_str->push_back(candidato_aux->getNombreYApellido());
							delete candidato_aux;
						}

					}


					this->listarEntidad((*candidatos_str));
				}

				delete candidatos_grab;
				delete candidatos_str;

				cout << "Ingrese cualquier letra para continuar: ";
				getchar();
				retorno=true;
				break;
		case 'C':
			cout << endl;
			cout << "Ingrese el nombre de la Lista del Candidato: ";
			cin.ignore();
			getline(cin, nombreLista);
			cout << "Lista ingresada: " << nombreLista <<endl;
			cout << endl <<endl;
			cout << "Eleccion a la que se postula: "<<endl;
			cout << "- Ingrese fecha: ";
			cin >> fechaEleccion;
			//cin.ignore() sirve para sacar el Enter del buffer y podes usar despues getline()

			cout<< "Fecha ingresada: " << fechaEleccion << endl;
			cin.ignore();
			cout << "- Ingrese el Cargo del candidato: ";
			getline(cin, cargo);

			cout<< "Cargo ingresado: " << cargo << endl;

			cout<< endl<<"Ingrese DNI del candidato: "<<endl;
			cin>>dni;

			claveConsulta = Utilidades::obtenerClaveLista(fechaEleccion, cargo, nombreLista);

			if(consulta.ObtenerRegistro(claveConsulta,lista))
			{
				string clave = Utilidades::obtenerClaveCandidato(fechaEleccion, cargo, nombreLista,dni);

				Candidato candidato;
				if (consulta.ObtenerRegistro(clave,candidato)){

					cout<<"Candidato: "<<candidato.getNombreYApellido()<<endl;
					cout<<"Cargo Postulado: "<<candidato.getCargo().getCargoPrincipal()<<endl;
					cout<<"Pertenece a la Lista: "<<candidato.getLista().getNombre()<<endl;
					cout<<"Datos Personales:"<<endl;
					cout<<"DNI: "<<candidato.getDNI()<<endl;
					cout<<"Domicilio: "<<candidato.getVotante().getDomicilio()<<endl;
					cout<<"Distrito: "<<candidato.getVotante().getDistrito().getNombre()<<endl;


				}else
					cout<<"No existe el Candidato ingresado."<<endl;
			}
			else
				cout<<endl<<"No existe la Lista Ingresada."<<endl;


			cout<<endl<<"Presione una tecla para continuar."<<endl;
			getchar();
			retorno=true;
			break;

		case 'A':
			cout << endl <<endl;
			cout << "Ingrese el nombre de la Lista del Candidato: ";
			cin.ignore();
			getline(cin, nombreLista);
			cout << "Lista ingresada: " << nombreLista <<endl;
			cout << endl <<endl;
			cout << "Eleccion a la que se postula: "<<endl;
			cout << "- Ingrese fecha: ";
			cin >> fechaEleccion;
			//cin.ignore() sirve para sacar el Enter del buffer y podes usar despues getline()

			cout<< "Fecha ingresada: " << fechaEleccion << endl;
			cin.ignore();
			cout << "- Ingrese el Cargo del candidato: ";
			getline(cin, cargo);

			cout<< "Cargo ingresado: " << cargo << endl;

			cout<< endl<<"Ingrese DNI del candidato: "<<endl;
			cin>>dni;

			claveConsulta = Utilidades::indexarFecha(fechaEleccion) + "$" + cargo + "$" + nombreLista;
			Utilidades::formatearClave(claveConsulta);

			if(consulta.ObtenerRegistro(claveConsulta,lista))
			{

				string dniAux = Utilidades::obtenerClaveVotante(dni);
				Votante votante;
				if (consulta.ObtenerRegistro(dniAux,votante)){
					Candidato candidato(votante,lista);
					if(abm.altaCandidato(candidato)){
						cout<<"El Candidato se creo correctamente."<<endl;
					}else{
						cout<<"Hubo un problema al dar de Alta al Candidato."<<endl;
					}

				}else{
					cout<<"No existe el Votante asociado al Candidato"<<endl;
				}

			}
			else
				cout<<endl<<"No existe la Lista Ingresada."<<endl;

			cout<<endl<<"Presione una tecla para continuar."<<endl;

			getchar();
			retorno=true;
			break;
		case 'B':
			cout << endl <<endl;
			cout << "Ingrese el nombre de la Lista del Candidato: ";
			cin.ignore();
			getline(cin, nombreLista);
			cout << "Lista ingresada: " << nombreLista <<endl;
			cout << endl <<endl;
			cout << "Eleccion a la que se postula: "<<endl;
			cout << "- Ingrese fecha: ";
			cin >> fechaEleccion;
			//cin.ignore() sirve para sacar el Enter del buffer y podes usar despues getline()

			cout<< "Fecha ingresada: " << fechaEleccion << endl;
			cin.ignore();
			cout << "- Ingrese el Cargo del candidato: ";
			getline(cin, cargo);

			cout<< "Cargo ingresado: " << cargo << endl;

			cout<< endl<<"Ingrese DNI del candidato: "<<endl;
			cin >> dni;

			claveConsulta = Utilidades::indexarFecha(fechaEleccion) + "$" + cargo + "$" + nombreLista;
			Utilidades::formatearClave(claveConsulta);

			if(consulta.ObtenerRegistro(claveConsulta,lista))
			{

				string clave = Utilidades::obtenerClaveCandidato(fechaEleccion, cargo, nombreLista,dni);
				Candidato candidato;
				if (consulta.ObtenerRegistro(clave,candidato)){

					if(abm.bajaCandidato(candidato)){

						cout<<"Se dio de baja como Candidato a:"<<candidato.getNombreYApellido()<<endl;

					}else{

						cout<<"Hubo un problema al dar de baja al Candidato."<<endl;

					}



				}else{
					cout<<"No existe el Candidato ingresado."<<endl;
				}

			}
			else
				cout<<endl<<"No existe la Lista Ingresada."<<endl;

			cout<<endl<<"Presione una tecla para continuar."<<endl;

			getchar();

			retorno=true;
			break;

		case 'M':

			cout << endl;
			cout << "Puede modificar un candidato accediendo a Modificar Lista para cambiar el nombre de su lista asociada, "<<endl;
			cout << "o puede modificar sus datos de votante accediendo al Modificar Votante con su DNI correspondiente" << endl;
			/*cout << "Ingrese el nombre de la Lista del Candidato: ";
			cin.ignore();
			getline(cin, nombreLista);
			cout << "Lista ingresada: " << nombreLista <<endl;
			cout << endl <<endl;
			cout << "Eleccion a la que se postula: "<<endl;
			cout << "- Ingrese fecha: ";
			cin >> fechaEleccion;
			//cin.ignore() sirve para sacar el Enter del buffer y podes usar despues getline()

			cout<< "Fecha ingresada: " << fechaEleccion << endl;
			cin.ignore();
			cout << "- Ingrese el Cargo del candidato: ";
			getline(cin, cargo);

			cout<< "Cargo ingresado: " << cargo << endl;

			cout<< endl<<"Ingrese DNI del candidato: "<<endl;
			cin>>dni;

			claveConsulta = Utilidades::indexarFecha(fechaEleccion) + "$" + cargo + "$" + nombreLista;
			Utilidades::formatearClave(claveConsulta);

			if(consulta.ObtenerRegistro(claveConsulta,lista))
			{

				string clave = Utilidades::obtenerClaveCandidato(fechaEleccion, cargo, nombreLista,dni);
				Candidato candidato;
				if (consulta.ObtenerRegistro(clave,candidato)){

					bool huboCambios=false;
					string nuevoDomicilio;
					cout<<"Candidato: "<<candidato.getNombreYApellido()<<" Postulado para:"<<candidato.getCargo().getCargoPrincipal()<<endl;
					cout<<"Domicilio Actual: "<<candidato.getVotante().getDomicilio()<<endl;
					cout<<"Ingrese Nuevo Domicilio(0 para no cambiarlo):";

					cin.ignore();
					getline(cin,nuevoDomicilio);

					if(nuevoDomicilio!="0"){
						huboCambios=true;
						candidato.getVotante().setDomicilio(nuevoDomicilio);

					}


					if(huboCambios){

						if(abm.modificacionCandidato(candidato)){
							cout<<"Se modifico correctamente el Candidato."<<endl;
						}else{
							cout<<"Hubo un problema al intertar modificar el Candidato."<<endl;
						}

					}




				}else{
					cout<<"No existe el Candidato ingresado."<<endl;
				}

			}
			else
				cout<<endl<<"No existe la Lista Ingresada."<<endl;


			cout<<endl<<"Presione una tecla para continuar."<<endl;
			getchar();
			retorno=true;*/
			retorno=true;
			break;

		case 'V':
			system("clear");
			break;
		default:
			invalida=true;
			break;
		}

	} while ((invalida) || (retorno));
}

void Menu::adminLista(){


	char opcion;
	bool invalida=false;
	bool retorno=false;

	ABMentidades abm;
	ConsultaEntidades consulta;

	Eleccion eleccion;
	string fechaEleccion;
	string cargo;

	Lista lista;
	string nombreLista;
	string claveConsulta;

	vector<string> *listas_str;
	Grabable *listaGrabable;
	vector<Grabable *> *listas_grab;
	vector<unsigned int> offsets;

	string arch_registros = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDLISTA_REGS));
	string arch_bloq_libres = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDLISTA_BLOQ_LIB));
	string arch_tabla = ((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDLISTA_TABLA));
	hash_extensible* hash;

	do {
		system("clear");
		if (invalida) {
			cout << "Opcion invalida, intente nuevamente." << endl;
		}
		cout << "********************************" << endl;
		cout << "L => Mostrar Listas." << endl;
		cout << "C => Consultar una Lista." << endl;
		cout << "A => Crear una Lista." << endl;
		cout << "B => Borrar una Lista." << endl;
		cout << "M => Modificar una Lista." << endl;
		cout << "V => Volver." << endl << "Opcion: ";
		cin >> opcion;

		retorno=false;
		invalida=false;
		switch ((char)toupper(opcion)) {
		case 'L':
					/*
					 *Agregar las descripciones de todas las listas
					 * al vector de strings
					 */
					listaGrabable = new Lista();

					hash = new hash_extensible(arch_registros,arch_bloq_libres,arch_tabla);

					offsets = hash->listar();

					listas_str = new vector<string>;
					listas_grab = new vector<Grabable *>;

					this->obtenerGrabables((*listas_grab),"Lista",offsets,listaGrabable->getURLArchivoDatos());

					delete listaGrabable;

					if (listas_grab->size()==0){

						cout<<"Aun no hay ninguna Lista Cargada."<<endl;

					}else{
						int cant = listas_grab->size();
						Lista *lista_aux;
						for(int i=0 ; i<cant ;i++){

							lista_aux = dynamic_cast<Lista *> ((*listas_grab)[i]);

							listas_str->push_back(lista_aux->getNombre());

							delete lista_aux;
						}


						this->listarEntidad((*listas_str));
					}

			delete listas_grab;
			delete listas_str;

			cout<<endl<<"Presione una tecla para continuar."<<endl;
			getchar();
			retorno=true;
			break;
		case 'C':
			cout << endl <<endl;
			cout << "Ingrese el nombre de la Lista: ";
			cin.ignore();
			getline(cin, nombreLista);
			cout << "Lista ingresada: " << nombreLista <<endl;
			cout << endl <<endl;
			cout << "Ingrese el Fecha de Eleccion: ";
			cin >> fechaEleccion;
			//cin.ignore() sirve para sacar el Enter del buffer y podes usar despues getline()

			cout<< "Fecha ingresada: " << fechaEleccion << endl;
			cin.ignore();
			cout << "Ingrese el Cargo: ";
			getline(cin, cargo);

			cout<< "Cargo ingresado: " << cargo << endl;

			claveConsulta = Utilidades::indexarFecha(fechaEleccion) + "$" + cargo + "$" + nombreLista;
			Utilidades::formatearClave(claveConsulta);

			if(consulta.ObtenerRegistro(claveConsulta, lista))
			{
				cout<<endl;
				lista.Imprimir();
			}
			else
				cout<<endl<<"No existe la lista solicitada."<<endl;

			cout<<endl<<"Presione una tecla para continuar."<<endl;

			getchar();
			retorno=true;
			break;
		case 'A':
			cout << endl <<endl;
			cout << "Ingrese el nombre de la Lista: ";
			cin.ignore();
			getline(cin, nombreLista);
			cout << "Lista ingresada: " << nombreLista <<endl;
			cout << endl <<endl;
			cout << "Eleccion a presentarse: "<<endl;
			cout << "- Ingrese fecha: ";
			cin >> fechaEleccion;
			//cin.ignore() sirve para sacar el Enter del buffer y podes usar despues getline()

			cout<< "Fecha ingresada: " << fechaEleccion << endl;
			cin.ignore();
			cout << "- Ingrese el cargo: ";
			getline(cin, cargo);

			cout<< "Cargo ingresado: " << cargo << endl;

			claveConsulta = Utilidades::indexarFecha(fechaEleccion) + "$" + cargo;
			Utilidades::formatearClave(claveConsulta);

			if(consulta.ObtenerRegistro(claveConsulta, eleccion))
			{
				claveConsulta += ("$" + nombreLista);
				Utilidades::formatearClave(claveConsulta);

				lista.setNombre(nombreLista);
				lista.setEleccion(eleccion);

				if(!abm.altaLista(lista))
					cout<<endl<<"La lista ya existe."<<endl;
			}
			else
				cout<<endl<<"No existe la eleccion a la cual se quiere presentar."<<endl;

			cout<<endl<<"Presione una tecla para continuar."<<endl;

			getchar();
			retorno=true;
			break;
		case 'B':

			cout << endl <<endl;
			cout << "Ingrese el nombre de la Lista: ";
			cin.ignore();
			getline(cin, nombreLista);
			cout << "Lista ingresada: " << nombreLista <<endl;
			cout << endl;
			cout << "Eleccion a la que pertenece: "<<endl;
			cout << "- Ingrese fecha: ";
			cin >> fechaEleccion;
			//cin.ignore() sirve para sacar el Enter del buffer y podes usar despues getline()

			cout<< "Fecha ingresada: " << fechaEleccion << endl;
			cin.ignore();
			cout << "- Ingrese el Cargo: ";
			getline(cin, cargo);

			cout<< "Cargo ingresado: " << cargo << endl;

			claveConsulta = Utilidades::indexarFecha(fechaEleccion) + "$" + cargo;
			Utilidades::formatearClave(claveConsulta);

			if(consulta.ObtenerRegistro(claveConsulta, eleccion))
			{
				claveConsulta += ("$" + nombreLista);
				Utilidades::formatearClave(claveConsulta);

				if(consulta.ObtenerRegistro(claveConsulta,lista)){

					if(abm.bajaLista(lista)){
						cout<<"Hubo un problema al dar de baja la Lista."<<endl;
					}else{
						cout<<"Se dio de baja correctamente."<<endl;
					}

				}else{
					cout<<"No existe la Lista."<<endl;
				}


			}
			else
				cout<<endl<<"No existe la eleccion que se ingreso."<<endl;

			cout<<endl<<"Presione una tecla para continuar."<<endl;
			getchar();
			retorno=true;
			break;

		case 'M':

			cout << endl <<endl;
			cout << "Ingrese el nombre de la Lista: ";
			cin.ignore();
			getline(cin, nombreLista);
			cout << "Lista ingresada: " << nombreLista <<endl;
			cout << endl;
			cout << "Eleccion a la que pertenece: "<<endl;
			cout << "- Ingrese fecha: ";
			cin >> fechaEleccion;
			//cin.ignore() sirve para sacar el Enter del buffer y podes usar despues getline()

			cout<< "Fecha ingresada: " << fechaEleccion << endl;
			cin.ignore();
			cout << "- Ingrese el Cargo: ";
			getline(cin, cargo);

			cout<< "Cargo ingresado: " << cargo << endl;

			claveConsulta = Utilidades::indexarFecha(fechaEleccion) + "$" + cargo;
			Utilidades::formatearClave(claveConsulta);

			if(consulta.ObtenerRegistro(claveConsulta, eleccion))
			{
				claveConsulta += ("$" + nombreLista);
				Utilidades::formatearClave(claveConsulta);

				if(consulta.ObtenerRegistro(claveConsulta,lista)){
					bool hubo_cambios=false;
					string nv_nombre;
					cout<<"******Modificaciones******"<<endl;
					cout<<"Nombre de la Lista: "<<lista.getNombre()<<endl;
					cout<<"Ingrese Nuevo nombre para la Lista(0 para finalizar):";
					cin.ignore();
					getline(cin,nv_nombre);

					if(nv_nombre!="0"){

						hubo_cambios=true;
						lista.setNombre(nv_nombre);
					}


					if(hubo_cambios){
						if(abm.modificacionLista(lista)){
							cout<<"Se modifico correctamente."<<endl;
						}else{
							cout<<"Hubo un error al Modificar la Lista."<<endl;
						}
					}

				}else{
					cout<<"No existe la Lista."<<endl;
				}


			}
			else
				cout<<endl<<"No existe la eleccion que se ingreso."<<endl;

			cout<<endl<<"Presione una tecla para continuar."<<endl;
			getchar();
			retorno=true;
			break;

		case 'V':
			system("clear");
			break;
		default:
			invalida=true;
			break;
		}

	} while ((invalida) || (retorno));


}



void Menu::adminDistrito() {
	char opcion;
	bool invalida=false;
	bool retorno=false;
	std::string nombreDistrito;
	ABMentidades abm;
	ConsultaEntidades consulta;
	Distrito *distrito,*distritoGrabable;

	vector<string> *distritos;
	vector<unsigned int> offsets;
	vector<Grabable *> *distritos_grab;

	string arch_registros((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDDISTRITO_REGS));
	string arch_bloq_libres((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDDISTRITO_BLOQ_LIB));
	string arch_tabla((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDDISTRITO_TABLA));
	hash_extensible* hash;



	do {
		system("clear");
		if (invalida) {
			cout << "Opcion invalida, intente nuevamente." << endl;
		}
		cout << "********************************" << endl;
		cout << "L => Listar Distritos."<< endl;
		cout << "A => Crear un Distrito." << endl;
		cout << "B => Borrar un Distrito." << endl;
		cout << "M => Modificar un Distrito." << endl;
		cout << "V => Volver." << endl << "Opcion: ";
		cin >> opcion;

		retorno=false;
		invalida=false;
		switch ((char)toupper(opcion)) {

		case 'L':


					/*
					 * Agregar las descripciones de todos los Distritos
					 * al vector de strings "distritos"
					 */
					distritos = new vector<string>;
					distritos_grab = new vector<Grabable *>;


					hash = new hash_extensible(arch_registros,arch_bloq_libres,arch_tabla);

					offsets = hash->listar();
					delete hash;

					distrito = new Distrito(" ");

					this->obtenerGrabables((*distritos_grab),"Distrito",offsets,distrito->getURLArchivoDatos());
					delete distrito;


					if (distritos_grab->size()==0){

						cout<<"Aun no hay ningun Distrito Cargada."<<endl;

					}else{
						int cant = distritos_grab->size();
						Distrito *distrito_aux;
						for(int i=0 ; i<cant ;i++){

							if (distrito_aux = dynamic_cast<Distrito *> ((*distritos_grab)[i])){
								distritos->push_back(distrito_aux->getNombre());
								delete distrito_aux;
							}

						}


						this->listarEntidad((*distritos));
					}

					delete distritos;
					delete distritos_grab;

			cout << "Ingrese cualquier letra para continuar: ";
			cin >> opcion;
			retorno=true;
			break;

		case 'A':
			cout << endl <<endl;
			cout << "Ingrese el nombre del ditrito: ";

			cin.ignore();
			getline(cin,nombreDistrito);

			//nombre_distrito.append(nombre);

			distrito=new Distrito(nombreDistrito);

			if(abm.altaDistrito(*distrito))
				cout << "Se creo el Distrito \"" << nombreDistrito << "\"." << endl;
			else
				cout << "El Distrito ya existe o se produjo un error al crearlo." << endl;

			cout << "Ingrese cualquier letra para continuar: ";
			cin >> opcion;
			retorno=true;
			break;
		case 'B':
			cout << endl <<endl;
			cout << "Ingrese el nombre del ditrito a borrar: ";
			cin.ignore();
			getline(cin,nombreDistrito);

			distrito=new Distrito;


			if (!consulta.ObtenerRegistro(nombreDistrito,*distrito))
			{
				cout << "No existe el Distrito \"" << nombreDistrito << "\"." << endl;
			}else{
				abm.bajaDistrito(*distrito);
			}
			cout << "Presione cualquier letra para continuar: ";
			getchar();
			retorno=true;
			break;

		case 'M':
			cout << endl <<endl;
			cout << "Ingrese el nombre del ditrito a modificar: ";
			cin.ignore();
			getline(cin,nombreDistrito);

			distrito=new Distrito();

			if(consulta.ObtenerRegistro(nombreDistrito,*distrito)){
				cout << "Distrito encontrado." << endl;
				cout << "ingrese el nuevo nombre del distrito(0 para ningun cambio): ";
				cin.ignore();
				getline(cin,nombreDistrito);

				if(nombreDistrito!="0"){
					distrito->modificarNombre(nombreDistrito);

					if(abm.modificacionDistrito(*distrito)){
						cout<<"Se modifico el Distrito."<<endl;
					}else{
						cout << "Error al modificar el distrito."<<endl;
					}
				}
			}else{
				cout<<"El distrito No Existe"<<endl;
			}

			cout << "Ingrese cualquier letra para continuar: ";
			getchar();
			retorno=true;
			break;

		case 'V':
			system("clear");
			break;
		default:
			invalida=true;
			break;
		}

	} while ((invalida) || (retorno));
}

void Menu::adminCargo() {
	char opcion;
	bool invalida=false;
	bool retorno=false;
	string nombreCargo;
	Cargo cargo;
	int resultado;
	ConsultaEntidades consulta;
	ABMentidades abm;

	vector<string> *cargos;
	vector<Grabable *> *cargos_grab;
	vector<unsigned int> offsets;

	string arch_registros((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDCARGO_REGS));
	string arch_bloq_libres((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDCARGO_BLOQ_LIB));
	string arch_tabla((*Configuracion::getConfig()).getValorPorPrefijo(RUTA_HASH_IDCARGO_TABLA));
	hash_extensible* hash;

		do {
			system("clear");
			if (invalida) {
				cout << "Opcion invalida, intente nuevamente." << endl;
			}
			cout << "********************************" << endl;
			cout << "L => Listar Cargos."<<endl;
			cout << "C => Consulta para un Cargo."<<endl;
			cout << "A => Crear un Cargo." << endl;
			cout << "B => Borrar un Cargo." << endl;
			cout << "G => Agregar un Cargo secundario." << endl;
			cout << "V => Volver." << endl << "Opcion: ";
			cin >> opcion;

			retorno=false;
			invalida=false;
			switch ((char)toupper(opcion)) {
			case 'L':
					/*
					 * Agregar las descripciones de todos los Cargos
					 * al vector de strings "cargos"
					 */
					hash = new hash_extensible(arch_registros,arch_bloq_libres,arch_tabla);
					offsets = hash->listar();

					cargos_grab= new vector<Grabable *>;
					cargos = new vector<string>;

					this->obtenerGrabables((*cargos_grab),"Cargo",offsets,cargo.getURLArchivoDatos());


					if ((*cargos_grab).size()==0){

						cout<<"Aun no hay ningun Cargo cargado."<<endl;

					}else{
						int cant = (*cargos_grab).size();
						Cargo *cargo_aux;
						for(int i=0 ; i<cant ;i++){

							cargo_aux = dynamic_cast<Cargo *> ((*cargos_grab)[i]);

							cargos->push_back(cargo_aux->getCargoPrincipal());

							delete cargo_aux;
						}


						this->listarEntidad((*cargos));


					}
				delete cargos;
				delete cargos_grab;


				cout << "Presione cualquier letra para continuar: ";
				getchar();
				retorno=true;
				break;

			case 'C':
				cout << endl <<endl;
				cout << "Ingrese el nombre del Cargo: ";
				cin.ignore();
				//fgets(nombre,50,stdin);
				getline(cin,nombreCargo);

				if(!(consulta.ObtenerRegistro(nombreCargo, cargo)))
				{
					cout<<"El cargo no existe"<<endl;
					resultado = -1;
					break;
				} else {

					cout<<"Cargo: "<<cargo.getCargoPrincipal()<<endl;
					vector<string> cargosSec=cargo.getCargosSecundarios();
					int cantCargosSec=cargosSec.size();

					if(cantCargosSec>0)
						cout<<"Lista de Cargos Secundarios:"<<endl;
					else
						cout<<"No posee Cargos Secundarios."<<endl;

					for (int i=0;i<cantCargosSec;i++){
						cout<<i+1<<" - "<<cargosSec[i]<<endl;
					}

				}

				cout << "Presione cualquier letra para continuar: ";
				getchar();
				retorno=true;
				break;
			case 'A':
			{
				cout << endl <<endl;
				cout << "Ingrese el nombre del nuevo Cargo: ";
				cin.ignore();
				getline(cin,nombreCargo);


				Cargo nuevo(nombreCargo);

				bool seguirCargando=false;
				string cargoSec;
				cin.ignore();

				do{
					cout<<"Ingrese un Cargo Secundario(0 para no ingresar):";

					getline(cin,cargoSec);

					seguirCargando=cargoSec!="0";

					if(seguirCargando){
						nuevo.agregarCargoSecundario(cargoSec);
					}


				}while(seguirCargando);


				//Se crea el cargo, verificando si no existia
				if(abm.altaCargo(nuevo))
					cout << "Se creo el Cargo \"" << nombreCargo << "\"." << endl;
				else
					cout << "El cargo ya existe o se produjo un error al crearlo." << endl;

				cout << "Ingrese cualquier letra para continuar: ";
				getchar();
				retorno=true;
				break;
			}
			case 'B':
				cout << endl <<endl;
				cout << "Ingrese el nombre del Cargo a borrar: ";

				cin.ignore();
				getline(cin,nombreCargo);


				if(!(consulta.ObtenerRegistro(nombreCargo, cargo)))
				{
					cout<<"El cargo no existe"<<endl;
					resultado = -1;
					break;
				}
				if(abm.bajaCargo(cargo))
					cout << "Se borro el cargo \"" << nombreCargo << "\"." << endl;
				else
					cout<< "No se ha podido borrar al cargo." << endl;

				cout << "Ingrese cualquier letra para continuar: ";
				getchar();
				retorno=true;
				break;
			case 'G':
				cout << endl <<endl;
				cout << "Ingrese el nombre del Cargo a modificar: ";
				cin.ignore();
				getline(cin,nombreCargo);

				if(!(consulta.ObtenerRegistro(nombreCargo, cargo)))
				{
					cout<<"El cargo no existe"<<endl;
					resultado = -1;
					break;
				} else {
					string cargoSec;
					bool seguirCargando=false;
					bool huboCambios=false;
					cout << endl;
					do{
						cout << "Ingrese el nombre del Cargo secundario(0 para finalizar): ";
						cin.ignore();
						getline(cin,cargoSec);

						seguirCargando=cargoSec!="0";

						if(seguirCargando){
							cargo.agregarCargoSecundario(cargoSec);
							huboCambios=true;
						}

					}while(seguirCargando);

					if(huboCambios){
						if(abm.modificacionCargo(cargo)){
							cout<<"Se agregaron Correctamente los Cargos Sec."<<endl;
						}else{
							cout<<"Hubo Problemas al Intentar modificar el Cargo."<<endl;
						}
					}
				}

				cout << "Presione cualquier letra para continuar: ";
				getchar();
				retorno=true;
				break;
			}
		} while ((invalida) || (retorno));

}


void Menu::adminEleccion(){


	char opcionSiNo;
	char opcion;
	bool invalida=false;
	bool retorno=false;

	ABMentidades abm;
	ConsultaEntidades consulta;

	Eleccion eleccion;
	string fecha_eleccion;

	Cargo cargo;
	string cargo_eleccion;

	Distrito distrito;
	string nom_distrito;

	string nombreCargo,claveConsulta;


	do {
		system("clear");
		if (invalida) {
			cout << "Opcion invalida, intente nuevamente." << endl;
		}
		cout << "********************************" << endl;
		cout << "C => Consultar una Eleccion." << endl;
		cout << "A => Crear un Eleccion." << endl;
		cout << "B => Borrar un Eleccion." << endl;
		cout << "M => Modificar un Eleccion." << endl;
		cout << "V => Volver." << endl << "Opcion: ";
		cin >> opcion;


		retorno=false;
		invalida=false;
		switch ((char)toupper(opcion)) {
		case 'C':
			cout << endl <<endl;
			cout << "Ingrese el Fecha de Eleccion: ";
			cin >> fecha_eleccion;
			//cin.ignore() sirve para sacar el Enter del buffer y podes usar despues getline()
			cin.ignore();
			cout << "Ingrese el Cargo: ";
			getline(cin, nombreCargo);

			claveConsulta = Utilidades::indexarFecha(fecha_eleccion) + "$" + nombreCargo;
			Utilidades::formatearClave(claveConsulta);

			if(consulta.ObtenerRegistro(claveConsulta, eleccion))
			{
				cout<<endl;
				eleccion.Imprimir();
				cout<<"Desea ver sus diistritos? (S/N)"<<endl;

				cin>>opcionSiNo;
				cin.ignore();

				if(toupper(opcionSiNo) == 'S')
					eleccion.ImprimirDistritos();
			}
			else
				cout<<endl<<"No existe la eleccion solicitada."<<endl;

			cout<<endl<<"Presione una tecla para continuar."<<endl;
			retorno=true;
			getchar();
			break;
		case 'A':
			cout << endl <<endl;

			cout<<"Ingrese el Cargo Para la Eleccion: ";
			cin.ignore();
			getline(cin,cargo_eleccion);


			if(consulta.ObtenerRegistro(cargo_eleccion,cargo)){

				cout << "Ingrese el Fecha de Eleccion: ";
				cin >> fecha_eleccion;

				string fecha_index=Utilidades::indexarFecha(fecha_eleccion);

				string clave= fecha_index + "$" + cargo_eleccion;
				Utilidades::formatearClave(clave);

				if(!consulta.ObtenerRegistro(clave,eleccion)){

					cout<<"Ingrese el Primer Distrito para esta Eleccion: ";
					cin.ignore();
					getline(cin,nom_distrito);


					if (consulta.ObtenerRegistro(nom_distrito,distrito)){

						eleccion.setCargo(cargo);
						eleccion.agregarDistrito(distrito);

						if(abm.altaEleccion(eleccion)){
							cout<<"La eleccion se creo correctamente."<<endl;
						}else{
							cout<<"Ya existe o hubo un problema al crear la eleccion."<<endl;
						}


					}else {
						cout<<"No existe el Distrito: "<<nom_distrito<<endl;
					}


				}else{
					cout<<"Ya Existe la Eleccion."<<endl;
				}

			}else{

				cout<<"No esta registrado el Cargo: "<<cargo_eleccion<<endl;

			}

		cout<<"Presione una tecla para continuar:";
		getchar();
		retorno=true;
		break;
		case 'B':

			cout<<"Ingrese el Cargo de la Eleccion a Borrar: ";
			cin.ignore();
			getline(cin,cargo_eleccion);

			cout << "Ingrese fecha de la Eleccion: ";
			cin >> fecha_eleccion;

			if(consulta.ObtenerRegistro(cargo_eleccion,cargo)){

				string fecha_index=Utilidades::indexarFecha(fecha_eleccion);
				string clave= fecha_index + "$" + cargo_eleccion;
				Utilidades::formatearClave(clave);
				if(consulta.ObtenerRegistro(clave,eleccion)){


					//Aca se hace la baja de la Eleccion
					if(!abm.bajaEleccion(eleccion)){
						cout<<"Hubo un problema al dar de baja la eleccion."<<endl;
					}

				}else{

					cout<<"No existe la Eleccion"<<endl;
				}


			}else{
				cout<<"No existe el Cargo Ingresado."<<endl;

			}


			cout<<"Presione una tecla para continuar:";
			getchar();
			retorno=true;
			break;

		case 'M':
			cout<<"Ingrese el Cargo de la Eleccion: ";
			cin >> cargo_eleccion;

			cout << "Ingrese fecha de la Eleccion: ";
			cin >> fecha_eleccion;


			if(consulta.ObtenerRegistro(cargo_eleccion,cargo)){

				string fecha_index=Utilidades::indexarFecha(fecha_eleccion);
				string clave=fecha_index + "$" + cargo_eleccion;

				if(consulta.ObtenerRegistro(clave,eleccion)){
						string cargo_sec;
						bool ingresando=false;
						bool hubo_cambios=false;

						do{
							cout<<"Agregue un Cargo Secundario(ingrese '0' para finalizar):";
							cin.ignore();
							getline(cin,cargo_sec);

							ingresando=cargo_sec!="0";
							if(ingresando){
								eleccion.getCargo().agregarCargoSecundario(cargo_sec);

								hubo_cambios=true;
							}

						}while(ingresando);

						ingresando=false;
						string nombreDistrito;

						do{
							cout<<"Ingrese nombre del Distrito a agregar(0 para finalizar):";
							cin.ignore();
							getline(cin,nombreDistrito);
							Distrito dist;

							ingresando=nombreDistrito!="0";
							if(ingresando){
								if(consulta.ObtenerRegistro(nombreDistrito,dist)){
									eleccion.agregarDistrito(dist);
									hubo_cambios=true;
								}else{
									cout<<"No existe el distrito."<<endl;
								}
							}

						}while(ingresando);


						if(hubo_cambios){

							if(!abm.modificacionEleccion(eleccion)){
								cout<<"Se produjo un error en la Modificacion"<<endl;
							}
						}

				}else{
					cout<<"No existe la eleccion."<<endl;
				}

			}else{
				cout<<"No existe el Cargo Ingresado."<<endl;

			}


			retorno=true;
			break;

		case 'V':
			system("clear");
			break;
		default:
			invalida=true;
			break;
		}

	} while ((invalida) || (retorno));


}


void Menu::adminVotante() {
    char opcion;
    bool invalida=false;
    bool retorno=false;
    string dniM,nombreVotante,direccionVotante,claveVotante,nombreDistrito,direccion;
    int dniN;

    Votante votante,*vot=NULL;
    Distrito distrito;
    ConsultaEntidades consulta;
    ABMentidades abm;

    int resultado;
    do {
        system("clear");
        if (invalida) {
                cout << "Opcion invalida, intente nuevamente." << endl;
        }
        cout << "********************************" << endl;
        cout << "C => Consulta de algun Votante."<< endl;
        cout << "A => Crear un Votante." << endl;
        cout << "B => Borrar un Votante." << endl;
        cout << "M => Modificar un Votante." << endl;
        cout << "V => Volver." << endl << "Opcion: ";
        //cin >> opcion;
        //fgets(&opcion,1,stdin);
        //cin.getline(&opcion,1);
        scanf("%c",&opcion);

        retorno=false;
        invalida=false;
        opcion = (char)toupper(opcion);
        switch (opcion) {

			case 'C':

				  dniN= -2;
			    do {
					if (dniN!=-2) {
						cout << "El DNI ingresado es invalido. para salir ingrese salir" << endl;
					}
					cout << "Ingrese el DNI del Votante a modificar: ";
					cin.ignore();
					getline(cin,dniM);
					dniN = atoi(dniM.c_str());
				} while (( (dniN<5000000) || (dniN>100000000) ) && dniM != salir);

				//string dniAux(dni);

				if(consulta.ObtenerRegistro(dniM, votante))
				{
					cout<<"Nombre: "<<votante.getNombreYApellido()<<endl;
					cout<<"DNI: "<<votante.getDNI()<<endl;
					cout<<"Domicilio: "<<votante.getDomicilio()<<endl;
					cout<<"Distrito: "<<votante.getDistrito().getNombre()<<endl;



				}else{
					cout<<"El votante no existe"<<endl;

				}

				cout<< endl <<"Presione una tecla para continuar:"<<endl;
				retorno=true;
				getchar();
				break;
            case 'A':
                    cout <<endl; //((DNI)i, NombreyApellido, clave, domicilio, (distrito)ie, ((eleccion)ie)*)
                    int dniN;
                    dniN= -2;
                    do {
							if (dniN!=-2) {
								cout << "El DNI ingresado es invalido." << endl;
							}

                            cout << "Ingrese el DNI: ";

                            cin.ignore();
							getline(cin,dniM);
							dniN = atoi(dniM.c_str());


                    } while ( (dniN<5000000) || (dniN>100000000) );

                    cout<<"Ingrese una clave para el Votante:";

                    //fgets(claveVotante,50,stdin);
                    getline(cin,claveVotante);

                    cout << "Ingrese el nombre y apellido del nuevo Votante: ";
                    //fgets(nombre,50,stdin);
                    getline(cin,nombreVotante);

                    cout << "Ingrese el domicilio: ";
                    //fgets(direccion,30,stdin);
                    getline(cin,direccion);

                    cout << "ingrese el nombre del distrito del Votante: ";
                    //fgets(nombreDistrito,50,stdin);
                    getline(cin,nombreDistrito);

                    cout<<"Distrito es: "<<nombreDistrito<<"."<<endl;

                    //Se verifica si existe el distrito
                    if(consulta.ObtenerRegistro(nombreDistrito, distrito))
                        //votante.setDistrito(distrito);
                    	vot=new Votante(dniN,nombreVotante,claveVotante,direccion,distrito);
                    else
                        cout << "No existe el distrito " <<nombreDistrito<< endl; // no se encontro distrito

                    //Hecho...

                    //Se crea el votante, verificando si no existia

                    //if(abm.altaVotante(votante))
                    if(vot!=NULL && abm.altaVotante(*vot))
                        cout << "Se creo el Votante \"" << nombreVotante << "\"." << endl;
                    else
                        cout << "El votante ya existe o se produjo un error al crearlo." << endl;

                    cout << "Ingrese cualquier letra para continuar: ";
                    cin >> opcion;
                    retorno=true;
                    break;

                    /*
                     * Borrar Votante
                     */
            case 'B':
                    dniN= -2;
                    do {
                            if (dniN!=-2) {
                                    cout << "El DNI ingresado es invalido. para salir ingrese salir" << endl;
                            }
                            cout << "Ingrese el DNI del Votante a borrar: ";

                            cin.ignore();
							getline(cin,dniM);
							dniN = atoi(dniM.c_str());

                    } while (( (dniN<5000000) || (dniN>100000000) ) && dniM != salir);

                    if(!(consulta.ObtenerRegistro(dniM, votante)))
                    {
                        cout<<"El votante no existe"<<endl;
                        resultado = -1;
                        getchar();
                        break;
                    }

                    if(abm.bajaVotante(votante))
                        cout << "Se borro el Votante \"" << votante.getNombreYApellido() << "\"." << endl;
                    else
                        cout<< "No se ha podido borrar al votante." << endl;
                    
                    cout << "Ingrese cualquier letra para continuar: ";
                    cin >> opcion;
                    retorno=true;
                    break;

                    /*
                     * Modificar un Votante
                     *
                     */

            case 'M':
                dniN= -2;
               do {
                    if (dniN!=-2) {
                        cout << "El DNI ingresado es invalido. para salir ingrese salir" << endl;
                    }
                    cout << "Ingrese el DNI del Votante a modificar: ";

                    cin.ignore();
					getline(cin,dniM);
					dniN = atoi(dniM.c_str());
                } while (( (dniN<5000000) || (dniN>100000000) ) && dniM != salir);

                //string dniAux(dni);

                if(!(consulta.ObtenerRegistro(dniM, votante)))
                {
                    cout<<"El votante no existe"<<endl;
                    resultado = -1;
                    cout<<"Presione una tecla para continuar.";
                    getchar();
                    break;
                }
                resultado= 1;
                if (resultado>0) {
                    bool invalida_v=false;
                    do {
                        system("clear");
                        if (invalida_v) {
                            cout << "Opcion invalida, intente nuevamente." << endl;
                        }
                        cout << "********************************" << endl;
                        cout << "O => Modificar el domicilio." << endl;
                        cout << "D => Modificar el distrito." << endl;
                        cout << "V => Volver." << endl << "Opcion: ";
                        cin >> opcion;

                        invalida_v=true;
                        switch ((char)toupper(opcion)) {
                        case 'o':
                        case 'O':
                            cout << "ingrese el nuevo domicilio del Votante(0 para no cambiar): ";
                            cin.ignore();
                            getline(cin, direccion);
                            if(direccion!="0"){
                            	votante.setDomicilio(direccion);
                            	invalida_v=false;
                            }
                            break;
                        case 'd':
                        case 'D':
                            cout << "ingrese el nombre del nuevo distrito del Votante(0 para no cambiar): ";
                            cin.ignore();
                            getline(cin, nombreDistrito);
                            if(nombreDistrito!="0"){
								if(consulta.ObtenerRegistro(nombreDistrito, distrito)){
									votante.setDistrito(distrito);
									invalida_v=false;
								}
								else
									cout << "No existe el distrito" << resultado << endl; // no se encontro distrito
                            }
                            break;
                        case 'v':
                        case 'V':
                            break;
                        default:
                            invalida_v=true;
                            break;
                        }

                        if(!invalida_v && (opcion != 'V'))
                        {
                            if((abm.modificacionVotante(votante)))
                                cout << "Se modifico el Votante \"" << nombreVotante << "\"." << endl;
                            else
                                cout << "Error al modificar al Votante." << endl;
                        }

                    } while (invalida_v);
                } else {
                        cout << "No se encontro el Votante \"" << nombreVotante << "\"." << endl;
                }
                
                cout << "Ingrese cualquier letra para continuar: ";
                cin >> opcion;
                retorno=true;
                break;
            case 'V':
                system("clear");
                break;
            default:
                invalida=true;
                break;
        }

    } while ((invalida) || (retorno));
}

void Menu::adminInformes(){

    char opcion;
    bool invalida=false;
    bool retorno=false;
    string nombreDistrito;
    Eleccion eleccion;
    string fechaEleccion,fechaEleccionIndex,cargoEleccion;
    string clave;
    string nombreCargo,nombreLista;

    Distrito distrito;
    Lista lista;

    Informe *informe;
    ConsultaEntidades consulta;
    ABMentidades abm;

    do {
        system("clear");
        if (invalida) {
                cout << "Opcion invalida, intente nuevamente." << endl;
        }
        cout << "********************************" << endl;
        cout << "E => Presentar Informe por Eleccion." << endl;
        cout << "D => Presentar Informe por Distrito." << endl;
        cout << "L => Presentar Informe por Lista." << endl;
        cout << "V => Volver." << endl << "Opcion: ";
        cin >> opcion;

        retorno=false;
        invalida=false;
        opcion = (char)toupper(opcion);
        switch (opcion) {

            case 'E':

				cout<<"Ingrese fecha Eleccion:";
				cin>>fechaEleccion;
				cout<<"Ingrese Cargo:";
				cin.ignore();
				getline(cin, cargoEleccion);

				clave=Utilidades::obtenerClaveEleccion(fechaEleccion,cargoEleccion);
				if(consulta.ObtenerRegistro(clave,eleccion)){
                    informe = new Informe(eleccion);
                    delete informe;
                    informe = new Informe(eleccion, "./Reportes/ReporteEleccion" );
					delete informe;

				}else{

					cout<<"No existe la Eleccion ingresada."<<endl;
				}

				cout << "Ingrese cualquier letra para continuar: ";
				cin >> opcion;
				retorno=true;
				break;

            case 'D':
				cout<<"Ingrese Nombre de Distrito:";
				cin.ignore();
				getline(cin, nombreDistrito);

				clave=Utilidades::obtenerClaveDistrito(nombreDistrito);

				if(consulta.ObtenerRegistro(clave,distrito)){

					vector<Conteo *> conteos;

					DataGetter::getConteosPorDistrito(conteos, distrito);

					informe=new Informe(distrito);
					delete informe;
                    informe=new Informe(distrito, "./Reportes/ReporteDistrito");
					delete informe;

				}else{

					cout<<"No existe distrito "<<nombreDistrito<<"."<<endl;

				}

				cout << "Ingrese cualquier letra para continuar: ";
				cin >> opcion;
				retorno=true;
				break;

            case 'L':

            	cout<<"Ingrese Nombre de Lista:";
				cin.ignore();
				getline(cin, nombreLista);

            	cout<<"Ingrese Cargo que se postula en la Lista:";
				//cin.ignore();
				getline(cin, nombreCargo);

            	cout<<"Ingrese fecha de eleccion para la lista:";
				cin>>fechaEleccion;

            	clave= Utilidades::obtenerClaveLista(fechaEleccion, nombreCargo, nombreLista);

            	if(consulta.ObtenerRegistro(clave,lista)){

            		informe= new Informe(lista);
            		delete informe;
                    informe= new Informe(lista, "./Reportes/ReporteLista");
            		delete informe;

            	}else{

            		cout<<"No Existe la Lista Ingresada."<<endl;
            	}

    			cout << "Ingrese cualquier letra para continuar: ";
    			cin >> opcion;;
                retorno=true;
                break;
            case 'V':
                system("clear");
                break;
            default:
                invalida=true;
                break;
        }

    } while ((invalida) || (retorno));
}


void Menu::adminAdministrador() {
	char opcion;
	bool invalida=false;
	bool retorno=false;
	std::string nombreAdministrador, claveAdministrador;
	ABMentidades abm;
	ConsultaEntidades consulta;
	Administrador *admin;

	do {
		system("clear");
		if (invalida) {
			cout << "Opcion invalida, intente nuevamente." << endl;
		}
		cout << "********************************" << endl;
		cout << "A => Crear un Administrador." << endl;
		cout << "B => Borrar un Administrador." << endl;
		cout << "V => Volver." << endl << "Opcion: ";
		cin >> opcion;

		retorno=false;
		invalida=false;
		switch ((char)toupper(opcion)) {

		case 'A':
			cout << endl <<endl;
			cout << "Ingrese el nombre de usuario del Administrador: ";

			cin.ignore();
			getline(cin,nombreAdministrador);

			cout << "Ingrese la clave del Administrador: ";
			cin >> claveAdministrador;

			admin = new Administrador(nombreAdministrador,claveAdministrador);

			if(abm.altaAdministrador(*admin))
				cout << "Se creo el Administrador \"" << nombreAdministrador << "\"." << endl;
			else
				cout << "El Administrador ya existe o se produjo un error al crearlo." << endl;

			cout << "Ingrese cualquier letra para continuar: ";
			cin >> opcion;
			retorno=true;
			break;

		case 'B':
			cout << endl <<endl;
			cout << "Ingrese el nombre del Administrador a borrar: ";
			cin.ignore();
			getline(cin,nombreAdministrador);

			admin = new Administrador;


			if (!consulta.ObtenerRegistro(nombreAdministrador,*admin))
			{
				cout << "No existe el Administrador \"" << nombreAdministrador << "\"." << endl;
			}else{
				abm.bajaAdministrador(*admin);
				cout << "La baja del Administrador fue completada." << endl;
			}
			cout << "Ingrese cualquier letra para continuar: ";
			cin >> opcion;
			retorno=true;
			break;

		case 'V':
			system("clear");
			break;
		default:
			invalida=true;
			break;
		}

	} while ((invalida) || (retorno));
}


void Menu::adminVigenere() {
	char opcion;
	bool invalida=false;
	bool retorno=false;
	std::string clave, rutaOrigen, rutaDestino;
	Vigenere vigenere;

	do {
		system("clear");
		if (invalida) {
			cout << "Opcion invalida, intente nuevamente." << endl;
		}
		cout << "********************************" << endl;
		cout << "E => Encriptar un Reporte." << endl;
		cout << "D => Desencriptar un Reporte." << endl;
                cout << "R => Romper la encriptación un reporte con el método de Kasiski." << endl;
		cout << "V => Volver." << endl << "Opcion: ";
		cin >> opcion;

		retorno=false;
		invalida=false;
		switch ((char)toupper(opcion)) {

		case 'E':
			cout << endl <<endl;
			cout << "Ingrese una clave para encriptar (usar solo caracteres ASCII de 7 bits): ";

			cin.ignore();
			getline(cin,clave);

			vigenere.setClave(clave);

			cout << "Ingrese la ruta del archivo a encriptar: ";
			cin >> rutaOrigen;

			cout << "Ingrese la ruta donde se guardara el archivo encriptado: ";
			cin >> rutaDestino;

			if (vigenere.cifrarArchivo(rutaOrigen,rutaDestino)) {
				cout << "El archivo se cifro correctamente." << endl;
			} else {
				cout << "El archivo no se cifro correctamente. Revise las rutas y vuelva a intentarlo." << endl;
			}

			cout << "Ingrese cualquier letra para continuar: ";
			cin >> opcion;
			retorno=true;
			break;

		case 'D':
			cout << endl <<endl;
			cout << "Ingrese la clave correspondiente para desencriptar: ";

			cin.ignore();
			getline(cin,clave);

			vigenere.setClave(clave);

			cout << "Ingrese la ruta del archivo a desencriptar (archivo ya cifrado): ";
			cin >> rutaOrigen;

			cout << "Ingrese la ruta donde se guardara el archivo desencriptado: ";
			cin >> rutaDestino;

			if (vigenere.descifrarArchivo(rutaOrigen,rutaDestino)) {
				cout << "El archivo se descifro correctamente." << endl;
			} else {
				cout << "El archivo no se descifro correctamente. Revise las rutas y la clave y vuelva a intentarlo." << endl;
			}

			cout << "Ingrese cualquier letra para continuar: ";
			cin >> opcion;
			retorno=true;
			break;

                case 'R':

			cout << "Ingrese la ruta del criptograma a romper: ";
			cin >> rutaOrigen;

			cout << "Ingrese la ruta donde se guardará el archivo obtenido: ";
			cin >> rutaDestino;

			if (Kasiski::Romper(rutaOrigen.c_str(),rutaDestino.c_str())) {
				cout << "El archivo se obtuvo correctamente." << endl;
                                cout << "nota: el método de Kasiski no es efectivo en todos los casos, por lo que el archivo generado ";
                                cout << "podría ser ilegible o imposible de abrir con el editor de textos." << endl;
			} else {
				cout << "Error: la ruta de origen o de destino es/son incorrecta/s." << endl;
			}

			cout << "Ingrese cualquier letra para continuar: ";
			cin >> opcion;
			retorno=true;
			break;

		case 'V':
			system("clear");
			break;
		default:
			invalida=true;
			break;
		}

	} while ((invalida) || (retorno));
}


void Menu::listarEntidad(vector<string> entidades){

	bool seguirMostrando=true;
	char opcion;
	int cantPorPantalla=10;
	int i,iPantalla=0;
	int cantEntidades=entidades.size();

	for ( i=0;i < cantEntidades  && seguirMostrando ; i++,iPantalla++){

		if (iPantalla >= cantPorPantalla){

					cout<<"Ingrese F para finalizar o alguna tecla para continuar:";
					cin >> opcion;
					opcion=(char)toupper(opcion);


					seguirMostrando = (opcion != 'F');
					iPantalla = 0;

		}else{

			cout<<" - "<<entidades[i]<<endl;
		}


	}


}



void Menu::obtenerGrabables(vector<Grabable *> &grabables,string tipo,vector<unsigned int> &offsets,string nom_archivo){

	DataAccess dtAccess;
	int cant= offsets.size();
	Grabable *obj;

	ifstream archivo;
	archivo.open(nom_archivo.c_str(), ios::binary | ios::out);
	unsigned int offsetActual;
//	for (int i=0; i < cant;i++)
//		cout<<"offset "<<i<<":  "<<offsets[i]<<endl;


	try {

		for (long int i=0; i < cant;i++){
			obj = this->instanciarGrabable(tipo);
			offsetActual = offsets[i];
			obj->Leer(archivo,offsetActual);

			grabables.push_back(obj);

		}
	}catch (VotoElectronicoExcepcion e){
		cout<<"Error en lectura de Archivo de "<<(*obj).getClassName()<<endl;
	}

	archivo.close();

}

Grabable* Menu::instanciarGrabable(string tipo){

	Grabable *instancia;

	if (tipo == "Distrito"){

		instancia = new Distrito(" ");

	}else if (tipo == "Lista"){

		instancia = new Lista();

	}else if (tipo == "Cargo"){

		instancia = new Cargo();

	}else if (tipo == "Candidato"){

		instancia = new Candidato();
	}

	return instancia;

}


Menu::~Menu() {
}
