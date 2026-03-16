//crashedCar.h
#ifndef CRASHEDCAR_H
#define CRASHEDCAR_H

#include <GL/glut.h>
#include <GL/glu.h>

// External declaration for global variables defined in main.cpp
extern float carX;
extern float carZ;
extern float carAngle;
extern float wheelRotation;
extern float wheelSteerAngle;
extern float carSpeed;
extern bool isCrashed;

// Function declarations for car geometry
void drawCrashedWheel(float radius, float width);
void crashedBody();
void crashedCabin();
void crashedWheels();
void crashedLights();
void drawCrashedCar();

#endif // CAR_H




