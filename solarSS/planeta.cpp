#include "planeta.h"
#include <GL/glut.h>	

void Planeta::SetValues(float distanciaSol, float raioPlaneta, GLuint textura)
{
	_distanciaSol = distanciaSol;
	_raioPlaneta = raioPlaneta;
	_textura = textura;

}
void Planeta::Draw()
{
	glTranslatef(_distanciaSol, 0.0, 0.0);
	glutWireSphere(_raioPlaneta, 20, 20);
}