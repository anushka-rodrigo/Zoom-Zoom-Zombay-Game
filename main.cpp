#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>

#include <windows.h>
#include <mmsystem.h>

// Include our custom modules
#include "car.h"
#include "environment.h"
#include "crashedCar.h"

// Define M_PI if necessary
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Global Variables for Car Movement (Defined here, accessed by Car.cpp using 'extern')
float carX = 20.0f;
float carZ = -15.0f;
float carAngle = 0.0f;
float carSpeed = 0.0f; //forward, backward speed
float wheelRotation = 0.0f; //for wheels to rotate
float wheelSteerAngle = 0.0f; //variable to turn the wheels

bool isDay = true;
bool isCrashed = false;
float zoom = 60.0f; //this is the default FOV (45 to 60)

//Person collision state data
const int PERSON_COUNT = 5;
const int POLICE_COUNT = 2;

float personX[PERSON_COUNT] = {-15.0f, 0.0f, -5.0f, -20.0f, -45.0f};
float personZ[PERSON_COUNT] = {-22.0f, -22.0f, -7.0f, -7.0f, -50.0f};
float personColor[PERSON_COUNT] = {0.5f, 0.7f, 0.3f, 0.9f, 0.0f};

float policeX[PERSON_COUNT] = {20.0f, -25.0f};
float policeZ[PERSON_COUNT] = {-22.0f, -70.0f};

bool personVisible[PERSON_COUNT] = {true, true, true, true, true,};
bool policeVisible[PERSON_COUNT] = {true, true};

//obj collisions
struct Obstacle {
    float x;
    float z;
    float radius;
};

Obstacle treeObstacles[] = {
    { 5.0f,  -35.0f, 2.5f },
    { 25.0f, -45.0f, 2.5f },
    { -10.0f, -55.0f, 3.0f },
    { 15.0f, -65.0f, 3.0f },
    { 45.0f, -2.0f,  3.0f },
    { 75.0f, -2.0f,  3.0f },
    { -45.0f, -105.0f, 3.0f }
};
const int TREE_COUNT = sizeof(treeObstacles) / sizeof(Obstacle);

Obstacle houseObstacles[] = {
    { -30.0f, -2.0f, 3.5f },
    { -10.0f, -2.0f, 3.5f },
    {  10.0f, -2.0f, 3.5f }
};
const int HOUSE_COUNT = sizeof(houseObstacles) / sizeof(Obstacle);

Obstacle buildingObstacles[] = {
    { -50.0f, -35.0f, 5.0f },
    { -50.0f, -60.0f, 5.0f },
    { -50.0f, -85.0f, 5.0f },
    {  60.0f, -40.0f, 5.0f }
};
const int BUILDING_COUNT = sizeof(buildingObstacles) / sizeof(Obstacle);

bool hitObstacle(float cx, float cz, Obstacle o) {
    float dx = cx - o.x;
    float dz = cz - o.z;
    return (dx * dx + dz * dz) <= (o.radius * o.radius);
}

bool hitPerson(float carX, float carZ, float px, float pz, float radius)
{
    float dx = carX - px;
    float dz = carZ - pz;
    return (dx * dx + dz * dz) <= (radius * radius);
}

// Car movement with arrow keys (from car.cpp)
void specialKeys(int key, int x, int y) {
    float moveSpeed = 0.5f;
    float accel = 0.1f;
    float steering = 2.0f;

    if (isCrashed){
        return;
    }
    switch (key) {
        case GLUT_KEY_UP:
            if (carSpeed < 0.0f){
                carSpeed += accel * 2.5f;  // braking while reversing
            }
            else{
                carSpeed += accel;
            }
            break;

        case GLUT_KEY_DOWN:
            if (carSpeed > 0.0f){
                carSpeed -= accel * 2.5f;  // braking while moving forward
            }
            else{
                carSpeed -= accel;
            }
            break;

        case GLUT_KEY_LEFT:
            if (carSpeed >= 0.0f){
                carAngle += steering; //forward
            }
            else{
                carAngle -= steering; //backward steering
            }
            wheelSteerAngle = 25.0f;   // steer left
            break;

        case GLUT_KEY_RIGHT:
            if (carSpeed >= 0.0f){
                carAngle -= steering; //forward
            }
            else{
                carAngle += steering; //backward steering
            }
            wheelSteerAngle = -25.0f;   // steer right
            break;

        case GLUT_KEY_PAGE_UP: //for testing purposes - to move car in and out of scrn in z axis
            carZ -= moveSpeed;
            break;

        case GLUT_KEY_PAGE_DOWN: //for testing purposes - to move car in and out of scrn in z axis
            carZ += moveSpeed;
            break;
    }

    // Auto-center steering
    if (key != GLUT_KEY_LEFT && key != GLUT_KEY_RIGHT) {
        wheelSteerAngle *= 0.85f; // smooth return
    }

    glutPostRedisplay();
}

