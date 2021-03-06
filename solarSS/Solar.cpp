#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#ifndef GL_MULTISAMPLE_ARB
#define GL_MULTISAMPLE_ARB  0x809D
#endif
#define PI 3.14159265
#define GL_CLAMP_TO_EDGE 0x812F

#include "Solar.h"   
#include <stdlib.h> 
#include <stdio.h>
#include "gl/glfw.h"
#include "lua.h"
#include "tga.h"
#include <string>
#include <vector>
#include "Camera.h"
#include "FPSManager.hpp"
#include "Vec3.h"
#include "glext.h"

#pragma region Global Variables

//Constantes para definição de erros do GLFW
const int GLFW_INIT_ERROR = -1;
const int GLFW_WINDOW_ERROR = -2;

//Largura, altura e posição central da janela
int windowWidth, windowHeight;
GLint midWindowX, midWindowY;

//Opções, controladas pelo teclado
GLenum spinMode = GL_TRUE;
GLenum drawOrbits = GL_TRUE;
float simulationSpeed = 1000; //Bigger values are slower
float simulationSpeedOriginal = simulationSpeed;
bool holdingMoreSimulationSpeed = false;
bool holdingLessSimulationSpeed = false;
float simulationSpeedChangeAcceleration = 0.1;
float simulationSpeedChangeAccelerationOriginal = simulationSpeedChangeAcceleration;
bool gameMode = true;
bool changeGameModeActive = false;

//Largura e altura da simulação em modo windowed
int width, height;

//Fornece informações sobre as capacidades do computador
GLFWvidmode desktop;

//Variáveis auxiliares para a camara orbital
bool arcCam = false;
float arcCamAngle = 0;
float arcCamX = 0;
float arcCamY = 0;
float arcCamZ = 0;
float arcCamRadius = 0;

//Numero de estrelas a desenhar
const int numeroEstrelas = 25000;

//Define uma estrela com uma determinada posição e luminosidade
struct ESTRELA
{
	Vec3<float> posicao;
	float luminosity;
};

//Array de estrelas com a dimensão numeroEstrelas
ESTRELA estrelas[numeroEstrelas];

//Display Lists
GLuint displayListIndex;

//Textura dos anéis
GLuint texturaAneis;

//Instância da câmara
Camera *cam;

//Titulo da janela da simulação
string windowTitle = "Solar System Model | PMarques / VGomes | April 2016";

/*Instância de FPS Manager: tenta manter a simulação a correr a 60fps e atualiza o titulo da janela com os FPS
a cada 3 segundos*/
FpsManager fpsManager(60.0, 3.0, windowTitle);

//Numero de planetas (o Sol conta como planeta)
const int numeroPlanetas = 9;

//Array de planetas - sistema solar
Planeta sistemasolar[numeroPlanetas];

//Array de texturas dos planetas
GLuint textures[numeroPlanetas];

//Variáveis reutilizada no metodo load_tga_image para fazer o load das texturas
tgaInfo *im;
GLUquadric *mysolid;

//Numero de luas
const int numeroLuas = 3;

//Array de luas
Lua luas[numeroLuas];

//Array de texturas de luas
GLuint texturasLua[numeroLuas];

//Variável reutilizada para fazer o carregamento das texturas da skybox
tgaInfo *im2[6];
//Array de texturas da skybox
GLuint skyboxtextures[6];

#pragma endregion

#pragma region Method Signatures
int changeWindowmode();
void handleKeypress(int, int);
void AppStart();
void draworbit(float, float, float, GLint);
void load_tga_image(std::string, GLuint, bool);
#pragma endregion

