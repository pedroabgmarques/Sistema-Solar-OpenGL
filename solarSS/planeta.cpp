#include "planeta.h"
#include <math.h>
#include <string>
#include <iostream>

// Specify default namespace for commonly used elements
using std::string;
using std::cout;
using std::endl;

void Planeta::SetValues(float distanciaSol, float raioPlaneta, float rotacao,float periodoOrbital, GLuint textura)
{
	_raioOrbita = distanciaSol;
	_raioPlaneta = raioPlaneta;
	_rotacao = rotacao;
	_PeriodoOrbital = periodoOrbital;
	_rotacaoAtual = 0;
	_textura = textura;
	x = distanciaSol;
	y = 0;
	z = distanciaSol;
	iterator = 0;

}
void Planeta::Draw(GLUquadric *mysolid, bool minimap)
{
	

	glBindTexture(GL_TEXTURE_2D, _textura);

	glTranslatef(
		x,
		0.0,
		z);

	//Rotação sobre si próprio
	glRotatef(_rotacaoAtual, 0.0, 1.0, 0.0);

	//Endireitar os planetas
	glRotatef(-90.0, 1.0, 0.0, 0.0);

	if (minimap){
		glScalef(8.0, 8.0, 8.0);
	}
	gluSphere(mysolid, _raioPlaneta, 64, 64);

}

void Planeta::Update(float simulationSpeed){
	//Cálculo do movimento de trabslação do planeta em volta do sol
	x = _raioOrbita * sin(iterator / 180.0 * 3.14);
	z = _raioOrbita * cos(iterator / 180.0 * 3.14);

	//Esta linha define a velocidade orbital
	iterator += 3.14 * 2 / (simulationSpeed * _PeriodoOrbital);
	if (iterator >= 360 ) 
	{
		//Demos uma volta completa ao sol (um ano)
		iterator = 0;
		if (_raioOrbita == 10){
			//Terra
			std::cout << "Contador de dias num ano: " << contadorDias << "\t" << std::endl;
		}
		
	}

	//Cálculo do movimento de rotação em torno do seu eixo
	_rotacaoAtual += 3.14 * 2 / (simulationSpeed * _rotacao);
	if (_rotacaoAtual > 360){
		_rotacaoAtual = 0;
		contadorDias++;
	}
	
}

float Planeta::GetDistanciaSol(){
	return _raioOrbita;
}

float Planeta::GetRaioPlaneta(){
	return _raioPlaneta;
}

float Planeta::GetX(){
	return x;
}

float Planeta::GetY(){
	return y;
}

float Planeta::GetZ(){
	return z;
}

GLuint Planeta::Gettextura(){
	return _textura;
}