//declared function so it can be used in keyboard function
void reshape(int w, int h);
void resetPeople();


void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        resetPeople();
        glutPostRedisplay();
        return;
    }

    if (key == 'd' || key == 'D') {
        isDay = !isDay;
        glutPostRedisplay();
    }

    //for testing purposes - change to crashed state
    if (key == 'c' || key == 'C') {
        isCrashed = !isCrashed;
        if (isCrashed){
            //sop all movements
            carSpeed=0.0f;
            wheelRotation=0.0f;
            wheelSteerAngle=0.0f;
        }
        glutPostRedisplay();
        return;
    }

    switch(key){
        case 'w':
            if (zoom > 5.0f){
                zoom -= 2.0f;
            }
            break;
        case 'W':
            if (zoom > 5.0f){
                zoom -= 2.0f;
            }
            break;
        case 's':
            if (zoom < 120.0f){
                zoom += 2.0f;
            }
            break;
        case 'S':
            if (zoom < 120.0f){
                zoom += 2.0f;
            }
            break;
    }

    // We must manually call the reshape logic to apply the new 'zoom' to gluPerspective
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    reshape(width, height);

    glutPostRedisplay();
}

//arc motion for car
void updateCar() {
    float rad = carAngle * M_PI / 180.0f;

    if (isCrashed){
        return;
    }

    // move forward in the direction the car is facing
    carX -= carSpeed * cos(rad);
    carZ += carSpeed * sin(rad);

    //person collision detection
    float hitRadius = 2.0f; // car + person size

    for (int i=0; i<PERSON_COUNT; i++){
        if (personVisible[i] && hitPerson(carX, carZ, personX[i], personZ[i], hitRadius)){
            personVisible[i] = false; //bye bye signore
        }
    }
    for (int j=0; j<POLICE_COUNT; j++){
        if (policeVisible[j] && hitPerson(carX, carZ, policeX[j], policeZ[j], hitRadius)){
            policeVisible[j] = false; //bye bye signore
        }
    }

    //obj collision check
    float carRadius = 1.5f;

    for (int i = 0; i < TREE_COUNT; i++) {
        if (hitObstacle(carX, carZ, treeObstacles[i])) {
            isCrashed = true;
            carSpeed = 0.0f;
            return;
        }
    }

    for (int i = 0; i < HOUSE_COUNT; i++) {
        if (hitObstacle(carX, carZ, houseObstacles[i])) {
            isCrashed = true;
            carSpeed = 0.0f;
            return;
        }
    }

    for (int i = 0; i < BUILDING_COUNT; i++) {
        if (hitObstacle(carX, carZ, buildingObstacles[i])) {
            isCrashed = true;
            carSpeed = 0.0f;
            return;
        }
    }

    // Rotate wheels based on speed
    wheelRotation += carSpeed * 50.0f; // multiplier controls spin speed

    // natural slowing down (friction)
    carSpeed *= 0.98f;

    // speed limits (IMPORTANT)
    if (carSpeed > 0.7f)  {
        carSpeed = 0.65f;   // max forward speed
    }
    if (carSpeed < -0.4f) {
        carSpeed = -0.35f;  // max reverse speed
    }
}

//bring back the people to life
void resetPeople()
{
    for (int i = 0; i < PERSON_COUNT; i++) {
        personVisible[i] = true;
    }for (int j = 0; j < POLICE_COUNT; j++) {
        policeVisible[j] = true;
    }
    isCrashed = false;
    // Reset car to original starting position
    carX = 20.0f;       // original X
    carZ = -15.0f;      // original Z
    carAngle = 0.0f;    // facing forward
    carSpeed = 0.0f;    // stopped
    wheelRotation = 0.0f;
    wheelSteerAngle = 0.0f;
}