#pragma region Initialization and Loading
void init(void)
{
	//Smooth shading
	glShadeModel(GL_SMOOTH);

	//Activa o teste de profundidade
	glEnable(GL_DEPTH_TEST);

	//Define shading e correção de perspetiva de alta qualidade
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Estas linhas impediam o openGL de desenhar as back faces, mas
	//não pode ficar por causa de problemas com a skybox que já não temos tempo de resolver
	//(quads desenhados ao contrário)
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/

	//Antialising das linhas das órbitas
	glEnable(GL_MULTISAMPLE_ARB);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwDisable(GLFW_MOUSE_CURSOR);   // Hide the mouse cursor
	glfwSwapInterval(0);              // Disable vsync

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL); // GL_LINE, GL_POINT, GL_FILL
}

void initLights(void)
{
	//Define a luz ambiente global
	GLfloat global_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	//Define a luz light0
	GLfloat light0_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Fonte de luz ambiente
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	// Fonte de luz posicional - na posicao do sol
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0025 /2);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.00001);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0025/2);
	glEnable(GL_LIGHT0);


}

void initPlanetOrbits(){
	//Cria uma display list com as órbitas dos planetas
	glNewList(displayListIndex + 1, GL_COMPILE);
	for (int i = 0; i < numeroPlanetas; i++){
		glBindTexture(GL_TEXTURE_2D, sistemasolar[i].Gettextura());
		draworbit(
			sistemasolar[0].GetX(),
			sistemasolar[0].GetY(),
			sistemasolar[0].GetZ(),
			sistemasolar[i].GetDistanciaSol()
			);
	}
	glEndList();
}

void initEstrelas(){

	//Gerar um determinado numero de estrelas com propriedades aleatórias entre min e max
	int max = 1000;
	int min = -1000;

	for (int i = 0; i < numeroEstrelas; i++){
		estrelas[i].luminosity = ((double)rand() / (RAND_MAX));
		estrelas[i].posicao = Vec3<float>(rand() % (max - min) + min, rand() % (max - min) + min, rand() % (max - min) + min);
	}

	//Criar uma displaylist com as estrelas
	glNewList(displayListIndex, GL_COMPILE);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);
	for (int i = 1; i < numeroEstrelas; i++)
	{
		glColor3f(estrelas[i].luminosity, estrelas[i].luminosity, estrelas[i].luminosity);
		glVertex3f(estrelas[i].posicao.getX(), estrelas[i].posicao.getY(), estrelas[i].posicao.getZ());
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_FOG);

	glEndList();
}

void initSistemaSolar()
{

	//Parametros do método SetValues:
	//Distancia ao sol / raio da órbita
	//Raio do planeta
	//Velocidade de rotação
	//Velocidade de periodo orbital
	//Textura

	/*A escala dos planetas "pequenos" (Mercúrio, Vénus, Terra e Marte) é maior que o dos "grandes",
	por motivos estéticos*/
	float escalapp = 30;
	float escalapg = 10;

	//Gerar os planetas e luas

	Planeta sol;
	sol.SetValues(0, 1, 0.01, 1, textures[0]);
	load_tga_image("sun", textures[0], false);
	sistemasolar[0] = sol;

	Planeta mercurio;
	mercurio.SetValues(4, 0.0035 * escalapp, 0, 1.2, textures[1]);
	load_tga_image("mercury", textures[1], false);
	sistemasolar[1] = mercurio;

	Planeta venus;
	venus.SetValues(7, 0.0087 * escalapp, 0, 1.6, textures[2]);
	load_tga_image("venus", textures[2], false);
	sistemasolar[2] = venus;

	Planeta terra;
	terra.SetValues(10, 0.0092*escalapp, 0.00365, 1, textures[3]);
	load_tga_image("earth", textures[3], false);
	sistemasolar[3] = terra;

	Lua lua;
	lua.SetValues(1, 0.0092*escalapp / 2, 0.00365, 0.01, textures[9], 3);
	load_tga_image("moon", textures[9], false);
	luas[0] = lua;

	Planeta marte;
	marte.SetValues(16, 0.0049*escalapp, 0, 2.9, textures[4]);
	load_tga_image("mars", textures[4], false);
	sistemasolar[4] = marte;

	Planeta jupiter;
	jupiter.SetValues(55, 0.1004*escalapg, 0, 12.9, textures[5]);
	load_tga_image("jupiter", textures[5], false);
	sistemasolar[5] = jupiter;


	Lua jupiter1;
	jupiter1.SetValues(2, 0.0092*escalapp / 2, 0.00365, 0.005, textures[3], 5);
	luas[1] = jupiter1;

	Lua jupiter2;
	jupiter1.SetValues(4, 0.0092*escalapp / 2, 0.00365, 0.01, textures[3], 5);
	luas[2] = jupiter1;

	Planeta saturno;
	saturno.SetValues(102, 0.08369*escalapg, 0, 30.5, textures[6]);
	load_tga_image("saturn", textures[6], false);
	sistemasolar[6] = saturno;

	//Aneis de Saturno
	load_tga_image("rings", texturaAneis, true);

	Planeta urano;
	urano.SetValues(206, 0.03644*escalapp, 0, 85.0, textures[7]);
	load_tga_image("uranus", textures[7], false);
	sistemasolar[7] = urano;

	Planeta neptuno;
	neptuno.SetValues(323, 0.03538*escalapp, 0, 165.8, textures[8]);
	load_tga_image("neptune", textures[8], false);
	sistemasolar[8] = neptuno;
}

