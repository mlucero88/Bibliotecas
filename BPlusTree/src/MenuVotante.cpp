/*
 * Menu_votante.cpp
 *
 *  Created on: 07/10/2011
 *      Author: Miguel Torres
 */

#include "MenuVotante.h"

MenuVotante::MenuVotante(Votante *votante) {

	this->votante=votante;

	opciones();
}

void MenuVotante::opciones(){

	char opcion;
	bool opcion_elegida=false;

	do{
		system("clear");
		cout<<"***********************"<<endl;
		cout<<"Menu Votante"<<endl<<endl;
		cout<<"Usuario: "<<this->votante->getNombreYApellido()<<endl;


		cout<<"V - Votar en Eleccion"<<endl;
		cout<<"O - Cambiar Datos personales"<<endl;
		cout<<"S - Salir"<<endl<<endl;
		cout<<"Elegir opcion: ";

		cin>>opcion;

		opcion =(char) toupper(opcion);

		opcion_elegida=	opcion=='V' ||
						opcion=='O' ||
						opcion=='S';
	}
	while(!opcion_elegida);

	switch ((char)toupper(opcion)){
		case 'V':
			mostrarMenuElecciones();
			break;
		case 'O':
			mostrarMenuDatos();
			break;
		case 'S':
			break;
		default:
			break;
	}
}

void MenuVotante::mostrarMenuElecciones(){
	Eleccion *eleccion;

	eleccion=this->elegirEleccion();

	if(eleccion==NULL){
		opciones();
	}else{
		mostrarListasPresentadas(eleccion);
	}
}


Eleccion* MenuVotante::elegirEleccion(){

	bool eleccion_elegida=false;
	unsigned opcion;
	Eleccion *resultado;
	string fecha;

	do{
		cout<<endl<<"Ingrese la fecha de hoy en el siguiente formato DD/MM/AAAA: ";
		cin>>fecha;
		if(Utilidades::indexarFecha(fecha)=="aaaammdd")
			cout<<endl<<"La fecha ingresada no es correcta."<<endl;
		else
			break;
	}while(1);

	vector<Eleccion *> elecciones;

	DataGetter::getEleccionesPorFechayDistrito(elecciones, fecha, *(this->votante));
	unsigned int cantidad_elecciones=elecciones.size();

	system("clear");
	cout<<"************************************"<<endl;
	cout<<"Elecciones Disponibles: "<<endl<<endl;
	do{
		opcion=1;

		for(unsigned i=0;i<cantidad_elecciones;i++){

			cout<<opcion+i<<" - "<<elecciones[i]->getCargo().getCargoPrincipal()<<" "<<elecciones[i]->getFecha()<<endl;

		}

		cout<<"0 - Volver a menu anterior"<<endl<<endl;
		cout<<"Elegir Eleccion: ";
		cin>>opcion;
		eleccion_elegida=((opcion>=0) && (opcion<=cantidad_elecciones));

	}while(!eleccion_elegida);


	if(opcion==0){

		resultado=NULL;

	}else{
		resultado=elecciones[opcion-1];
	}

	return resultado;

}


void MenuVotante::mostrarListasPresentadas(Eleccion *eleccion){

	Lista *lista;

	bool confirmacion=false;

	do{

		lista=this->elegirLista(eleccion);

		if(lista!=NULL){
			confirmacion=confirmarVotacion(eleccion,lista);
		}

	}while((!confirmacion) && lista!=NULL);

	if (confirmacion){
		cout<<"Voto Aceptado"<<endl;

		//Se regresa al menu Principal
		opciones();
	}else if (lista==NULL) {
		mostrarMenuElecciones();
	}
}


Lista* MenuVotante::elegirLista(Eleccion *eleccion){

	Lista *lista_resultado;
	bool listaElegida=false;
	vector<Lista *> listas;

	DataGetter::getListasPorEleccion(listas, *eleccion);
	unsigned int cantidad_listas=listas.size();
	unsigned int opcion;

	do{
		system("clear");
		cout<<"************************************"<<endl;
		cout<<"Eleccion "<< eleccion->getCargo().getCargoPrincipal()<<endl<<endl;
		cout<<"Listas Presentadas:"<<endl;
		for(unsigned int i=0;i<cantidad_listas;i++){

			cout<<(i+1)<<" -"<<listas[i]->getNombre()<<endl;
		}

		cout<<endl<<"0 - Volver a menu anterior"<<endl;
		cout<<"Elegir Lista a Votar: ";
		cin>>opcion;

		listaElegida= ((opcion>=0) && (opcion<=cantidad_listas));
	}while(!listaElegida);

	if(opcion == 0){
		lista_resultado=NULL;
	}else{
		lista_resultado=listas[opcion-1];
		Logger::Voto(*(this->votante), this->votante->getDNI());

	}

	return lista_resultado;


}

