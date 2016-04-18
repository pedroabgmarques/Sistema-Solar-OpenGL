
#include "Solar.h"   
#include <stdlib.h> 
#include <GL/glut.h>	
#include "planeta.h"


static GLenum spinMode = GL_TRUE;
static GLenum singleStep = GL_FALSE;
const int numeroPlanetas = 8;
Planeta sistemasolar[numeroPlanetas];
int MoveX = 0;
int MoveY = 0;




// Controlo do estado e velocidade das animacoes
static float HourOfDay = 0.0;
static float DayOfYear = 0.0;
static float HourOfDay2 = 0.0;
static float DayOfYear2 = 0.0;
static float HourOfDay3 = 0.0;
static float DayOfYear3 = 0.0;
static float HourOfDay4 = 0.0;
static float DayOfYear4 = 0.0;
static float HourOfDay5 = 0.0;
static float DayOfYear5 = 0.0;
static float HourOfDay6 = 0.0;
static float DayOfYear6 = 0.0;
static float HourOfDay7 = 0.0;
static float DayOfYear7 = 0.0;
static float HourOfDay8 = 0.0;
static float DayOfYear8 = 0.0;

static float AnimateIncrement = 24.0;  // tempo para animacoes (horas)
static void initLights(void);

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
	AnimateIncrement *= 1.5;			// duplica a velocidade da animaçao
}

