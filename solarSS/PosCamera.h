#include <iostream>
#include <math.h>         // Used only for sin() and cos() functions
#include "GL/glfw.h"     // Include OpenGL Framework library for the GLFW_PRESS constant only!
#include "Vec3.h"        

using namespace glm;

vec3 ExtractCameraPos(const mat4 & a_modelView)
{
	// Get the 3 basis vector planes at the camera origin and transform them into model space.
	//  
	// NOTE: Planes have to be transformed by the inverse transpose of a matrix
	//       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
	//
	//       So for a transform to model space we need to do:
	//            inverse(transpose(inverse(MV)))
	//       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
	//
	// As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
	//  
	mat4 modelViewT = transpose(a_modelView);

	// Get plane normals 
	vec3 n1(modelViewT[0]);
	vec3 n2(modelViewT[1]);
	vec3 n3(modelViewT[2]);

	// Get plane distances
	float d1(modelViewT[0].w);
	float d2(modelViewT[1].w);
	float d3(modelViewT[2].w);

	// Get the intersection of these 3 planes
	// http://paulbourke.net/geometry/3planes/
	vec3 n2n3 = cross(n2, n3);
	vec3 n3n1 = cross(n3, n1);
	vec3 n1n2 = cross(n1, n2);

	vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
	float denom = dot(n1, n2n3);

	return top / -denom;
}

vec3 ExtractCameraPos2(const mat4 & a_modelView)
{
	// Get the 3 basis vector planes at the camera origin and transform them into model space.
	//  
	// NOTE: Planes have to be transformed by the inverse transpose of a matrix
	//       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
	//
	//       So for a transform to model space we need to do:
	//            inverse(transpose(inverse(MV)))
	//       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
	//
	// As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
	//  
	mat4 modelViewT = transpose(a_modelView);

	// Get plane normals 
	vec3 n1(modelViewT[0]);
	vec3 n2(modelViewT[1]);
	vec3 n3(modelViewT[2]);

	// Get plane distances
	float d1(modelViewT[0].w);
	float d2(modelViewT[1].w);
	float d3(modelViewT[2].w);

	// Get the intersection of these 3 planes 
	// (uisng math from RealTime Collision Detection by Christer Ericson)
	vec3 n2n3 = cross(n2, n3);
	float denom = dot(n1, n2n3);

	vec3 top = (n2n3 * d1) + cross(n1, (d3*n2) - (d2*n3));
	return top / -denom;
}


vec3 ExtractCameraPos3(const mat4 & a_modelView)
{
	// Get the 3 basis vector planes at the camera origin and transform them into model space.
	//  
	// NOTE: Planes have to be transformed by the inverse transpose of a matrix
	//       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
	//
	//       So for a transform to model space we need to do:
	//            inverse(transpose(inverse(MV)))
	//       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
	//
	// As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
	//  
	mat4 modelViewT = transpose(a_modelView);

	// Get plane normals 
	vec3 n1(modelViewT[0]);
	vec3 n2(modelViewT[1]);
	vec3 n3(modelViewT[2]);

	// Get plane distances
	float d1(modelViewT[0].w);
	float d2(modelViewT[1].w);
	float d3(modelViewT[2].w);

	// Get the intersection of these 3 planes 
	// (uisng math from RealTime Collision Detection by Christer Ericson)
	vec3 n2n3 = cross(n2, n3);
	float denom = dot(n1, n2n3);

	vec3 d(d1, d2, d3);
	vec3 v = cross(n1, d);

	vec3 top;
	top.x = dot(d, n2n3);
	top.y = dot(n3, v);
	top.z = -dot(n2, v);

	return top / -denom;
}


vec3 ExtractCameraPos_NoScale(const mat4 & a_modelView)
{
	mat3 rotMat(a_modelView);
	vec3 d(a_modelView[3]);

	vec3 retVec = -d * rotMat;
	return retVec;
}


void PrintVec3(const vec3 & a_vec)
{
	printf("%f %f %f", a_vec.x, a_vec.y, a_vec.z);
}

int main(int argc, char * argv[])
{
	vec3 cameraPos(1.234f, 2.657f, 7.865f);

	// Create a model view matrix with translations and scales
	mat4 modelView(1.0f);
	modelView = rotate(modelView, 0.5f, vec3(1.0f, 0.0f, 0.0f));
	modelView = rotate(modelView, 0.2f, vec3(0.0f, 1.0f, 0.0f));
	modelView = rotate(modelView, 0.3f, vec3(0.0f, 0.0f, 1.0f));
	modelView = scale(modelView, vec3(2.0f, 3.0f, 4.0f));

	modelView = translate(modelView, -cameraPos);

	vec3 cameraPos2 = ExtractCameraPos(modelView);
	vec3 cameraPos2a = ExtractCameraPos2(modelView);

	// ExtractCameraPos3 does not seem as accurate as the previous 2 methods
	vec3 cameraPos2b = ExtractCameraPos3(modelView);

	mat4 viewModel = inverse(modelView);
	vec3 cameraPos3(viewModel[3]);

	// Do not add scale if using this
	//vec3 cameraPos4 = ExtractCameraPos_NoScale(modelView);

	PrintVec3(cameraPos); printf(" - Base Camera\n");
	PrintVec3(cameraPos2); printf(" - Plane intersection extraction\n");
	PrintVec3(cameraPos2a); printf(" - Plane intersection extraction - optimize\n");
	PrintVec3(cameraPos2b); printf(" - Plane intersection extraction alternate\n");
	PrintVec3(cameraPos3); printf(" - Inverse Matrix\n");

	return 0;
}