void loadSkyboxTextures(void)
{
	char *impathfile[6] = { "textures/front.tga", "textures/back.tga", "textures/right.tga", "textures/left.tga", "textures/top.tga", "textures/bottom.tga" };
	int i;

	// Carrega as imagens de textura da skybox
	for (i = 0; i<6; i++)
	{
		im2[i] = tgaLoad(impathfile[i]);

		//printf("IMAGE INFO: %s\nstatus: %d\ntype: %d\npixelDepth: %d\nsize%d x %d\n", impathfile[i], im2[i]->status, im2[i]->type, im2[i]->pixelDepth, im2[i]->width, im2[i]->height); fflush(stdout);
	}

	for (i = 0; i<6; i++)
	{
		// Selecciona uma textura
		glBindTexture(GL_TEXTURE_2D, skyboxtextures[i]);

		// Impede a existência de "bordas" entre as texturas
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // MIPMAP
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// build our texture mipmaps
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, im2[i]->width, im2[i]->height, GL_RGB, GL_UNSIGNED_BYTE, im2[i]->imageData); // MIPMAP
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im2[i]->width, im2[i]->height, 0, GL_RGB, GL_UNSIGNED_BYTE, im2[i]->imageData);

	}

	// Destroi as imagens
	for (i = 0; i<6; i++) tgaDestroy(im2[i]);
}

void applyLights(){
	// Define a posição de light0 - posição do sol
	GLfloat light0_position[] = { 0.0f, 0.0f, 0.0f, 1.0 };

	// Aplica a light0
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glEnable(GL_LIGHTING);
}

void applymaterial(int type)
{
	// Define as propriedades dos materiais
	// Type: 0 (Branco); 1 (Amarelo); 2 (Ciano); 3 (Branco-Emissor)
	GLfloat mat_ambient[4][4] = {
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 0.0f, 1.0f },
			{ 0.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
	};
	GLfloat mat_diffuse[4][4] = {
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.0f, 1.0f },
			{ 0.0f, 0.5f, 0.5f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
	};
	GLfloat mat_specular[4][4] = {
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 0.0f, 1.0f },
			{ 0.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
	};
	GLfloat mat_emission[4][4] = {
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
	};
	GLfloat mat_shininess[4][1] = { { 20.0f }, { 20.0f }, { 20.0f }, { 50.0f } };

	if ((type >= 0) && (type < 4))
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient[type]); // GL_FRONT, GL_FRONT_AND_BACK , GL_BACK, 
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse[type]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular[type]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission[type]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[type]);
	}
}

