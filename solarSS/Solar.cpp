#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#ifndef GL_MULTISAMPLE_ARB
#define GL_MULTISAMPLE_ARB  0x809D
#endif
#define PI 3.14159265

#include "Solar.h"   
#include <stdlib.h> 
#include <stdio.h>
#include "gl/glut.h"
#include "GL/glfw.h"
#include "lua.h"
#include "tga.h"
#include <string>
#include <vector>
#include "Camera.h"
#include "FPSManager.hpp"

// Specify default namespace for commonly used elements
using std::string;
using std::cout;
using std::endl;

// Define a few constants for error conditions
const int GLFW_INIT_ERROR = -1;
const int GLFW_WINDOW_ERROR = -2;

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
int width, height;
GLFWvidmode desktop;
bool arcCam = false;
float arcCamAngle = 0;
float arcCamX = 0;
float arcCamZ = 0;

//Camara
Camera *cam;

// Define our window title to append the FPS stats to
string windowTitle = "Solar System Model | PMarques / VGomes | April 2016";

// Create a FPS manager that locks to 60fps and updates the window title with stats every 3 seconds
FpsManager fpsManager(60.0, 3.0, windowTitle);

const int numeroPlanetas = 9;
Planeta sistemasolar[numeroPlanetas];
GLuint textures[numeroPlanetas];
tgaInfo *im;
GLUquadric *mysolid;
const int numeroLuas = 3;
Lua luas[numeroLuas];
GLuint texturasLua[numeroLuas];

// Callback function to handle mouse movements
void handleMouseMove(int mouseX, int mouseY)
{
	cam->handleMouseMove(mouseX, mouseY);
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
	
	spinMode = !spinMode;	// liga e desliga a animaçao
	
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
	GLfloat global_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
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
	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0025 /2);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0025/2);
	glEnable(GL_LIGHT0);


}

void applyLights(){
	// Define a posição de light0
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

void draworbit(float centerX, float centerY, float centerZ, GLint radius)
{
	//glDisable(GL_LIGHTING);
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
		}
		else{
			applymaterial(0);
		}

		glPushMatrix();

		sistemasolar[i].Draw(mysolid, minimap);

		glPopMatrix();

		if (drawOrbits){
			draworbit(
				sistemasolar[0].GetX(),
				sistemasolar[0].GetY(),
				sistemasolar[0].GetZ(),
				sistemasolar[i].GetDistanciaSol()
			);
		}
		
		
		
	}
}

void DrawCamera(){
	glPushMatrix();
	if (!arcCam){
		glTranslatef(cam->getXPos(), 0, cam->getZPos());
		glRotatef(-cam->getYRot(), 0.0f, 1.0f, 0.0f);
	}
	else{
		glTranslatef(arcCamX, 0, arcCamZ);
		//glRotatef((arcCamAngle * 3.1415) / 180, 0.0f, 1.0f, 0.0f);
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
}

void DrawLuas()
{
	for (int i = 0; i < numeroLuas; i++){

		applymaterial(0);

		glPushMatrix();

		luas[i].Draw(mysolid);

		glPopMatrix();

		Vec3<float> posicaoPlaneta = Vec3<float>(
			sistemasolar[luas[i].GetPlaneta()].GetX(),
			sistemasolar[luas[i].GetPlaneta()].GetY(),
			sistemasolar[luas[i].GetPlaneta()].GetZ()
			);

		if (drawOrbits){
			draworbit(
				posicaoPlaneta.getX(),
				posicaoPlaneta.getY(),
				posicaoPlaneta.getZ(),
				luas[i].GetRaioOrbita()
				);
		}

	}
}
//isto pode ou nao ser alterado
static void Animate(void)
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
		float radius = 100.0;
		arcCamX = radius * sin(arcCamAngle);
		arcCamZ = radius * cos(arcCamAngle);
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

		//Atualiza a posição dos planetas
		UpdatePlanetas();
		UpdateLuas();

	}

	glEnable(GL_TEXTURE_2D);

	applyLights();

	//glViewport(0, 0, width, height);
	glViewport(0, 0, width, height);
	ResizeWindow(width, height);

	//Desenha os planetas
	DrawPlanetas(false);
	DrawLuas();

	// Draw on left side

	float ratio;
	// Prevenir a divisão por zero, se a janela for muito pequena
	if (height == 0) height = 1;
	ratio = 1.0 * width / height;
	glViewport(width / 1.5, height / 1.5, width / 2, height / 2);
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
	glDisable(GL_LIGHTING);
	DrawCamera();
	DrawPlanetas(true);
	glEnable(GL_LIGHTING);

	glfwSwapBuffers(); // Swap the buffers to display the scene (so we don't have to watch it being drawn!)

}

