// car.h
#ifndef CAR_H
#define CAR_H

#include <GL/glut.h>
#include <GL/glu.h>

// External declaration for global variables defined in main.cpp
extern float carX;
extern float carZ;
extern float carAngle;
extern float wheelRotation;
extern float wheelSteerAngle;
extern float carSpeed;
extern bool isDay;

// Function declarations for car geometry
void drawWheel(float radius, float width);
void carBody();
void carCabin();
void carSpoiler();
void carWheels();
void drawCar();

#endif // CAR_H


