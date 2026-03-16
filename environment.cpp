#include "environment.h"
#include <GL/glut.h>
#include <cmath>


static void drawPyramidRoof(float w, float h, float d) {
    glBegin(GL_TRIANGLES);
        // Front Face
        glNormal3f(0.0f, 0.5f, 1.0f);
        glVertex3f(0.0f, h, 0.0f);
        glVertex3f(-w / 2, 0.0f, d / 2);
        glVertex3f(w / 2, 0.0f, d / 2);

        // Right Face
        glNormal3f(1.0f, 0.5f, 0.0f);
        glVertex3f(0.0f, h, 0.0f);
        glVertex3f(w / 2, 0.0f, d / 2);
        glVertex3f(w / 2, 0.0f, -d / 2);

        // Back Face
        glNormal3f(0.0f, 0.5f, -1.0f);
        glVertex3f(0.0f, h, 0.0f);
        glVertex3f(w / 2, 0.0f, -d / 2);
        glVertex3f(-w / 2, 0.0f, -d / 2);

        // Left Face
        glNormal3f(-1.0f, 0.5f, 0.0f);
        glVertex3f(0.0f, h, 0.0f);
        glVertex3f(-w / 2, 0.0f, -d / 2);
        glVertex3f(-w / 2, 0.0f, d / 2);
    glEnd();
}

