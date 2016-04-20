#include "planeta.h"
#include "GL/glut.h"
#include <math.h>

void Planeta::SetValues(float distanciaSol, float raioPlaneta, float rotacao,float periodoOrbital, GLuint textura)
{
	_raioOrbita = distanciaSol;
	_raioPlaneta = raioPlaneta;
	_rotacao = rotacao;
	_PeriodoOrbital = periodoOrbital;
	_rotacaoAtual = 0;
	_textura = textura;
	x, y, z = 0;
	iterator = 0;

}
void Planeta::Draw(GLUquadric *mysolid)
{
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, _textura);

	glTranslatef(
		x,
		0.0,
		z);

	//Rotação sobre si próprio
	glRotatef(_rotacaoAtual, 0.0, 1.0, 0.0);

	//Endireitar os planetas
	glRotatef(-90.0, 1.0, 0.0, 0.0);

	gluSphere(mysolid, _raioPlaneta, 64, 64);

	glPopMatrix();
}

void Planeta::Update(){
	//Cálculo do movimento de trabslação do planeta em volta do sol
	x = sin(iterator) * _raioOrbita;
	z = cos(iterator) * _raioOrbita;

	//Esta linha define a velocidade orbital
	iterator += 3.14*2 / (600 *_PeriodoOrbital);
	if (iterator >= 3.14 * 2) 
	{
		//Demos uma volta completa ao sol (um ano)
		iterator = 0;
	}

	//Cálculo do movimento de rotação em torno do seu eixo
	_rotacaoAtual += _rotacao * 32;
	if (_rotacaoAtual > 360)
	{
		//Demos uma volta completa (um dia)
		_rotacaoAtual = _rotacao - 360;
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
