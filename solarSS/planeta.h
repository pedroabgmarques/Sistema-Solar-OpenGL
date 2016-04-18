#include "GL/glut.h"
using namespace std;

class Planeta
{

	GLuint _textura;
	float _raioOrbita, _raioPlaneta;
	float x, y, z;
	float iterator = 0;

public:

	void SetValues(float, float, GLuint);
	void Draw();
	void Update();
	float GetDistanciaSol();
	float GetRaioPlaneta();
	float GetX();
	float GetY();
	float GetZ();



};

