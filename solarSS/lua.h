#include "gl/glut.h"
#include "planeta.h"
#include "Vec3.h"

class Lua
{

	GLuint _textura;
	float _raioOrbita, _raioLua;
	float x, y, z;
	float iterator = 0;
	float _rotacao; 
	float _rotacaoAtual;
	float _PeriodoOrbital;
	int _indicePlaneta;
public:

	void SetValues(float, float, float, float, GLuint,int);
	void Draw(GLUquadric*);
	void Update(float, Vec3<float>);
	float GetRaioLua();
	float GetX();
	float GetY();
	float GetZ();
	float GetRaioOrbita();
	int GetPlaneta();



};