/*
 * Informe.cpp
 *
 *  Created on: 19/09/2011
 *      Author: facundo
 */

#include "Informe.h"
#include <vector>
#include "DataAccess.h"
#include "DataGetter.h"

Informe::Informe(Eleccion& eleccion) {

	vector<Conteo *> conteos;

	DataGetter::getConteosPorEleccion(conteos, eleccion);

	vector<Conteo *>::size_type cantidadConteos = conteos.size();


	cout<<"Resultados De Eleccion "<<eleccion.getDescripcion()<<endl;

	string nom_primer_Distrito;
	string nom_actual_Distrito;
	if (cantidadConteos>0){
		nom_primer_Distrito=conteos[0]->getDistrito().getNombre();
		cout<<"Resultados Para Distrito: "<<nom_primer_Distrito<<endl;
		nom_actual_Distrito=nom_primer_Distrito;
	}

	long total_votos_distrito=0;
	long total_votos=0;




	for(string::size_type i = 0; i < cantidadConteos; i++)
	{

		if(conteos[i]->getDistrito().getNombre()!=nom_actual_Distrito){


			cout<<"		Total Votos en "<<nom_actual_Distrito<<": "<<total_votos_distrito<<endl;
			total_votos_distrito=0;

			nom_actual_Distrito=conteos[i]->getDistrito().getNombre();
			cout<<endl<<"Resultado Para Distrito: "<<nom_actual_Distrito<<endl;

		}


		Lista lista=conteos[i]->getLista();

		cout<<"		Lista: "<<lista.getNombre()<<". . . . . Votos: "<<conteos[i]->getVotos()<<endl;

		total_votos_distrito+=conteos[i]->getVotos();
		total_votos+=conteos[i]->getVotos();

	}

	cout<<"Total de votos en Eleccion: "<<total_votos<<endl;

}

////////////////////

Informe::Informe(Eleccion& eleccion, string rutaInforme){

    ofstream archInforme(rutaInforme.c_str(), ios::trunc);

    vector<Conteo *> conteos;

    DataGetter::getConteosPorEleccion(conteos, eleccion);

    vector<Conteo *>::size_type cantidadConteos = conteos.size();


    archInforme << "Resultados De Eleccion " << eleccion.getDescripcion() << endl;

    string nom_primer_Distrito;
    string nom_actual_Distrito;
    if (cantidadConteos>0){
            nom_primer_Distrito=conteos[0]->getDistrito().getNombre();
            archInforme<<"Resultados Para Distrito: "<<nom_primer_Distrito<<endl;
            nom_actual_Distrito=nom_primer_Distrito;
    }

    long total_votos_distrito=0;
    long total_votos=0;


    for(string::size_type i = 0; i < cantidadConteos; i++)
    {

            if(conteos[i]->getDistrito().getNombre()!=nom_actual_Distrito){


                    archInforme <<"		Total Votos en "<<nom_actual_Distrito<<": "<<total_votos_distrito<<endl;
                    total_votos_distrito=0;

                    nom_actual_Distrito=conteos[i]->getDistrito().getNombre();
                    archInforme <<endl<<"Resultado Para Distrito: "<<nom_actual_Distrito<<endl;

            }


            Lista lista=conteos[i]->getLista();

            archInforme <<"		Lista: "<<lista.getNombre()<<". . . . . Votos: "<<conteos[i]->getVotos()<<endl;

            total_votos_distrito+=conteos[i]->getVotos();
            total_votos+=conteos[i]->getVotos();

    }

    archInforme <<"Total de votos en Eleccion: "<<total_votos<<endl;
}

///////////////////

Informe::Informe(Distrito& distrito) {


	vector<Conteo *> conteos;

	DataGetter::getConteosPorDistrito(conteos, distrito);

	cout<<"Informe para Distito: "<<distrito.getNombre()<<endl;

	int cantidad_Conteos=conteos.size();

	string primer_fecha_Eleccion,fecha_actual_Eleccion=" ";
	if (cantidad_Conteos>0){
		primer_fecha_Eleccion=conteos[0]->getEleccion().getFecha();
		cout<<"Resultados Para Eleccion del "<<primer_fecha_Eleccion<<" por el cargo "<<conteos[0]->getEleccion().getCargo().getCargoPrincipal()<<endl;
		fecha_actual_Eleccion=primer_fecha_Eleccion;
	}


	long total_votos=0;
	long total_votos_eleccion=0;

	for(int i=0;i<cantidad_Conteos;i++){



		if(conteos[i]->getEleccion().getFecha()!=fecha_actual_Eleccion){


			cout<<"		Total Votos en Eleccion del "<<fecha_actual_Eleccion<<": "<<total_votos_eleccion<<endl;
			total_votos_eleccion=0;

			fecha_actual_Eleccion=conteos[i]->getEleccion().getFecha();
			cout<<endl;
			cout<<"Resultados Para Eleccion del "<<fecha_actual_Eleccion<<" por el cargo "<<conteos[i]->getEleccion().getCargo().getCargoPrincipal()<<endl;

		}

		Lista lista=conteos[i]->getLista();

		cout<<"		Lista: "<<lista.getNombre()<<". . . . . Votos: "<<conteos[i]->getVotos()<<endl;

		total_votos_eleccion+=conteos[i]->getVotos();
		total_votos+=conteos[i]->getVotos();
	}

	cout<<"Total de votos en Distrito: "<<total_votos<<endl;
}