static void drawHouse(float x, float z, float r, float g, float b, float angle) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(angle, 0, 1, 0);

    float width = 5.0f;
    float height = 4.0f;
    float depth = 5.0f;

    // Main Body
    glColor3f(r, g, b);
    glPushMatrix();
    glTranslatef(0.0f, height / 2, 0.0f);
    glScalef(width, height, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Roof
    glColor3f(0.35f, 0.15f, 0.1f); // Darker, natural roof
    glPushMatrix();
    glTranslatef(0.0f, height, 0.0f);
    drawPyramidRoof(width + 0.8f, 2.5f, depth + 0.8f);
    glPopMatrix();

    // Chimney
    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(width / 4, height + 1.5f, -depth / 4);
    glScalef(0.8f, 2.0f, 0.8f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Door
    glColor3f(0.3f, 0.15f, 0.05f);
    glPushMatrix();
    glTranslatef(0.0f, 1.25f, depth / 2 + 0.05f);
    glScalef(1.2f, 2.5f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Door Step
    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0f, 0.1f, depth / 2 + 0.6f);
    glScalef(1.6f, 0.2f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Window
    glColor3f(0.7f, 0.85f, 1.0f);
    glPushMatrix();
    glTranslatef(1.5f, 2.0f, depth / 2 + 0.05f);
    glScalef(1.2f, 1.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

static void drawSkyscraper(float x, float z, float h, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, h / 2, z);

    float width = 6.0f;
    float depth = 6.0f;

    // Main Tower
    glColor3f(r, g, b);
    glPushMatrix();
    glScalef(width, h, depth);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Glass Strips
    glColor3f(0.5f, 0.7f, 0.9f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, depth / 2 + 0.05f);
    glScalef(width * 0.7f, h * 0.95f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Rooftop Mechanical Room
    glColor3f(0.4f, 0.4f, 0.45f);
    glPushMatrix();
    glTranslatef(0.0f, h / 2 + 1.0f, 0.0f);
    glScalef(width * 0.6f, 2.0f, depth * 0.6f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Small Antenna
    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0.0f, h / 2 + 3.0f, 0.0f);
    glScalef(0.1f, 2.0f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

static void drawStreetLamp(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // Base
    glColor3f(0.15f, 0.15f, 0.15f);
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 0.0f);
    glScalef(0.8f, 0.4f, 0.8f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pole
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f);
    glScalef(0.3f, 8.0f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Arm
    glPushMatrix();
    glTranslatef(-1.0f, 7.8f, 0.0f);
    glScalef(2.0f, 0.2f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Light Fixture
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(-2.0f, 7.8f, 0.0f);
    glScalef(0.6f, 0.4f, 0.6f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Light Bulb (Glowing)
    if (!isDay){
        glDisable(GL_LIGHTING);
    }
    glColor3f(1.0f, 1.0f, 0.7f);
    glPushMatrix();
    glTranslatef(-2.0f, 7.5f, 0.0f);
    glutSolidSphere(0.3f, 8, 8);
    glPopMatrix();

    if (!isDay){
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();
}

static void drawPineTree(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // Trunk
    glColor3f(0.35f, 0.2f, 0.1f);
    glPushMatrix();
    glScalef(0.6f, 2.5f, 0.6f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Leaves (3 layers)
    glColor3f(0.1f, 0.35f, 0.1f);

    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(3.0f, 4.0f, 15, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 3.5f, 0.0f);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(2.5f, 3.5f, 15, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 5.0f, 0.0f);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(1.8f, 3.0f, 15, 1);
    glPopMatrix();

    glPopMatrix();
}

static void drawOakTree(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // Trunk with base
    glColor3f(0.4f, 0.25f, 0.1f);

    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 0.0f);
    glScalef(1.2f, 0.6f, 1.2f);
    glutSolidCube(1.0f); // Base
    glPopMatrix();

    glPushMatrix();
    glScalef(0.8f, 3.5f, 0.8f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glutSolidCube(1.0f); // Trunk
    glPopMatrix();

    // Leaves
    glColor3f(0.15f, 0.5f, 0.1f);

    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f);
    glutSolidSphere(2.2f, 12, 12);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5f, 3.5f, 0.5f);
    glutSolidSphere(1.6f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5f, 3.8f, -0.5f);
    glutSolidSphere(1.7f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, 4.5f, 1.5f);
    glutSolidSphere(1.4f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5f, 3.2f, -1.8f);
    glutSolidSphere(1.5f, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

static void drawPoliceman(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(1.5f, 1.5f, 1.5f);

    // Shoes (Black)
    glColor3f(0.1f, 0.1f, 0.1f);

    glPushMatrix();
    glTranslatef(-0.15f, 0.0f, 0.05f);
    glScalef(0.2f, 0.1f, 0.35f);
    glutSolidCube(1.0f); // Left Shoe
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0.05f);
    glScalef(0.2f, 0.1f, 0.35f);
    glutSolidCube(1.0f); // Right Shoe
    glPopMatrix();

    // Legs
    glColor3f(0.05f, 0.05f, 0.3f);

    glPushMatrix();
    glTranslatef(-0.15f, 0.3f, 0.0f);
    glScalef(0.15f, 0.6f, 0.25f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15f, 0.3f, 0.0f);
    glScalef(0.15f, 0.6f, 0.25f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Body
    glColor3f(0.1f, 0.1f, 0.6f);
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, 0.0f);
    glScalef(0.5f, 0.6f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Arms
    glColor3f(0.1f, 0.1f, 0.6f);

    glPushMatrix();
    glTranslatef(-0.33f, 0.9f, 0.0f);
    glScalef(0.12f, 0.55f, 0.2f);
    glutSolidCube(1.0f); // Left Arm
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.33f, 0.9f, 0.0f);
    glScalef(0.12f, 0.55f, 0.2f);
    glutSolidCube(1.0f); // Right Arm
    glPopMatrix();

    // Hands
    glColor3f(0.9f, 0.75f, 0.6f);

    glPushMatrix();
    glTranslatef(-0.33f, 0.6f, 0.0f);
    glutSolidSphere(0.1f, 10, 10); // Left Hand
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.33f, 0.6f, 0.0f);
    glutSolidSphere(0.1f, 10, 10); // Right Hand
    glPopMatrix();

    // Head
    glColor3f(0.9f, 0.75f, 0.6f);
    glPushMatrix();
    glTranslatef(0.0f, 1.35f, 0.0f);
    glutSolidSphere(0.25f, 12, 12);
    glPopMatrix();

    // Police Utility Belt
    glColor3f(0.0f, 0.0f, 0.0f); // Black belt
    glPushMatrix();
    glTranslatef(0.0f, 0.65f, 0.0f); // Position at waist
    glScalef(0.55f, 0.1f, 0.35f);    // Slightly wider than the body
    glutSolidCube(1.0f);
    glPopMatrix();

    // Cap
    glColor3f(0.05f, 0.05f, 0.3f);
    glPushMatrix();
    glTranslatef(0.0f, 1.55f, 0.0f);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.05f, 0.25f, 12, 12);
    glutSolidSphere(0.25f, 12, 12);
    glPopMatrix();

    glPopMatrix();
}

static void drawPerson(float x, float z, float tcolor){
    glPushMatrix();

    glTranslatef(x, 0.0f, z);
    glScalef(1.5f, 1.5f, 1.5f);

    // Shoes (Black)
    glColor3f(0.1f, 0.1f, 0.1f);

    glPushMatrix();
    glTranslatef(-0.15f, 0.0f, 0.05f);
    glScalef(0.2f, 0.1f, 0.35f);
    glutSolidCube(1.0f); // Left Shoe
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0.05f);
    glScalef(0.2f, 0.1f, 0.35f);
    glutSolidCube(1.0f); // Right Shoe
    glPopMatrix();

    // Legs
    glColor3f(0.05f, 0.05f, 0.3f);

    glPushMatrix();
    glTranslatef(-0.15f, 0.3f, 0.0f);
    glScalef(0.15f, 0.6f, 0.25f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15f, 0.3f, 0.0f);
    glScalef(0.15f, 0.6f, 0.25f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Body
    glColor3f(0.5f, tcolor, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, 0.0f);
    glScalef(0.5f, 0.6f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Arms
    glColor3f(0.5f, tcolor, 0.2f);

    glPushMatrix();
    glTranslatef(-0.33f, 0.9f, 0.0f);
    glScalef(0.12f, 0.55f, 0.2f);
    glutSolidCube(1.0f); // Left Arm
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.33f, 0.9f, 0.0f);
    glScalef(0.12f, 0.55f, 0.2f);
    glutSolidCube(1.0f); // Right Arm
    glPopMatrix();

    // Hands
    glColor3f(0.9f, 0.75f, 0.6f);

    glPushMatrix();
    glTranslatef(-0.33f, 0.6f, 0.0f);
    glutSolidSphere(0.1f, 10, 10); // Left Hand
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.33f, 0.6f, 0.0f);
    glutSolidSphere(0.1f, 10, 10); // Right Hand
    glPopMatrix();

    // Head
    glColor3f(0.9f, 0.75f, 0.6f);
    glPushMatrix();
    glTranslatef(0.0f, 1.35f, 0.0f);
    glutSolidSphere(0.25f, 12, 12);
    glPopMatrix();

    // Cap
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 1.55f, 0.0f);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.05f, 0.25f, 12, 12);
    glutSolidSphere(0.25f, 12, 12);
    glPopMatrix();

    glPopMatrix();
}

// Main Environment Drawing Function
void drawEnvironment() {

    // --- 1. Ground (Grass) ---
    glColor3f(0.3f, 0.45f, 0.25f);
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0); // Normal facing UP
        glVertex3f(-150.0f, -0.55f, -150.0f);
        glVertex3f(-150.0f, -0.55f,  150.0f);
        glVertex3f( 150.0f, -0.55f,  150.0f);
        glVertex3f( 150.0f, -0.55f, -150.0f);
    glEnd();

    float roadWidth = 12.0f;

    // Confirmed correct height settings
    float roadY = -0.44f;
    float stripeY = -0.41f; // Highest
    float swY = -0.45f;     // Under the road (lowest)
    float swWidth = 3.5f;

    float roadRightStart = 120.0f;
    float roadCornerX = -33.0f;
    float roadTopEnd = -140.0f;
    float roadZOffset = -15.0f;

    // --- 2. Sidewalks ---
    glColor3f(0.35f, 0.35f, 0.35f);
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);

        // Horizontal
        glVertex3f(roadCornerX - roadWidth / 2 - swWidth, swY, roadZOffset - roadWidth / 2 - swWidth);
        glVertex3f(roadCornerX - roadWidth / 2 - swWidth, swY, roadZOffset + roadWidth / 2 + swWidth);
        glVertex3f(roadRightStart, swY, roadZOffset + roadWidth / 2 + swWidth);
        glVertex3f(roadRightStart, swY, roadZOffset - roadWidth / 2 - swWidth);

        // Vertical
        glVertex3f(roadCornerX - roadWidth / 2 - swWidth, swY, roadTopEnd);
        glVertex3f(roadCornerX + roadWidth / 2 + swWidth, swY, roadTopEnd);
        glVertex3f(roadCornerX + roadWidth / 2 + swWidth, swY, roadZOffset - roadWidth / 2 - swWidth);
        glVertex3f(roadCornerX - roadWidth / 2 - swWidth, swY, roadZOffset - roadWidth / 2 - swWidth);
    glEnd();

    // --- 3. Asphalt Road (BLACK) ---
    glColor3f(0.05f, 0.05f, 0.05f); // Deep Black
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0); // Explicit normal

        // Horizontal
        glVertex3f(roadCornerX - roadWidth / 2, roadY, roadZOffset - roadWidth / 2);
        glVertex3f(roadCornerX - roadWidth / 2, roadY, roadZOffset + roadWidth / 2);
        glVertex3f(roadRightStart, roadY,  roadZOffset + roadWidth / 2);
        glVertex3f(roadRightStart, roadY,  roadZOffset - roadWidth / 2);

        // Vertical
        glVertex3f(roadCornerX - roadWidth / 2, roadY, roadZOffset - roadWidth / 2);
        glVertex3f(roadCornerX + roadWidth / 2, roadY, roadZOffset - roadWidth / 2);
        glVertex3f(roadCornerX + roadWidth / 2, roadY, roadTopEnd);
        glVertex3f(roadCornerX - roadWidth / 2, roadY, roadTopEnd);
    glEnd();

    // --- 4. Road Markings (White) ---
    glColor3f(1.0f, 1.0f, 1.0f);
    float stripeLen = 3.0f;
    float gapLen = 4.0f;
    float stripeW = 0.4f;

    // Set normal once for all markings
    glNormal3f(0, 1, 0);

    for (float x = roadRightStart - 2.0f; x > roadCornerX; x -= (stripeLen + gapLen)) {
        glBegin(GL_QUADS);
        glVertex3f(x - stripeLen, stripeY, roadZOffset - stripeW / 2);
        glVertex3f(x - stripeLen, stripeY, roadZOffset + stripeW / 2);
        glVertex3f(x, stripeY, roadZOffset + stripeW / 2);
        glVertex3f(x, stripeY, roadZOffset - stripeW / 2);
        glEnd();
    }
    for (float z = (roadZOffset - roadWidth / 2); z > roadTopEnd; z -= (stripeLen + gapLen)) {
        glBegin(GL_QUADS);
        glVertex3f(roadCornerX - stripeW / 2, stripeY, z);
        glVertex3f(roadCornerX + stripeW / 2, stripeY, z);
        glVertex3f(roadCornerX + stripeW / 2, stripeY, z - stripeLen);
        glVertex3f(roadCornerX - stripeW / 2, stripeY, z - stripeLen);
        glEnd();
    }

    // --- 5. Placed Objects ---
    drawPineTree(5.0f, -35.0f);
    drawPineTree(25.0f, -45.0f);
    drawOakTree(-10.0f, -55.0f);
    drawOakTree(15.0f, -65.0f);

    drawOakTree(45.0f, -2.0f);
    drawOakTree(75.0f, -2.0f);
    drawPineTree(-45.0f, -105.0f);

    drawStreetLamp(15.0f, -6.0f);
    drawStreetLamp(50.0f, -6.0f);
    drawStreetLamp(85.0f, -6.0f);
    drawStreetLamp(-23.0f, -45.0f);
    drawStreetLamp(-23.0f, -80.0f);

    if (isDay){
        // make people only appear during day time as he is off-duty at night

        /* the actual X and Z locations of the people
        drawPoliceman(20.0f, -22.0f);
        drawPerson(-15.0f, -22.0f, 0.5f);
        drawPerson(0.0f, -22.0f, 0.7f);
        drawPerson(-5.0f, -7.0f, 0.3f);
        drawPerson(-20.0f, -7.0f, 0.9f);
        drawPerson(-45.0f, -50.0f, 0.0f);
        drawPoliceman(-25.0f, -70.0f);*/

        for (int i=0; i<PERSON_COUNT; i++){
            if (personVisible[i]){
                drawPerson(personX[i], personZ[i], personColor[i]);
            }
        }
        for (int j=0; j<PERSON_COUNT; j++){
            if (policeVisible[j]){
                drawPoliceman(policeX[j], policeZ[j]);
            }
        }
    }

    // Houses
    for (float x = -30; x < 30; x += 20) {
        float r = (int(x) % 50 == 0) ? 0.85f : 0.75f;
        float g = (int(x) % 50 == 0) ? 0.75f : 0.85f;
        drawHouse(x, -2.0f, r, g, 0.65f, 0.0f);
    }

    // Skyscrapers
    drawSkyscraper(-50.0f, -35.0f, 28.0f, 0.5f, 0.5f, 0.55f);
    drawSkyscraper(-50.0f, -60.0f, 40.0f, 0.4f, 0.4f, 0.45f);
    drawSkyscraper(-50.0f, -85.0f, 22.0f, 0.6f, 0.6f, 0.65f);

    drawSkyscraper(60.0f, -40.0f, 20.0f, 0.55f, 0.45f, 0.45f);
}

