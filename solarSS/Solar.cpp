
#include "Solar.h"   
#include <stdlib.h> 
#include "GL/glut.h"
#include "planeta.h"


static GLenum spinMode = GL_TRUE;
static GLenum singleStep = GL_FALSE;
const int numeroPlanetas = 8;
Planeta sistemasolar[numeroPlanetas];
int MoveX = 0;
int MoveY = 0;

static void KeyPressFunc(unsigned char Key, int x, int y)
{
	switch (Key) {//isto das teclas fui buscar a net, se quiseres tira,tao a funciunar
	case 'R':
	case 'r':
		Key_r();
		break;
	case 's':
	case 'S':
		Key_s();
		break;
	case 27:	// tecla esc
		exit(1);
	}
}


static void SpecialKeyFunc(int Key, int x, int y)
{
	switch (Key) {
	case GLUT_KEY_UP:
		Key_up();
		break;
	case GLUT_KEY_DOWN:
		Key_down();
		break;
	}
}


static void Key_r(void)
{
	if (singleStep) {
		singleStep = GL_FALSE;
		spinMode = GL_TRUE;		// da restard a animacao
	}
	else {
		spinMode = !spinMode;	// liga e desliga a animaçao
	}
}

static void Key_s(void)
{
	singleStep = GL_TRUE;
	spinMode = GL_TRUE;
}

static void Key_up(void)
{
	
}

static void Key_down(void)
{
	

}
static void initLights(void)
{
	// Define a luz ambiente global
	GLfloat global_ambient[] = { 0.01f, 0.01f, 0.01f, 1.0f };
	// Define a luz light0. Existem 8 fontes de luz no total.
	GLfloat light0_ambient[] = { 0.02f, 0.02f, 0.02f, 1.0f };
	GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Fonte de luz ambiente
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	// Fonte de luz posicional
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.005);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.005);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005);

	glEnable(GL_LIGHT0);


}

void applyLights(){
	// Define a posição de light0
	GLfloat light0_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Aplica a light0
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	// Desenha uma esfera que sinaliza a posição da light0
	/*glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glutSolidSphere(1.9, 20, 20);
	glPopMatrix();*/

	glEnable(GL_LIGHTING);
}

void applymaterial(int type)
{
	// Define as propriedades dos materiais
	// Type: 0 (Branco); 1 (Amarelo); 2 (Ciano); 3 (Branco-Emissor)
	GLfloat mat_ambient[4][4] = { { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } };
	GLfloat mat_diffuse[4][4] = { { 0.5f, 0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, 0.0f, 1.0f }, { 0.0f, 0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f } };
	GLfloat mat_specular[4][4] = { { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } };
	GLfloat mat_emission[4][4] = { { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } };
	GLfloat mat_shininess[4][1] = { { 20.0f }, { 20.0f }, { 20.0f }, { 20.0f } };

	if ((type >= 0) && (type < 4))
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient[type]); // GL_FRONT, GL_FRONT_AND_BACK , GL_BACK, 
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse[type]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular[type]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission[type]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[type]);
	}
}


void UpdatePlanetas(){
	for (int i = 0; i < numeroPlanetas; i++){
		sistemasolar[i].Update();
	}
}

void DrawPlanetas(){
	for (int i = numeroPlanetas; i > -1; i--){

		glPushMatrix();

		glTranslatef(
			sistemasolar[i].GetX(), 
			0.0,
			sistemasolar[i].GetZ());
		//Se é o sol, material emissive
		if (i == 0){
			applymaterial(3);
		}
		else{
			applymaterial(0);
		}
		glutSolidSphere(sistemasolar[i].GetRaioPlaneta(), 64, 64);

		glPopMatrix();
	}
}

//isto pode ou nao ser alterado
static void Animate(void)
{
	// limpa a janela
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	
	// reduz 8 unidades para ver da origem
	glTranslatef(0.0, 0.0, -30.0);
	glRotatef(25.0, 1.0, 0.0, 0.0);

	if (spinMode) {
		//Atualiza a posição dos planetas
		UpdatePlanetas();
	}

	applyLights();

	//Desenha os planetas
	DrawPlanetas();

	glFlush();
	glutSwapBuffers();

	if (singleStep) {
		spinMode = GL_TRUE;
	}

	glutPostRedisplay();
	// pede um redraw para um proposito da animaçao

}

// Initialize OpenGL's rendering modes
void OpenGLInit(void)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL); // GL_LINE, GL_POINT, GL_FILL

	// Activa o teste de profundidade
	glEnable(GL_DEPTH_TEST);
}


static void ResizeWindow(int w, int h)
{
	float aspectRatio;
	h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;
	glViewport(0, 0, w, h);	// View port usa uma janela inteira
	aspectRatio = (float)w / (float)h;

	// prepara a vista da matrix (nao muito bem!)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspectRatio, 1, 300);

	// seleciona o modelo de vista da matrix
	glMatrixMode(GL_MODELVIEW);
}
void initSistemaSolar()
{

	Planeta sol;
	sol.SetValues(0, 2, 1.0);
	sistemasolar[0] = sol;

	Planeta mercurio;
	mercurio.SetValues(3.5, 0.2, 1.0);
	sistemasolar[1] = mercurio;

	Planeta venus;
	venus.SetValues(5, 0.4, 1.0);
	sistemasolar[2] = venus;

	Planeta terra;
	terra.SetValues(9, 1.0, 1.0);
	sistemasolar[3] = terra;

	Planeta marte;
	marte.SetValues(12, 0.8, 1.0);
	sistemasolar[4] = marte;

	Planeta jupiter;
	jupiter.SetValues(17, 1.5, 1.0);
	sistemasolar[5] = jupiter;

	Planeta saturno;
	saturno.SetValues(25, 1.2, 1.0);
	sistemasolar[6] = saturno;

	Planeta urano;
	urano.SetValues(30, 1.1, 1.0);
	sistemasolar[7] = urano;

	Planeta neptuno;
	neptuno.SetValues(17, 1.0, 1.0);
	sistemasolar[8] = neptuno;

}

//rota principal
int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_RGBA);


	glutInitWindowPosition(0, 0);
	glutCreateWindow("Sistema Solar");
	//glutInitWindowSize(1080, 1080);
	glutFullScreen();


	OpenGLInit();
	initSistemaSolar();
	initLights();

	glutKeyboardFunc(KeyPressFunc);
	glutSpecialFunc(SpecialKeyFunc);
	glutReshapeFunc(ResizeWindow);
	glutDisplayFunc(Animate);
	glutMainLoop();

	return(0);
}
