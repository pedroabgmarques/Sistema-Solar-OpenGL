#include "planeta.h"
#include "GL/glut.h"
#include <math.h>

void Planeta::SetValues(float distanciaSol, float raioPlaneta, GLuint textura)
{
	_raioOrbita = distanciaSol;
	_raioPlaneta = raioPlaneta;
	_textura = textura;
	x, y, z = 0;
	iterator = 0;

}
void Planeta::Draw()
{
	glTranslatef(_raioOrbita, 0.0, 0.0);
	glutWireSphere(_raioPlaneta, 20, 20);
}

void Planeta::Update(){
	x = sin(iterator) * _raioOrbita;
	z = cos(iterator) * _raioOrbita;
	iterator += 3.14 / (45 * _raioOrbita);
	if (iterator >= 3.14 * 2) 
	{
		iterator = 0;
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