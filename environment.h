
// environment.h
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <GL/glut.h>

// External declaration for global variables defined in main.cpp
extern const int PERSON_COUNT;
extern float personX[];
extern float personZ[];
extern float personColor[];
extern bool personVisible[];

extern const int POLICE_COUNT;
extern float policeX[];
extern float policeZ[];
extern bool policeVisible[];

// Function declarations for environment objects
void drawEnvironment();

#endif // ENVIRONMENT_H