///////////////////////

Informe::Informe(Distrito& distrito, string rutaInforme) {

        ofstream archInforme(rutaInforme.c_str(), ios::trunc);

	vector<Conteo *> conteos;

	DataGetter::getConteosPorDistrito(conteos, distrito);

	archInforme <<"Informe para Distito: "<<distrito.getNombre()<<endl;

	int cantidad_Conteos=conteos.size();

	string primer_fecha_Eleccion,fecha_actual_Eleccion=" ";
	if (cantidad_Conteos>0){
		primer_fecha_Eleccion=conteos[0]->getEleccion().getFecha();
		archInforme <<"Resultados Para Eleccion del "<<primer_fecha_Eleccion<<" por el cargo "<<conteos[0]->getEleccion().getCargo().getCargoPrincipal()<<endl;
		fecha_actual_Eleccion=primer_fecha_Eleccion;
	}


	long total_votos=0;
	long total_votos_eleccion=0;

	for(int i=0;i<cantidad_Conteos;i++){



		if(conteos[i]->getEleccion().getFecha()!=fecha_actual_Eleccion){


			archInforme <<"		Total Votos en Eleccion del "<<fecha_actual_Eleccion<<": "<<total_votos_eleccion<<endl;
			total_votos_eleccion=0;

			fecha_actual_Eleccion=conteos[i]->getEleccion().getFecha();
			archInforme <<endl;
			archInforme <<"Resultados Para Eleccion del "<<fecha_actual_Eleccion<<" por el cargo "<<conteos[i]->getEleccion().getCargo().getCargoPrincipal()<<endl;

		}

		Lista lista=conteos[i]->getLista();

		archInforme <<"		Lista: "<<lista.getNombre()<<". . . . . Votos: "<<conteos[i]->getVotos()<<endl;

		total_votos_eleccion+=conteos[i]->getVotos();
		total_votos+=conteos[i]->getVotos();
	}

	archInforme <<"Total de votos en Distrito: "<<total_votos<<endl;
}

///////////////////////

Informe::Informe(Lista& lista){

	Eleccion eleccion=lista.getEleccion();

	long total_votos=0;

	vector<Conteo *> conteos;

	DataGetter::getConteosPorLista(conteos, lista);
	vector<Conteo *>::size_type cantidad_Conteos = conteos.size();


	cout<<"Resultados Para Lista "<<lista.getNombre()<<endl;


	for(string::size_type i = 0; i < cantidad_Conteos; i++)
	{

		Distrito distrito=conteos[i]->getDistrito();

		cout<<"		"<<distrito.getNombre()<<". . . . . Votos: "<<conteos[i]->getVotos()<<endl;

		total_votos+=conteos[i]->getVotos();

	}
	cout<<"Total de votos: "<<total_votos<<endl<<endl;

}

////////////////////

Informe::Informe(Lista& lista, string rutaInforme){

        ofstream archInforme(rutaInforme.c_str(), ios::trunc);

	long total_votos=0;

	vector<Conteo *> conteos;

	DataGetter::getConteosPorLista(conteos, lista);
	vector<Conteo *>::size_type cantidad_Conteos = conteos.size();


	archInforme <<"Resultados Para Lista "<<lista.getNombre()<<endl;


	for(string::size_type i = 0; i < cantidad_Conteos; i++)
	{

		Distrito distrito=conteos[i]->getDistrito();

		archInforme <<"		"<<distrito.getNombre()<<". . . . . Votos: "<<conteos[i]->getVotos()<<endl;

		total_votos+=conteos[i]->getVotos();

	}
	archInforme <<"Total de votos: "<<total_votos<<endl<<endl;

}

////////////////////

Informe::~Informe(){
    
}