#include "planeta.h"
#include "GL/glut.h"
#include <math.h>

void Planeta::SetValues(float distanciaSol, float raioPlaneta, float rotacao, GLuint textura)
{
	_raioOrbita = distanciaSol;
	_raioPlaneta = raioPlaneta;
	_rotacao = rotacao;
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
	x = sin(iterator) * _raioOrbita;
	z = cos(iterator) * _raioOrbita;
	iterator += 3.14 / (45 * _raioOrbita);
	if (iterator >= 3.14 * 2) 
	{
		iterator = 0;
	}
	_rotacaoAtual += _rotacao * 32;
	if (_rotacaoAtual > 360)
	{
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