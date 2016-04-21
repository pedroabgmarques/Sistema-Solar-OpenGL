#include "lua.h"
#include <math.h>
#include <string>
#include <iostream>

// Specify default namespace for commonly used elements
using std::string;
using std::cout;
using std::endl;

void Lua::SetValues(float distanciaSol, float raioLua, float rotacao, float periodoOrbital, GLuint textura, Planeta planeta)
{
	_raioOrbita = distanciaSol;
	_raioLua = raioLua;
	_rotacao = rotacao;
	_PeriodoOrbital = periodoOrbital;
	_rotacaoAtual = 0;
	_textura = textura;
	_planeta = planeta;
	x, y, z = 0;
	iterator = 0;

}
void Lua::Draw(GLUquadric *mysolid)
{

	glBindTexture(GL_TEXTURE_2D, _textura);

	
	glTranslatef(
		x ,
		0.0 ,
		z );

	//Rotação sobre si próprio
	//glRotatef(_rotacaoAtual, 0.0, 1.0, 0.0);

	//Endireitar os planetas
	//glRotatef(-90.0, 1.0, 0.0, 0.0);

	gluSphere(mysolid, _raioLua, 64, 64);

}

void Lua::Update(float simulationSpeed, Vec3<float> posisaoPlaneta){

	//Cálculo do movimento de trabslação do planeta em volta do sol
	x = posisaoPlaneta.getX() + _raioOrbita * sin(iterator / 180.0 * 3.14);
	z = posisaoPlaneta.getZ() + _raioOrbita * cos(iterator / 180.0 * 3.14);

	std::cout << "Posicao do planeta: " << posisaoPlaneta.getX() << "\t" << std::endl;

	//Esta linha define a velocidade orbital
	iterator += 3.14 * 2 / (simulationSpeed * _PeriodoOrbital);
	if (iterator >= 360)
	{
		//Demos uma volta completa ao planeta (um ano)
		iterator = 0;
	}

	//Cálculo do movimento de rotação em torno do seu eixo
	//rotacaoAtual += 3.14 * 2 / (simulationSpeed * _rotacao);


}

Planeta Lua::GetPlaneta(){
	return _planeta;
}

float Lua::GetRaioLua(){
	return _raioLua
;
}
float Lua::GetRaioOrbita(){
	return _raioOrbita;
}


float Lua::GetX(){
	return x;
}

float Lua::GetY(){
	return y;
}

float Lua::GetZ(){
	return z;
}
