#include "crashedCar.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>

//draw a single wheel
void drawCrashedWheel(float radius, float width) {
    glColor3f(0.0f, 0.0f, 0.0f); // black color wheel

    GLUquadric* quad = gluNewQuadric(); // create a quadric object

    glPushMatrix();
    glRotatef(180, 0, 1, 1); // rotate to align cylinder along X-axis
    gluCylinder(quad, radius, radius, width, 30, 1); // base radius, top radius, height, slices, stacks

    // Draw the two wheel caps
    gluDisk(quad, 0.0, radius, 30, 1);       // one end
    glTranslatef(0.0f, 0.05f, width);         // move to the other end
    gluDisk(quad, 0.0, radius, 30, 1);       // other end

    glPopMatrix();
    gluDeleteQuadric(quad);
}


//car body
void crashedBody(){
    glPushMatrix();
    glColor3f(0.7f, 0.0f, 0.0f);
    glTranslatef(0.2f, 0.0f, 0.0f);
    //glscalef is the size of obj, x,y,z
    glScalef(1.6f, 0.4f, 0.7f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

//car cabin trapezoidal
void crashedCabin(){

    glPushMatrix();
    glColor3f(0.9f, 0.8f, 0.4f);
    glTranslatef(0.25f, 0.35f, 0.0f);
    glScalef(0.4f, 0.1f, 0.4f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void crashedLights() {

    glColor3f(1.0f, 0.5f, 0.0f);

    // Left rear light
    glPushMatrix();
    glTranslatef(1.0f, -0.05f, 0.25f); // rear-left
    glScalef(0.08f, 0.05f, 0.12f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Right rear light
    glPushMatrix();
    glTranslatef(1.0f, -0.05f, -0.25f); // rear-right
    glScalef(0.08f, 0.05f, 0.12f);
    glutSolidCube(1.0f);
    glPopMatrix();
}


//car wheels
void crashedWheels(){
    float radius = 0.17f;
    float width = 0.1f; // thickness of wheel

    //back wheel left side
    glPushMatrix();
    glTranslatef(0.65f, -0.2f, 0.3f); //wheel position
    glRotatef(wheelRotation, 0, 0, 1);
    drawCrashedWheel(radius, width);
    glPopMatrix();

    //back wheel right side
    glPushMatrix();
    glTranslatef(0.65f, -0.2f, -0.4f);
    glRotatef(wheelRotation, 0, 0, 1);
    drawCrashedWheel(radius, width);
    glPopMatrix();

    //front wheel left side
    glPushMatrix();
    glTranslatef(-0.40f, -0.2f, 0.3f);
    glRotatef(wheelSteerAngle, 0, 1, 0); //turn wheel
    glRotatef(wheelRotation, 0, 0, 1); //rotate wheel with movement
    drawCrashedWheel(radius, width);
    glPopMatrix();

    //front wheel right side
    glPushMatrix();
    glTranslatef(-0.40f, -0.2f, -0.4f);
    glRotatef(wheelSteerAngle, 0, 1, 0); //turn wheel
    glRotatef(wheelRotation, 0, 0, 1); //rotate wheel with movement
    drawCrashedWheel(radius, width);
    glPopMatrix();
}

//draw entire car
void drawCrashedCar() {
    glPushMatrix();

    glTranslatef(carX, 1.0f, carZ);
    glRotatef(carAngle, 0.0f, 1.0f, 0.0f);


    glScalef(4.0f, 4.0f, 4.0f); //scale car

    crashedBody();
    crashedCabin();
    crashedLights();
    crashedWheels();

    glPopMatrix();
}