void load_tga_image(std::string nome, GLuint texture, bool transparency)
{
	std::string impathfile = "textures/" + nome + ".tga";

	std::vector<char> writable(impathfile.begin(), impathfile.end());
	writable.push_back('\0');

	// Carrega a imagem de textura
	im = tgaLoad(&writable[0]);
	//printf("IMAGE INFO: %s\nstatus: %d\ntype: %d\npixelDepth: %d\nsize%d x %d\n", impathfile, im->status, im->type, im->pixelDepth, im->width, im->height);

	// Seleciona a textura atual
	glBindTexture(GL_TEXTURE_2D, texture);

	// set up quadric object and turn on FILL draw style for it
	mysolid = gluNewQuadric();
	gluQuadricDrawStyle(mysolid, GLU_FILL);

	// turn on texture coordinate generator for the quadric
	gluQuadricTexture(mysolid, GL_TRUE);

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // MIPMAP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// build our texture mipmaps
	if (!transparency){
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, im->width, im->height, GL_RGB, GL_UNSIGNED_BYTE, im->imageData); // MIPMAP
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im->width, im->height, 0, GL_RGB, GL_UNSIGNED_BYTE, im->imageData);
	}
	else{
		//Textura com transparência (anéis de saturno)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, im->width, im->height, GL_RGB, GL_UNSIGNED_BYTE, im->imageData); // MIPMAP
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, im->width, im->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, im->imageData);
	}


	// Destroi a imagem
	tgaDestroy(im);
}

int changeWindowMode(){


	glfwGetDesktopMode(&desktop);

	if (gameMode){
		//Passar para modo janela
		glfwCloseWindow();

		width = 800;
		height = 600;

		// get the current Desktop screen resolution and colour depth
		// open the window at the current Desktop resolution and colour depth
		if (!glfwOpenWindow(
			width,
			height,
			desktop.RedBits,
			desktop.GreenBits,
			desktop.BlueBits,
			8,          // alpha bits
			32,         // depth bits
			0,          // stencil bits
			GLFW_WINDOW
			)) {
			std::cout << "Failed to open window window!" << std::endl;
			glfwTerminate();
			return GLFW_WINDOW_ERROR;
		}
		gameMode = false;

		AppStart();
	}
	else{
		//Passar para o modo de jogo
		glfwCloseWindow();

		width = desktop.Width;
		height = desktop.Height;

		// open the window at the current Desktop resolution and colour depth
		if (!glfwOpenWindow(
			width,
			height,
			desktop.RedBits,
			desktop.GreenBits,
			desktop.BlueBits,
			8,          // alpha bits
			32,         // depth bits
			0,          // stencil bits
			GLFW_FULLSCREEN
			)) {
			std::cout << "Failed to open fullscreen window!" << std::endl;
			glfwTerminate();
			return GLFW_WINDOW_ERROR;
		}
		gameMode = true;

		AppStart();
	}
}

void ResizeWindow(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
	midWindowX = windowWidth / 2; // Middle of the window horizontally
	midWindowY = windowHeight / 2; // Middle of the window vertically

	float aspectRatio;
	h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;
	glViewport(0, 0, w, h);	// View port usa uma janela inteira
	aspectRatio = (float)w / (float)h;

	// prepara a vista da matrix (nao muito bem!)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspectRatio, 1, 5000);

	// seleciona o modelo de vista da matrix
	glMatrixMode(GL_MODELVIEW);
}
#pragma endregion

#pragma region Event Handlers
void handleMouseMove(int mouseX, int mouseY)
{
	//Atualiza a orientação da camara
	cam->handleMouseMove(mouseX, mouseY);
}

void Key_r(void)
{
	//Liga ou desliga a simulação
	spinMode = !spinMode;	
}