// Initialize OpenGL's rendering modes
void OpenGLInit(void)
{

	//Tentativa de antialiasing das linhas das órbitas
	glEnable(GL_MULTISAMPLE_ARB);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwDisable(GLFW_MOUSE_CURSOR); // Hide the mouse cursor
	glfwSwapInterval(0);            // Disable vsync

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL); // GL_LINE, GL_POINT, GL_FILL

	// Activa o teste de profundidade
	glEnable(GL_DEPTH_TEST);
}

static void ResizeWindow(int w, int h)
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

//Parametros do método SetValues:
	//Distancia ao sol / raio da órbita
	//Raio do planeta
	//Velocidade de rotação
	//Velocidade de periodo orbital
	//Textura

	float escalapp = 30;
	float escalapg = 10;

	Planeta sol;
	sol.SetValues(0, 1, 0.01, 1, textures[0]);
	load_tga_image("sun", textures[0]);
	sistemasolar[0] = sol;

	Planeta mercurio;
	mercurio.SetValues(4, 0.0035 * escalapp, 0, 1.2, textures[1]);
	load_tga_image("mercury", textures[1]);
	sistemasolar[1] = mercurio;

	Planeta venus;
	venus.SetValues(7, 0.0087 * escalapp, 0, 1.6, textures[2]);
	load_tga_image("venus", textures[2]);
	sistemasolar[2] = venus;

	Planeta terra;
	terra.SetValues(10, 0.0092*escalapp, 0.00365, 1, textures[3]);
	load_tga_image("earth", textures[3]);
	sistemasolar[3] = terra;

		Lua lua;
		lua.SetValues(1, 0.0092*escalapp / 2, 0.00365, 0.01, textures[3], 3);
		luas[0] = lua;

	Planeta marte;
	marte.SetValues(16, 0.0049*escalapp, 0, 2.9, textures[4]);
	load_tga_image("mars", textures[4]);
	sistemasolar[4] = marte;

	Planeta jupiter;
	jupiter.SetValues(55, 0.1004*escalapg, 0, 12.9, textures[5]);
	load_tga_image("jupiter", textures[5]);
	sistemasolar[5] = jupiter;

		Lua jupiter1;
		jupiter1.SetValues(2, 0.0092*escalapp / 2, 0.00365, 0.005, textures[3], 5);
		luas[1] = jupiter1;

		Lua jupiter2;
		jupiter1.SetValues(4, 0.0092*escalapp / 2, 0.00365, 0.01, textures[3], 5);
		luas[2] = jupiter1;

	Planeta saturno;
	saturno.SetValues(102, 0.08369*escalapg, 0, 30.5, textures[6]);
	load_tga_image("saturn", textures[6]);
	sistemasolar[6] = saturno;

	Planeta urano;
	urano.SetValues(206, 0.03644*escalapp, 0, 85.0, textures[7]);
	load_tga_image("uranus", textures[7]);
	sistemasolar[7] = urano;

	Planeta neptuno;
	neptuno.SetValues(323, 0.03538*escalapp, 0, 165.8, textures[8]);
	load_tga_image("neptune", textures[8]);
	sistemasolar[8] = neptuno;
}

int changeWindowmode();
void handleKeypress(int, int);
void AppStart();

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

// Callback function to handle keypresses
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

void AppStart(){
	// ----- GLFW Settings -----

	glfwDisable(GLFW_MOUSE_CURSOR); // Hide the mouse cursor

	glfwSwapInterval(0);            // Disable vsync

	// ----- Window and Projection Settings -----

	// Set the window title
	glfwSetWindowTitle("Solar System FPS Controls Mk2| r3dux.org | Dec 2012");

	// Setup our viewport to be the entire size of the window
	glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);

	// Change to the projection matrix, reset the matrix and set up our projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	OpenGLInit();

	glGenTextures(numeroPlanetas, textures);
	initSistemaSolar();
	initLights();

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

//rota principal
int main(int argc, char** argv)
{

	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_RGBA);
	//glutInitWindowPosition(0, 0);
	//glutCreateWindow("Sistema Solar");
	////glutInitWindowSize(1080, 1080);
	//glutFullScreen();

	// Frame counter and window settings variables
	int redBits = 8, greenBits = 8, blueBits = 8;
	int alphaBits = 8, depthBits = 24, stencilBits = 0;

	// Initialise GLFW
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
		GLFW_WINDOW
		)) {
			std::cout << "Failed to open fullscreen window!" << std::endl;
			glfwTerminate();
			return GLFW_WINDOW_ERROR;
	}

	AppStart();

	// Clean up GLFW and exit
	glfwTerminate();

	delete cam; // Delete our pointer to the camera object

	return(0);
}