bool MenuVotante::confirmarVotacion(Eleccion *eleccion,Lista *lista){

	//system("clear");
	char opcion;
	bool opcion_elegida=false;
	bool confirmacion=false;
	vector<Candidato *> candidatosListaElegida; // = getCandidatosPorLista(*lista);

	do{
		system("clear");
		cout<<"***********************"<<endl;
		cout<<"Se ha elegido la lista "<<lista->getNombre()<<endl<<endl;
		cout<<" Sus candidatos son: "<<endl;

		for(unsigned int i = 0; i < candidatosListaElegida.size(); i++)
		{
			cout<<"  -"<<candidatosListaElegida[i]->getNombreYApellido()<<endl;
		}
		cout<<endl;

		cout<<"C - Confirmar voto"<<endl;
		cout<<"R - Rechazar Voto"<<endl;
		cout<<"Elegir opcion: ";
		cin>>opcion;

		opcion =(char) toupper(opcion);

		opcion_elegida= opcion=='C' ||
						opcion=='R';


	}while(!opcion_elegida);

	if (opcion=='C'){
		confirmacion=true;
		ABMentidades * abm = new ABMentidades();

		abm->agregarVoto(*(this->votante), *lista, this->votante->getDistrito());
		//LOG de la confirmacion de la votacion
		Logger::ConfirmacionDeVoto(*(this->votante), this->votante->getDNI());
		delete abm;
	}

	if(opcion=='R')
	{
		Logger::CambioDeVoto(*(this->votante), this->votante->getDNI());
	}

	//Limpio el vector de candidatos
	int cantidadCandidatos = candidatosListaElegida.size();

	for(int i = 0; i < cantidadCandidatos; i++)
		delete candidatosListaElegida[i];

	return confirmacion;

}

void MenuVotante::mostrarMenuDatos(){

	char opcion;
	bool opcion_elegida=false;

	do{
		system("clear");
		cout<<"***********************"<<endl;
		cout<<"Menu Datos Personales"<<endl<<endl;
		cout<<"C - Cambiar Clave "<<endl;
		cout<<"D - Cambiar Domicilio"<<endl<<endl;
		cout<<"V - Volver a Menu Principal"<<endl;
		cout<<"Elegir opcion: ";

		cin>>opcion;

		opcion =(char) toupper(opcion);

		opcion_elegida=	opcion=='C' ||
						opcion=='D' ||
						opcion=='V';
	}
	while(!opcion_elegida);

	switch (opcion){
		case 'V':
			opciones();
				break;
		case 'C':
			this->cambiarClave();
			mostrarMenuDatos();
			break;
		case 'D':
			this->cambiarDomicilio();
			mostrarMenuDatos();
			break;
		default:
			break;
	}
}

void MenuVotante::cambiarClave(){

	string nueva_clave;
	string nueva_clave_conf;
	bool ingreso_correcto = false;

	while (!ingreso_correcto){

		cout<<"Ingresar Nueva Clave: ";
		system("stty -echo");
		cin >> nueva_clave;
		system("stty echo");
		cout<<endl;

		cout<<"Reingresar Nueva Clave: ";
		system("stty -echo");
		cin >> nueva_clave_conf;
		system("stty echo");
		cout<<endl;

		if (nueva_clave == nueva_clave_conf){

			cout<<"Clave Nueva ingresada Correctamente."<<endl;
			ingreso_correcto = true;
		}
		else{

			cout<<"Ingreso de clave nueva incorrecto."<<endl;
			cout<<"Vuelva a ingresarla. "<<endl;

		}

	}
	string clave_cambiada(nueva_clave);
	this->votante->cambiarClave(votante->getClave(),clave_cambiada);
	this->guardarCambios();
}

void MenuVotante::cambiarDomicilio(){

	char nuevo_domicilio[30];
	cout<<"Domicilio Anterior: "<<this->votante->getDomicilio()<<endl;
	cout<<"Ingrese Nuevo Domicilio: ";

	cin>>nuevo_domicilio;
	string nuevo_domicilio_str(nuevo_domicilio);
	this->votante->setDomicilio(nuevo_domicilio_str);
	this->guardarCambios();

}

void MenuVotante::guardarCambios(){
	ABMentidades abm;
	abm.modificacionVotante(*this->votante);

}

MenuVotante::~MenuVotante() {

}