void handleKeypress(int theKey, int theAction)
{
	// If a key is pressed, toggle the relevant key-press flag
	if (theAction == GLFW_PRESS)
	{
		switch (theKey)
		{
		case 'W':
			cam->holdingForward = true;
			break;
		case 'S':
			cam->holdingBackward = true;
			break;
		case 'A':
			cam->holdingLeftStrafe = true;
			break;
		case 'D':
			cam->holdingRightStrafe = true;
			break;
		case 'Q':
			cam->holdingUp = true;
			break;
		case 'E':
			cam->holdingDown = true;
			break;
		case 'M':
		case 'm':
			//Mudar o tipo de janela
			changeGameModeActive = true;
			break;
		case GLFW_KEY_UP:
			spinMode = true;
			holdingMoreSimulationSpeed = true;
			break;
		case GLFW_KEY_DOWN:
			holdingLessSimulationSpeed = true;
			break;
		default:
			// Do nothing...
			break;
		}
	}
	else // If a key is released, toggle the relevant key-release flag
	{
		switch (theKey)
		{
		case 'W':
			cam->holdingForward = false;
			break;
		case 'S':
			cam->holdingBackward = false;
			break;
		case 'A':
			cam->holdingLeftStrafe = false;
			break;
		case 'D':
			cam->holdingRightStrafe = false;
			break;
		case 'Q':
			cam->holdingUp = false;
			break;
		case 'E':
			cam->holdingDown = false;
			break;
		case GLFW_KEY_UP:
			holdingMoreSimulationSpeed = false;
			simulationSpeedChangeAcceleration = simulationSpeedChangeAccelerationOriginal;
			break;
		case GLFW_KEY_DOWN:
			holdingLessSimulationSpeed = false;
			simulationSpeedChangeAcceleration = simulationSpeedChangeAccelerationOriginal;
			break;
		case 'R':
		case 'r':
			Key_r();
			break;
		case 'O':
		case 'o':
			drawOrbits = !drawOrbits;
			break;
		case 'M':
		case 'm':
			//Mudar o tipo de janela
			if (changeGameModeActive){
				changeWindowMode();
				changeGameModeActive = false;
			}
			break;
		case 'C':
		case 'c':
			//Tentativa de manter as posições, não funciona bem,
			//teria que ser mais trabalhado.

			/*if (arcCam){
			cam->setPositionX(arcCamX);
			cam->setPositionY(arcCamY);
			cam->setPositionZ(arcCamZ);
			}
			else{
			arcCamX = cam->getXPos();
			arcCamY = cam->getYPos();
			arcCamZ = cam->getZPos();
			}*/
			arcCamRadius = 100;
			arcCam = !arcCam;
			break;
		case 27:	// tecla esc
			exit(1);
		default:
			// Do nothing...
			break;
		}
	}
}

#pragma endregion

#pragma region Update
void UpdatePlanetas(){
	for (int i = 0; i < numeroPlanetas; i++){
		sistemasolar[i].Update(simulationSpeed);
	}
}

void UpdateLuas()
{
	for (int i = 0; i < numeroLuas; i++)
	{
		Vec3<float> posicaoPlaneta = Vec3<float>(
			sistemasolar[luas[i].GetPlaneta()].GetX(),
			sistemasolar[luas[i].GetPlaneta()].GetY(),
			sistemasolar[luas[i].GetPlaneta()].GetZ()
			);
		luas[i].Update(simulationSpeed, posicaoPlaneta);
	}
}
#pragma endregion

#pragma region Draw
void draworbit(float centerX, float centerY, float centerZ, GLint radius)
{
	//glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	float x, z;

	for (float i = 0; i<(3.14 * 2); i += (3.14 * 2) / 360)
	{
		x = centerX + sin(i)*radius;
		z = centerZ + cos(i)*radius;
		glVertex3f(x, 0, z);

		Vec3<float> normal = Vec3<float>(sistemasolar[0].GetX() - x, 0, sistemasolar[0].GetZ() - z);
		normal.normalise();

		glNormal3f(normal.getX(), 0, normal.getZ());
	}
	glEnd();
	//glEnable(GL_LIGHTING);
}

