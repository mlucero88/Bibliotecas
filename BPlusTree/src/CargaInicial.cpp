/*
 * CargaInicial.cpp
 *
 *  Created on: 19/10/2011
 *      Author: facundo
 */

#include "CargaInicial.h"

CargaInicial::CargaInicial() {

}

CargaInicial::~CargaInicial() {
}

void CargaInicial::ejecutar()
{
	vector<Distrito> distritos;
	vector<Cargo> cargos;
	vector<Eleccion> elecciones;
	vector<Lista> listas;
	vector<Votante *> votantes;
	vector<Candidato *> candidatos;
	int cantidadDistritos;
	int cantidadVotantes = 100;
	int cantidadVotantePorDistrito;
	ABMentidades abm;

	UtilidadesTests::cargarDistritos(distritos);
	for(unsigned int i = 0; i < distritos.size(); i++)
		abm.altaDistrito(distritos[i]);

	UtilidadesTests::cargarCargos(cargos);
	for(unsigned int i = 0; i < cargos.size(); i++)
		abm.altaCargo(cargos[i]);

	UtilidadesTests::cargarElecciones(elecciones, cargos, distritos);
	for(unsigned int i = 0; i < elecciones.size(); i++)
		abm.altaEleccion(elecciones[i]);

	UtilidadesTests::cargarListas(listas, elecciones);
	for(unsigned int i = 0; i < listas.size(); i++)
		abm.altaLista(listas[i]);

	cantidadDistritos = distritos.size();
	cantidadVotantePorDistrito = cantidadVotantes/cantidadDistritos;

	CargaInicial::getVotantes(votantes, distritos);
	for(unsigned int i = 0; i < votantes.size(); i++)
		abm.altaVotante(*votantes[i]);

	for(unsigned int i = 0; i < listas.size(); i++)
		candidatos.push_back(new Candidato(*votantes[i], listas[i]));

	for(unsigned int i = 0; i < candidatos.size(); i++)
		abm.altaCandidato(*candidatos[i]);

	for(unsigned int i = 0; i < candidatos.size(); i++)
		delete candidatos[i];

	//for(unsigned int i = 0; i < votantes.size(); i++)
		//delete votantes[i];
}

void CargaInicial::getVotantes(vector<Votante *> &votantes)
{
	int dni = dniInicial;
	ConsultaEntidades consulta;
	Distrito distrito;
	Votante * votante;

	for(int i = 0; i < cantidadVotantes; i++, dni++){
		votante = new Votante(dni, "", "", "", distrito);
		consulta.ObtenerRegistro(Utilidades::toString(dni), *votante);
		votantes.push_back(votante);
	}
}

void CargaInicial::getVotantes(vector<Votante *> &votantes, vector<Distrito> &distritos)
{
	int cantidadDistritos;
	int cantidadVotantePorDistrito;

	cantidadDistritos = distritos.size();
	cantidadVotantePorDistrito = cantidadVotantes/cantidadDistritos;

	int h = dniInicial;
	string direccion = "saraza";
	string nombre;
	Votante * votante;

	for(int i = 0; i < cantidadDistritos; i++){
		for(int j = 0; j < cantidadVotantePorDistrito; j++, h++)
		{
			nombre = "Persona"+Utilidades::toString(i);
			votante = new Votante(h, nombre, "tp", direccion, distritos[i]);
			votantes.push_back(votante);
		}
	}
}

vector<string> CargaInicial::getFechasElecciones()
{
	vector<Distrito> distritos;
	vector<Cargo> cargos;
	vector<Eleccion> elecciones;

	vector<string> fechas;

	UtilidadesTests::cargarDistritos(distritos);
	UtilidadesTests::cargarCargos(cargos);
	UtilidadesTests::cargarElecciones(elecciones, cargos, distritos);

	for(unsigned int i = 0; i < elecciones.size(); i++)
	{
		fechas.push_back(elecciones[i].getFecha());
	}

	return fechas;
}