// Lighting and setup (from environment.cpp init)
void init() {
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    // Lighting setup
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); //for sun/moon
    glEnable(GL_LIGHT1); //for headlights

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


}

// Reshaping window (from both files)
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(zoom, (float)w / h, 0.1, 500.0);
    glMatrixMode(GL_MODELVIEW);
}


void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    if (isDay) {
        //Light options for Day
        GLfloat lightPos[] = {0.0f, 80.0f, 0.0f, 1.0f};
        GLfloat ambient[]  = {0.75f, 0.75f, 0.75f, 1.0f};
        GLfloat diffuse[]  = {1.0f,  1.0f,  1.0f,  1.0f};
        GLfloat specular[] = {0.3f,  0.3f,  0.3f,  1.0f};

        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

        glClearColor(0.65f, 0.85f, 0.95f, 1.0f); // sky blue
    }
    else {
        //Light options for Night
        GLfloat lightPos[] = {0.0f, 40.0f, 20.0f, 1.0f};
        GLfloat ambient[]  = {0.15f, 0.15f, 0.25f, 1.0f};
        GLfloat diffuse[]  = {0.4f,  0.4f,  0.6f,  1.0f};
        GLfloat specular[] = {0.1f,  0.1f,  0.2f,  1.0f};

        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

        glClearColor(0.05f, 0.05f, 0.15f, 1.0f); // night sky
    }

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void setupHeadlights() {
    if (isDay) {
        // Turn headlights OFF during day
        glDisable(GL_LIGHT1);
        return;
    }

    glEnable(GL_LIGHT1);

    // Calculate forward direction from car angle
    float rad = carAngle * M_PI / 180.0f;
    float dirX = -cos(rad);
    float dirZ =  sin(rad);

    // Position headlights slightly in front of car
    GLfloat lightPos[] = {
        carX + dirX * 1.5f,  // forward offset
        0.8f,               // height from ground
        carZ + dirZ * 1.5f,
        1.0f
    };

    // Spotlight direction
    GLfloat lightDir[] = {
        dirX,
        -0.1f,   // slight downward angle
        dirZ
    };

    // Headlight color
    GLfloat diffuse[]  = {3.0f, 3.0f, 2.8f, 1.0f};   // was 1.0f
    GLfloat specular[] = {3.0f, 3.0f, 2.8f, 1.0f};   // was 1.0f
    GLfloat ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};

    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDir);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

    // Spotlight parameters
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0f);   // beam width
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 50.0f); // beam intensity
}


// Redrawing window (The main drawing loop)
void display() {

    setupLighting();   // sun / moon + sky
    setupHeadlights(); //car headlights

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    updateCar(); //added to car turning functionality
    //adding car functionality here ensures proper circular angled turning
    //if you implement this function any where else it won't implement the circular turning motion
    //instead it will just rotate in same place

    // Camera setup
    gluLookAt(
        -5.0f, 30.0f, 15.0f,   // eye
        -5.0f, 0.0f, -25.0f,    // center
        0.0f, 1.0f, 0.0f    // up
    );

    // Call drawing functions from other modules
    drawEnvironment();
    if (!isCrashed){
        drawCar();
    }
    else{
        drawCrashedCar();
    }

    glutSwapBuffers();
}

// Main function (from both files)
int main(int argc, char** argv) {
    // Print instructions to console
    std::cout << "********** 3D Car Simulation Controls **********\n\n";
    std::cout << "Car behaviour -\n";
    std::cout << "Arrow Up   : Accelerate / Move Forward\n";
    std::cout << "Arrow Down : Brake / Reverse\n";
    std::cout << "Arrow Left : Turn Left\n";
    std::cout << "Arrow Right: Turn Right\n\n\n";

    std::cout << "Environment behaviour -\n";
    std::cout << "D : Toggle Day / Night\n";
    std::cout << "R : Reset environment\n";
    std::cout << "W : Zoom in\n";
    std::cout << "S : Zoom out\n\n";
    std::cout << "**********************************************\n\n";

    //sounds

    //PlaySound(TEXT("sounds/test.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    //PlaySound(TEXT("sounds/MenuStartUpSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    //PlaySound(TEXT("sounds/StartGameSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    //PlaySound(TEXT("sounds/CarEngineSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    //PlaySound(TEXT("sounds/DeathSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    //initializing GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Race Car Simulation");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);


    glutMainLoop();
    return 0;
}