static void Key_down(void)
{
	AnimateIncrement /= 2.0;			// reduz a velocidade da animaçao

}
static void initLights(void)
{
	// Define a luz ambiente global
	GLfloat global_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	// Define a luz light0. Existem 8 fontes de luz no total.
	GLfloat light0_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light0_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// Define a luz light1. Existem 8 fontes de luz no total.
	GLfloat light1_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat light1_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light1_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat spot_angle = 45.0f;
	GLfloat spot_exp = 12.0f; // Maior valor = maior concentração de luz no centro

	// Fonte de luz ambiente
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	// Fonte de luz posicional
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.05);

	// Fonte de luz cónica
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_angle);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, spot_exp);

	// Activa a utilização de iluminação
	glEnable(GL_LIGHTING);
	// Activa a fonte de luz light0
	glEnable(GL_LIGHT0);
	// Activa a fonte de luz light1
	glEnable(GL_LIGHT1);
}
//isto pode ou nao ser alterado
static void Animate(void)
{
	// limpa a janela
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (spinMode) {
		// update da animaçao
		HourOfDay += AnimateIncrement;
		DayOfYear += AnimateIncrement / 30.0;

		HourOfDay = HourOfDay - ((int)(HourOfDay / 24)) * 24;
		DayOfYear = DayOfYear - ((int)(DayOfYear / 365)) * 365;

		HourOfDay2 = HourOfDay2 - ((int)(HourOfDay2 / 36)) * 36;
		DayOfYear2 = DayOfYear2 - ((int)(DayOfYear2 / 400)) * 400;

		HourOfDay3 = HourOfDay3 - ((int)(HourOfDay3 / 36)) * 36;
		DayOfYear3 = DayOfYear3 - ((int)(DayOfYear3 / 400)) * 400;

		HourOfDay4 = HourOfDay4 - ((int)(HourOfDay4 / 36)) * 36;
		DayOfYear4 = DayOfYear4 - ((int)(DayOfYear4 / 400)) * 400;

		HourOfDay5 = HourOfDay5 - ((int)(HourOfDay5 / 36)) * 36;
		DayOfYear5 = DayOfYear5 - ((int)(DayOfYear5 / 400)) * 400;

		HourOfDay6 = HourOfDay6 - ((int)(HourOfDay6 / 36)) * 36;
		DayOfYear6 = DayOfYear6 - ((int)(DayOfYear6 / 400)) * 400;

		HourOfDay7 = HourOfDay7 - ((int)(HourOfDay7 / 36)) * 36;
		DayOfYear7 = DayOfYear7 - ((int)(DayOfYear7 / 400)) * 400;

		HourOfDay8 = HourOfDay8 - ((int)(HourOfDay8 / 36)) * 36;
		DayOfYear8 = DayOfYear8 - ((int)(DayOfYear8 / 400)) * 400;

	}

	glLoadIdentity();
	/*
	// reduz 8 unidades para ver da origem
	glTranslatef(0.0, 0.0, -10.0);

	//roda o plano epiliticamente
	// (roda sobreo x do modelo em 15 graus)
	glRotatef(225.0, 1.0, 0.0, 0.0);


	
	// desenhar sol	-- amarelo em wireframe
	glRotatef(360.0*DayOfYear / 365.0, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(2.5, 15, 15);


	glPopMatrix();



	// desenha terra
	//		usa o dia do ano para determinar a sua posicao
	glRotatef(360.0*DayOfYear2 / 365.0, 1.0, 1.0, 0.0);
	glTranslatef(3.0, 0.0, 0.0);
	glPushMatrix();
	// roda a terra sobre o seu eixo
	//		usa a hora do dia para determinar a sua posicao
	glRotatef(24.0*HourOfDay2 / 24.0, 0.0, 1.0, 0.0);
	// desenha a terra numa esferea em wireframe
	glColor3f(0.2, 0.2, 1.0);
	glutWireSphere(0.2, 20, 20);

	glPopMatrix();

	glRotatef(480 * DayOfYear8 / 480.0, 1.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(36 * HourOfDay8 / 36.0, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.2, 1.0);
	glutWireSphere(0.2, 20, 20);

	glPopMatrix();


	//2 planeta
	glRotatef(480 * DayOfYear3 / 480.0, 1.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(36 * HourOfDay3 / 36.0, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.2, 1.0);
	glutWireSphere(0.2, 20, 20);

	glPopMatrix();
	//3 planeta
	glRotatef(480 * DayOfYear4 / 480.0, 1.0, 1.0, 0.0);
	glTranslatef(3.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(36 * HourOfDay4 / 36.0, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.2, 1.0);
	glutWireSphere(0.2, 10, 10);

	glPopMatrix();

	//4 planeta
	glRotatef(480 * DayOfYear5 / 480.0, 1.0, 1.0, 0.0);
	glTranslatef(4.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(36 * HourOfDay5 / 36.0, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.2, 1.0);
	glutWireSphere(0.2, 10, 10);

	glPopMatrix();

	//5 planeta
	glRotatef(480 * DayOfYear6 / 480.0, 1.0, 1.0, 0.0);
	glTranslatef(5.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(36 * HourOfDay6 / 36.0, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.2, 1.0);
	glutWireSphere(0.2, 10, 10);

	glPopMatrix();


	//6 planeta
	glRotatef(480 * DayOfYear7 / 480.0, 1.0, 1.0, 0.0);
	glTranslatef(6.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(36 * HourOfDay7 / 36.0, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.2, 1.0);
	glutWireSphere(0.2, 10, 10);

	glPopMatrix();

	/*
	glColor3f(0.0, 1.0, 0.5);
	gluLookAt(0.0, 10.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	if (MoveY == 720)
		MoveY = 0;
	glTranslatef(-4.0, 0.0, 0.0);
	glRotatef(36 * HourOfDay7 / 36.0, 0.0, 1.0, 0.0);
	glutWireSphere(0.2, 10, 10);
	int i = 0;
	glBegin(GL_LINE_STRIP);
	//glBegin(GL_QUAD_STRIP);
	glColor3f(0.0, 1.0, 0.7);
	for (i = 0; i <= 360; i++)
	{
		glVertex3f(sin(i*3.1416 / 180) * 1, cos(i*3.1416 / 180) * 1, 0);
		glVertex3f(sin(i*3.1416 / 180)*1.2, cos(i*3.1416 / 180)*1.2, 0);
	}
	
	glPopMatrix();

	*/

	
	

	//desenha  a lua
	//usa a hora do dia para determinar a sua posicao

	glRotatef(360.0*12.0*DayOfYear / 365.0, 0.0, 1.0, 0.0);
	glTranslatef(0.7, 0.0, 0.0);
	glColor3f(0.3, 0.7, 0.3);
	glutWireSphere(0.05, 5, 5);

	glPopMatrix();
	// troca os buffers
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
	glShadeModel(GL_CLAMP);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_RANGE);
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
	gluPerspective(100.0, aspectRatio, 1, 45.0);

	// seleciona o modelo de vista da matrix
	glMatrixMode(GL_MODELVIEW);
}
void initSistemaSolar()
{

	Planeta terra;
	terra.SetValues(0.1, 1.0, 1.0);
	sistemasolar[0] = terra;

	Planeta mercurio;
	mercurio.SetValues(0.1, 1.0, 1.0);
	sistemasolar[1] = mercurio;
	
	Planeta venus;
	venus.SetValues(0.1, 1.0, 1.0);
	sistemasolar[2] = venus;

	Planeta marte;
	marte.SetValues(0.1, 1.0, 1.0);
	sistemasolar[3] = marte;

	Planeta jupiter;
	jupiter.SetValues(0.1, 1.0, 1.0);
	sistemasolar[4] = jupiter;

	Planeta saturno;
	saturno.SetValues(0.1, 1.0, 1.0);
	sistemasolar[5] = saturno;

	Planeta urano;
	urano.SetValues(0.1, 1.0, 1.0);
	sistemasolar[6] = urano;

	Planeta neptuno;
	neptuno.SetValues(0.1, 1.0, 1.0);
	sistemasolar[7] = neptuno;

	Planeta mercurio;
	mercurio.SetValues(0.1, 1.0, 1.0);
	sistemasolar[7] = mercurio;
	//depois acabar

}

//rota principal
int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);


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