void DrawPlanetas(bool minimap){
	for (int i = numeroPlanetas; i > -1; i--){

		//Se é o sol, material emissive
		if (i == 0){
			applymaterial(3);
			if (!minimap) glDisable(GL_FOG);
		}
		else{
			applymaterial(0);
			if (!minimap) glEnable(GL_FOG);
		}

		glPushMatrix();

		sistemasolar[i].Draw(mysolid, minimap);

		glPopMatrix();

		if ((drawOrbits && !minimap) || minimap){
			glCallList(displayListIndex + 1);
		}
	}
}

void DrawCamera(){
	//Desenhar um pequeno triangulo no minimap que representa a camara
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	if (!arcCam){
		glTranslatef(cam->getXPos(), 0, cam->getZPos());
		glRotatef(-cam->getYRot(), 0.0f, 1.0f, 0.0f);
	}
	else{
		glTranslatef(arcCamX, arcCamY, arcCamZ);
		glRotatef((arcCamAngle * 180) / PI, 0.0f, 1.0f, 0.0f);
	}

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(10.0f, 0.0f, 10.0f);
	glVertex3f(-10.0f, 0.0f, 10.0f);
	glEnd();
	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
}

void DrawLuas()
{
	for (int i = 0; i < numeroLuas; i++){

		applymaterial(0);

		glPushMatrix();

		luas[i].Draw(mysolid);

		glPopMatrix();

		glColor3f(1.0, 1.0, 1.0);

		Vec3<float> posicaoPlaneta = Vec3<float>(
			sistemasolar[luas[i].GetPlaneta()].GetX(),
			sistemasolar[luas[i].GetPlaneta()].GetY(),
			sistemasolar[luas[i].GetPlaneta()].GetZ()
			);

		glDisable(GL_TEXTURE_2D);
		if (drawOrbits){
			draworbit(
				posicaoPlaneta.getX(),
				posicaoPlaneta.getY(),
				posicaoPlaneta.getZ(),
				luas[i].GetRaioOrbita()
				);
		}
		glEnable(GL_TEXTURE_2D);

	}
}

void DrawSkybox(){
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);

	glColor3f(1, 1, 1);

	// Selecciona textura
	glBindTexture(GL_TEXTURE_2D, skyboxtextures[0]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f, -1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1000.0f, -1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1000.0f, 1000.0f, 1000.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000.0f, 1000.0f, 1000.0f);
	glEnd();
	// Z- Face
	glBindTexture(GL_TEXTURE_2D, skyboxtextures[1]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f, -1000.0f, -1000.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1000.0f, -1000.0f, -1000.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1000.0f, 1000.0f, -1000.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000.0f, 1000.0f, -1000.0f);
	glEnd();
	// X+ Face
	glBindTexture(GL_TEXTURE_2D, skyboxtextures[2]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1000.0f, -1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1000.0f, -1000.0f, -1000.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1000.0f, 1000.0f, -1000.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1000.0f, 1000.0f, 1000.0f);
	glEnd();
	// X- Face
	glBindTexture(GL_TEXTURE_2D, skyboxtextures[3]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f, -1000.0f, -1000.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1000.0f, -1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1000.0f, 1000.0f, 1000.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000.0f, 1000.0f, -1000.0f);
	glEnd();
	// Y+ Face
	glBindTexture(GL_TEXTURE_2D, skyboxtextures[4]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f, 1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1000.0f, 1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1000.0f, 1000.0f, -1000.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000.0f, 1000.0f, -1000.0f);
	glEnd();
	// Y- Face
	glBindTexture(GL_TEXTURE_2D, skyboxtextures[5]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f, -1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1000.0f, -1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1000.0f, -1000.0f, -1000.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1000.0f, -1000.0f, -1000.0f);
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_FOG);
}

