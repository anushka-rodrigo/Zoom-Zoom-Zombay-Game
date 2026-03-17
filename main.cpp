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

bool isCrashed = false;
float zoom = 60.0f; //this is the default FOV (45 to 60)

enum State {MENU, PLAYING, GAMEOVER};
State current_state = MENU;

//for sounds
void playLoopSound(const char* file) {
    PlaySound(TEXT(file), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void playOneShot(const char* file) {
    PlaySound(TEXT(file), NULL, SND_FILENAME | SND_ASYNC);
}

void stopSound() {
    PlaySound(NULL, 0, 0);
}

// Function to render 2D text
void drawText(const char* text, float x, float y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // Set to window size
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING); // Ensure text isn't affected by lights
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

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
void restartGame();

void startGame(int value) {
    current_state = PLAYING;
    playLoopSound("sounds/CarEngineSound.wav");
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    // Start game on Spacebar
    if (key == ' ' && current_state == MENU) {
        stopSound();
        playOneShot("sounds/StartGameSound.wav");
        glutTimerFunc(2000, startGame, 0); //delays the starting of game by 2 seconds
        return;
    }

    if (key == 'r' || key == 'R') {
        restartGame();
        glutPostRedisplay();
        return;
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

    if (key == 'w' || key == 'W') {
        if (zoom > 5.0f){
                zoom -= 2.0f;
            }
    }

    if (key == 's' || key == 'S') {
        if (zoom < 120.0f){
                zoom += 2.0f;
            }
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
            current_state = GAMEOVER;
            carSpeed = 0.0f;
            stopSound(); // stop engine
            playOneShot("sounds/DeathSound.wav");
            glutPostRedisplay();
            return;
        }
    }

    for (int i = 0; i < HOUSE_COUNT; i++) {
        if (hitObstacle(carX, carZ, houseObstacles[i])) {
            isCrashed = true;
            current_state = GAMEOVER;
            carSpeed = 0.0f;
            stopSound(); // stop engine
            playOneShot("sounds/DeathSound.wav");
            glutPostRedisplay();
            return;
        }
    }

    for (int i = 0; i < BUILDING_COUNT; i++) {
        if (hitObstacle(carX, carZ, buildingObstacles[i])) {
            isCrashed = true;
            current_state = GAMEOVER;
            carSpeed = 0.0f;
            stopSound(); // stop engine
            playOneShot("sounds/DeathSound.wav");
            glutPostRedisplay();
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
void restartGame(){
    isCrashed = false;
    current_state = PLAYING;
    stopSound();
    playLoopSound("sounds/CarEngineSound.wav");

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

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void setupHeadlights() {

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (current_state == MENU){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Dark background for menu
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glColor3f(1.0f, 0.0f, 0.0f);
        drawText("ZOOM ZOOM ZOMBAY!!!", 300, 450);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText("INSTRUCTIONS:", 350, 380);
        drawText(" Use Arrow Keys to Drive", 300, 340);
        drawText("Crush the zombies with your Car!", 300, 310);
        drawText("Do not crash, the zombies will overrun you", 300, 280);
        drawText("There is no escape, crush as many as you can", 300, 250);
        glColor3f(1.0f, 1.0f, 0.0f);
        drawText("PRESS SPACE TO START", 310, 120);
    }
    else if(current_state == PLAYING){
        setupLighting();
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


        setupHeadlights(); //car headlights

        // Call drawing functions from other modules
        drawEnvironment();
        if (!isCrashed){
            drawCar();
        }
        else{
            //drawCrashedCar();
            current_state = GAMEOVER;
            glutPostRedisplay();
        }

    }
    else if (current_state == GAMEOVER){
        // 1. Draw the 3D World in the background
        setupLighting();
        gluLookAt(-5.0f, 30.0f, 15.0f, -5.0f, 0.0f, -25.0f, 0.0f, 1.0f, 0.0f);

        drawEnvironment();
        drawCrashedCar();

        // 2. Overlay Setup
        glDisable(GL_DEPTH_TEST); // IMPORTANT: Ignore depth so overlay/text stay on top
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_LIGHTING);

        // Draw the semi-transparent black tint
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor4f(0.0f, 0.0f, 0.0f, 0.7f); // 70% dark overlay
        glBegin(GL_QUADS);
            glVertex2f(0, 0);
            glVertex2f(800, 0);
            glVertex2f(800, 600);
            glVertex2f(0, 600);
        glEnd();

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        // 3. Draw the Game Over Text
        // Reset color to solid red/white before drawing
        glColor3f(1.0f, 0.0f, 0.0f);
        drawText("GAME OVER!", 350, 400);

        glColor3f(1.0f, 1.0f, 1.0f);
        drawText("The car has crashed.", 330, 360);

        glColor3f(1.0f, 0.0f, 0.0f);
        drawText("THE ZOMBIES ARE EATING YOU!.", 330, 330);
        drawText("NOOO!!!.", 330, 300);
        glColor3f(1.0f, 1.0f, 0.0f);
        drawText("PRESS 'R' TO RESTART", 330, 250);

        // 4. Cleanup: Restore states for the next frame
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
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

    //initializing GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Race Car Simulation");

    init();
    playLoopSound("sounds/MenuStartUpSound.wav");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);


    glutMainLoop();
    return 0;
}
