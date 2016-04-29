/*
Classe desenvolvida a partir do tutorial "A C++ Camera Class for Simple OpenGL FPS Controls":
http://r3dux.org/2012/12/a-c-camera-class-for-simple-opengl-fps-controls/	
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <math.h>         // Used only for sin() and cos() functions
#include "GL/glfw.h"     // Include OpenGL Framework library for the GLFW_PRESS constant only!
#include "Vec3.h"        // Include our custom Vec3 class

class Camera
{
protected:
	// Camera position
	Vec3<double> position;

	// Camera rotation
	Vec3<double> rotation;



	// Camera movement speed. When we call the move() function on a camera, it moves using these speeds
	Vec3<double> speed;

	double movementSpeedFactor; // Controls how fast the camera moves
	double cameraPosition;
	double pitchSensitivity;    // Controls how sensitive mouse movements affect looking up and down
	double yawSensitivity;      // Controls how sensitive mouse movements affect looking left and right

	// Window size in pixels and where the midpoint of it falls
	int windowWidth;
	int windowHeight;
	int windowMidX;
	int windowMidY;

	// Method to set some reasonable default values. For internal use by the class only.
	void initCamera();



public:
	static const double TO_RADS; // The value of 1 degree in radians

	// Holding any keys down?
	bool holdingForward;
	bool holdingBackward;
	bool holdingLeftStrafe;
	bool holdingRightStrafe;
	bool holdingUp;
	bool holdingDown;

	// Constructor
	Camera(float windowWidth, float windowHeight);

	// Destructor
	~Camera();

	// Mouse movement handler to look around
	void handleMouseMove(int mouseX, int mouseY);

	// Method to convert an angle in degress to radians
	const double toRads(const double &angleInDegrees) const;

	// Method to move the camera based on the current direction
	void move(double deltaTime);

	// --------------------------------- Inline methods ----------------------------------------------

	// Setters to allow for change of vertical (pitch) and horizontal (yaw) mouse movement sensitivity
	float getPitchSensitivity()            { return pitchSensitivity; }
	void  setPitchSensitivity(float value) { pitchSensitivity = value; }
	float getYawSensitivity()              { return yawSensitivity; }
	void  setYawSensitivity(float value)   { yawSensitivity = value; }

	void setPositionX(float value)		   { position.setX(value); }
	void setPositionY(float value)		   { position.setY(value); }
	void setPositionZ(float value)		   { position.setZ(value); }

	// Position getters
	Vec3<double> getPosition() const { return position; }
	double getXPos()           const { return position.getX(); }
	double getYPos()           const { return position.getY(); }
	double getZPos()           const { return position.getZ(); }

	// Rotation getters
	Vec3<double> getRotation() const { return rotation; }
	double getXRot()           const { return rotation.getX(); }
	double getYRot()           const { return rotation.getY(); }
	double getZRot()           const { return rotation.getZ(); }
};

#endif // CAMERA_H