void DrawRings(){
	//Aneis de Saturno
	glBindTexture(GL_TEXTURE_2D, texturaAneis);
	float raioAneis = 10;
	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(sistemasolar[6].GetX() - raioAneis, sistemasolar[6].GetY(), sistemasolar[6].GetZ() + raioAneis);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(sistemasolar[6].GetX() + raioAneis, sistemasolar[6].GetY(), sistemasolar[6].GetZ() + raioAneis);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(sistemasolar[6].GetX() + raioAneis, sistemasolar[6].GetY(), sistemasolar[6].GetZ() - raioAneis);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(sistemasolar[6].GetX() - raioAneis, sistemasolar[6].GetY(), sistemasolar[6].GetZ() - raioAneis);
	glEnd();
}

void DrawMinimap(){
	float ratio;
	// Prevenir a divisão por zero, se a janela for muito pequena
	if (height == 0) height = 1;
	ratio = 1.0 * width / height;
	glViewport(width / 1.5, height / 1.5, width / 2, height / 2);
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 1, 5000);

	//HERE BE DRAGONS AND MAGIC NUMBERS
	float offsetX;
	if (width > 800) {
		offsetX = -30.0;
	}
	else{
		offsetX = -90.0;
	}
	glTranslatef(offsetX, -150.0, 0.0);
	gluLookAt(0.0, 1300, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DrawCamera();
	DrawPlanetas(true);
}

void Animate(void)
{

	// limpa a janela
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset the matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (!arcCam){
		//MOVIMENTO E ROTAÇÃO DA CAMARA COM TECLADO E RATO
		// Move the camera to our location in space
		glRotatef(cam->getXRot(), 1.0f, 0.0f, 0.0f); // Rotate our camera on the x-axis (looking up and down)
		glRotatef(cam->getYRot(), 0.0f, 1.0f, 0.0f); // Rotate our camera on the  y-axis (looking left and right)
		// Translate the ModelView matrix to the position of our camera - everything should now be drawn relative
		// to this position!
		glTranslatef(-cam->getXPos(), -cam->getYPos(), -cam->getZPos());
	}
	else{
		//Camara orbital
		arcCamX = arcCamRadius * sin(arcCamAngle);
		arcCamZ = arcCamRadius * cos(arcCamAngle);
		gluLookAt(arcCamX, 30, arcCamZ, 0, 0, 0, 0, 1, 0);
		arcCamAngle += 0.01;
		if (arcCamAngle > 2 * PI){
			arcCamAngle = 0;
		}
	}


	if (spinMode) {

		//Controla a velocidade da simulação
		if (holdingMoreSimulationSpeed && simulationSpeed > 15){
			simulationSpeed -= simulationSpeedChangeAcceleration;
			simulationSpeedChangeAcceleration += 0.1;
		}
		if (holdingLessSimulationSpeed){
			simulationSpeed += simulationSpeedChangeAcceleration;
			simulationSpeedChangeAcceleration += 0.1;
			if (simulationSpeed > 5000){
				spinMode = false;
				simulationSpeed = 5000;
			}
		}

		//Atualiza a posição dos planetas e luas
		UpdatePlanetas();
		UpdateLuas();

	}

	applyLights();

	glViewport(0, 0, width, height);
	ResizeWindow(width, height);

	glEnable(GL_TEXTURE_2D);

	//Desenha os planetas e luas
	DrawPlanetas(false);
	DrawLuas();

	//Desenhar displayLists de estrelas
	glCallList(displayListIndex);

	glPushMatrix();

	//Mover oara posição da camara
	if (!arcCam){
		glTranslatef(cam->getXPos(), 0, cam->getZPos());
	}
	else{
		glTranslatef(arcCamX, arcCamY, arcCamZ);
	}

	//Desenhar a skybox na posição da camara - a skybox acompanha o movimento da camara
	DrawSkybox();

	glPopMatrix();

	//Desenhar anéis de saturno
	DrawRings();

	//Desenhar minimap
	DrawMinimap();

	glfwSwapBuffers(); // Swap the buffers to display the scene (so we don't have to watch it being drawn!)

}
#pragma endregion

