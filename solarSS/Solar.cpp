#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include "Solar.h"   
#include <stdlib.h> 
#include <stdio.h>
#include "GL/glut.h"
#include "planeta.h"
#include "tga.h"
#include <string>
#include <vector>


static GLenum spinMode = GL_TRUE;
static GLenum singleStep = GL_FALSE;
const int numeroPlanetas = 8;
Planeta sistemasolar[numeroPlanetas];
int MoveX = 0;
int MoveY = 0;
GLuint textures[numeroPlanetas];
tgaInfo *im;
GLUquadric *mysolid;

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
	GLfloat light0_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Fonte de luz ambiente
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	// Fonte de luz posicional
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0025);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0025);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0025);

	glEnable(GL_LIGHT0);


}

void applyLights(){
	// Define a posição de light0
	GLfloat light0_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Aplica a light0
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

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

void draworbit(float x, float y, float z, GLint radius)
{
	//glDisable(GL_LIGHTING);
	glBegin(GL_LINE_LOOP);

	for (float i = 0; i<(3.14 * 2); i += 3.14 / 360)

	{
		x = sin(i)*radius;
		z = cos(i)*radius;
		glVertex3f(x, 0, z);
	}

	glEnd();
	//glEnable(GL_LIGHTING);
}

void DrawPlanetas(){
	for (int i = numeroPlanetas; i > -1; i--){

		//Se é o sol, material emissive
		if (i == 0){
			applymaterial(3);
		}
		else{
			applymaterial(0);
		}

		sistemasolar[i].Draw(mysolid);

		draworbit(
			sistemasolar[i].GetX(), 
			sistemasolar[i].GetY(), 
			sistemasolar[i].GetZ(), 
			sistemasolar[i].GetDistanciaSol()
		);
		
	}
}

//isto pode ou nao ser alterado
static void Animate(void)
{
	// limpa a janela
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	
	//Puxa a camara para trás
	glTranslatef(0.0, 0.0, -30.0);
	//inclinação da camara
	glRotatef(25.0, 1.0, 0.0, 0.0);

	if (spinMode) {
		//Atualiza a posição dos planetas
		UpdatePlanetas();
	}

	glEnable(GL_TEXTURE_2D);

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

void load_tga_image(std::string nome, GLuint texture)
{
	std::string impathfile = "textures/" + nome + ".tga";

	std::vector<char> writable(impathfile.begin(), impathfile.end());
	writable.push_back('\0');

	// Carrega a imagem de textura
	im = tgaLoad(&writable[0]);

	//printf("IMAGE INFO: %s\nstatus: %d\ntype: %d\npixelDepth: %d\nsize%d x %d\n", impathfile, im->status, im->type, im->pixelDepth, im->width, im->height);

	// select our current texture
	glBindTexture(GL_TEXTURE_2D, texture);

	//	// set up quadric object and turn on FILL draw style for it
	mysolid = gluNewQuadric();
	gluQuadricDrawStyle(mysolid, GLU_FILL);

	//	// turn on texture coordinate generator for the quadric
	gluQuadricTexture(mysolid, GL_TRUE);

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // MIPMAP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// build our texture mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, im->width, im->height, GL_RGB, GL_UNSIGNED_BYTE, im->imageData); // MIPMAP
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, im->width, im->height, 0, GL_RGB, GL_UNSIGNED_BYTE, im->imageData);

	// Destroi a imagem
	tgaDestroy(im);
}

void initSistemaSolar()
{

	Planeta sol;
	sol.SetValues(0, 2, 0.01, textures[0]);
	load_tga_image("sun", textures[0]);
	sistemasolar[0] = sol;

	Planeta mercurio;
	mercurio.SetValues(4, 0.4, 0.03, textures[1]);
	load_tga_image("mercury", textures[1]);
	sistemasolar[1] = mercurio;

	Planeta venus;
	venus.SetValues(6, 0.8, 0.07, textures[2]);
	load_tga_image("venus", textures[2]);
	sistemasolar[2] = venus;

	Planeta terra;
	terra.SetValues(9, 1.0, 0.05, textures[3]);
	load_tga_image("earth", textures[3]);
	sistemasolar[3] = terra;

	Planeta marte;
	marte.SetValues(12, 0.8, 0.4, textures[4]);
	load_tga_image("mars", textures[4]);
	sistemasolar[4] = marte;

	Planeta jupiter;
	jupiter.SetValues(17, 1.5, 0.1, textures[5]);
	load_tga_image("jupiter", textures[5]);
	sistemasolar[5] = jupiter;

	Planeta saturno;
	saturno.SetValues(25, 1.2, 0.1, textures[6]);
	load_tga_image("saturn", textures[6]);
	sistemasolar[6] = saturno;

	Planeta urano;
	urano.SetValues(30, 1.1, 0.2, textures[7]);
	load_tga_image("uranus", textures[7]);
	sistemasolar[7] = urano;

	Planeta neptuno;
	neptuno.SetValues(17, 1.0, 0.2, textures[8]);
	load_tga_image("neptune", textures[8]);
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
	
	glGenTextures(numeroPlanetas, textures);
	initSistemaSolar();
	initLights();

	glutKeyboardFunc(KeyPressFunc);
	glutSpecialFunc(SpecialKeyFunc);
	glutReshapeFunc(ResizeWindow);
	glutDisplayFunc(Animate);
	glutMainLoop();

	return(0);
}