#pragma region Entry Point
void AppStart(){
	// ----- GLFW Settings -----

	glfwDisable(GLFW_MOUSE_CURSOR); // Hide the mouse cursor

	glfwSwapInterval(0);            // Disable vsync

	// ----- Window and Projection Settings -----

	// Set the window title
	glfwSetWindowTitle("Solar System Model | PMarques / VGomes | April 2016");

	// Setup our viewport to be the entire size of the window
	glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);

	// Change to the projection matrix, reset the matrix and set up our projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	init();

	glGenTextures(6, skyboxtextures);
	glGenTextures(numeroPlanetas, textures);

	//Inicializar as display lists para estrelas e órbitas
	displayListIndex = glGenLists(3);

	//Fog
	GLfloat fogColor[] = { 0.1, 0.1, 0.1, 1.0 };
	// Activar o nevoeiro
	glEnable(GL_FOG);
	// Define a cor do nevoeiro
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_DENSITY, 0.005f);

	loadSkyboxTextures();
	initSistemaSolar();
	//Gerar a displayList de órbitas de planetas
	initPlanetOrbits();
	initLights();
	initEstrelas();


	// Instantiate our pointer to a Camera object providing it the size of the window
	cam = new Camera(windowWidth, windowHeight);

	// Set the mouse cursor to the centre of our window
	glfwSetMousePos(midWindowX, midWindowY);
	//Handle keyboard events
	glfwSetKeyCallback(handleKeypress);
	// Specify the function which should execute when the mouse is moved
	glfwSetMousePosCallback(handleMouseMove);
	glfwSetWindowSizeCallback(ResizeWindow);

	// The deltaTime variable keeps track of how much time has elapsed between one frame and the next.
	// This allows us to perform framerate independent movement i.e. the camera will move at the same
	// overall speed regardless of whether the app's running at (for example) 6fps, 60fps or 600fps!
	double deltaTime = 0.0;

	// Flag to keep our main loop running
	bool running = true;

	std::cout << "Running!" << std::endl;

	while (running)
	{
		// Calculate our camera movement
		cam->move(deltaTime);

		// Draw our scene
		Animate();

		// exit if ESC was pressed or window was closed
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

		// Call our fpsManager to limit the FPS and get the frame duration to pass to the cam->move method
		deltaTime = fpsManager.enforceFPS();
	}
}

int main(int argc, char** argv)
{

	// Frame counter and window settings variables
	int redBits = 8, greenBits = 8, blueBits = 8;
	int alphaBits = 8, depthBits = 24, stencilBits = 0;

	// Inicializar GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialise GLFW!" << endl;
		glfwTerminate();
		return GLFW_INIT_ERROR;
	}

	// get the current Desktop screen resolution and colour depth
	GLFWvidmode desktop;
	glfwGetDesktopMode(&desktop);

	width = desktop.Width;
	height = desktop.Height;

	// open the window at the current Desktop resolution and colour depth
	if (!glfwOpenWindow(
		desktop.Width,
		desktop.Height,
		desktop.RedBits,
		desktop.GreenBits,
		desktop.BlueBits,
		8,          // alpha bits
		32,         // depth bits
		0,          // stencil bits
		GLFW_FULLSCREEN
		)) {
		std::cout << "Failed to open fullscreen window!" << std::endl;
		glfwTerminate();
		return GLFW_WINDOW_ERROR;
	}

	//Inicializar, carregar e correr a simulação
	AppStart();

	// Clean up GLFW and exit
	glfwTerminate();

	delete cam; // Delete our pointer to the camera object
	return 0;
}
#pragma endregion