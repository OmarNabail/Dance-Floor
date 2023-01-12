#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <glut.h>
#include <random>
#include <time.h>
#include <iostream>
#include <Windows.h>

using namespace std;

double boarderZ = 9;
double boarderX = 5;
double rotatecam = 0;

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
#define PI 3.1415927

bool keyStates[256];

int WIDTH = 1280;
int HEIGHT = 720;
GLdouble speedfactor = 1;
float finishfactor = 1;
float obs0 = -60;
float coin0 = -60;
float acc0 = 0;
float roadanim = 0;
float distance0 = 0;
float ground = 0;
int level = 0;
int cameraupcount = 0;
int camera3upcount = 0;
float camerafactor = 0;
float camera3factor = 0;
float lightvar = 0;
bool start0 = false;
int startingcount = 0;
float red0 = 0;
float green0 = 0;
float blue0 = 0;
float posx = 0;
float roty = 0;
bool lost0 = false;
float ring = 0;
float coinfall = 20;
int coinscollected = 0;
bool win0 = false;
float lightx = 0;
float car0 = 0;
float boost0 = -60;
GLuint tex;
GLuint tex2;
GLuint tex3;
char title[] = "fast cars";
bool timesup = false;
int usingcamera2 = 0;
bool coinlimiter = false;
float rotationcoins = 0;









void jumpAnim();
bool dancing = false;
double red = 0;
double green = 0;
double blue = 0;

float armRot = 0;
float headRot = 0;
float legRot = 0;
float legF = 0;
float rotAng;
float jumpY = 0;
float jumpY2 = 0;
float legJ1 = 0;
float legJ2 = 0;
float legT1 = 0;
float legT2 = 0;
bool up = true;
bool legB = 0;
bool sm = 0;
bool mu1 = 0;
bool mu2 = 0;
int ad = 1;
int hd = 1;
int ld = 1;
bool pov = 0;
int state = 0;

float R = 1;
float G = 0;
float B = 0;
float Rr = 1;
float Gr = 1;
float Br = 1;
float Rt = 0;
float Gt = 0;
float Bt = 0;
bool Rb = false;
bool Gb = false;
bool Bb = false;

float counters = 0;
float counter = 0;
//random vals
float r0 = 0;
float r1 = 0;
float r2 = 0;
float r3 = 0;
float r4 = 0;
float r5 = 0;
float r6 = 0;
float r7 = 0;
float r8 = 0;
float r9 = 0;
float r10 = 0;
float r11 = 0;
int Dance = 1;

int w;
///

float player1XP = 5;
float player1ZP = 9;
float player1R = 0;




















GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;
float maxboost = 70.0;

void draw_cylinder(GLfloat radius, GLfloat height, GLubyte R, GLubyte G, GLubyte B)
{
	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat angle = 0.0;
	GLfloat angle_stepsize = 0.1;

	glColor3ub(R - 40, G - 40, B - 40);
	glBegin(GL_QUAD_STRIP);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		glVertex3f(x, y, 0.0);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glVertex3f(radius, 0.0, 0.0);
	glEnd();

	glColor3ub(R, G, B);
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glEnd();
}



class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};
class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 0.296457, float eyeY = 5.02793, float eyeZ = 10.9389, float centerX = 0.0291684, float centerY = 0.955638, float centerZ = -0.293096, float upX = 0.274483, float upY = 4.73539, float upZ = 9.98293) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};
void output(int x, int y, int z, float r, float g, float b, char* string)
{
	glScalef(20, 20, 20);
	glColor3f(r, g, b);
	glRasterPos3f(x, y, z);
	int len, i;
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}
Camera camera;
Camera camera2;
Camera camera3;


Model_3DS model_car;



Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_chair;
Model_3DS model_table;


GLTexture tex_ground;
GLTexture tex_ground2;
GLTexture tex_gameover;
GLTexture tex_winscreen;
GLTexture tex_timesup;

void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, lightvar, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}
void handleCoins(int t) {
	rotationcoins += 50;
	roty += rotationcoins;
	if (rotationcoins < 3600)glutTimerFunc(20, handleCoins, 0);
	else {
		rotationcoins = 0;
		coinscollected += 1;
		coinlimiter = false;
	}
}
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}















void renderDancefloor()
{
	glPushMatrix();
	glColor3f(R, G, B);
	glTranslatef(0, -0.4, 0);
	glScaled(10, 1, 10);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1 - R, 1 - G, 1 - B);
	glTranslatef(10, -0.4, 0);
	glScaled(10, 1, 10);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(R, G, B);
	glTranslatef(10, -0.4, 10);
	glScaled(10, 1, 10);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1 - R, 1 - G, 1 - B);
	glTranslatef(0, -0.4, 10);
	glScaled(10, 1, 10);
	glutSolidCube(1);
	glPopMatrix();
}


void drawMortyHead() {
	glPushMatrix();
	glColor3d(0.988, 0.788, 0.628);
	if (dancing)
		glRotated(headRot, 1, 0, 0);

	glScaled(0.9, 1.1, 1);

	glutSolidSphere(0.08, 40, 40);
	glPopMatrix();

}
void drawMortyEye(int eyePosition) {
	double eyeTranslationDirection = 1;
	if (eyePosition == 0) {
		eyeTranslationDirection *= -1;
	}

	glPushMatrix();
	glColor3f(1, 1, 1);
	if (dancing)
		glRotated(headRot, 1, 0, 0);

	glTranslated(0.04 * eyeTranslationDirection, 0.03, 0.065);
	glScaled(1.2, 1.2, 1);

	glutSolidSphere(0.02, 40, 40);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	if (dancing)
		glRotated(headRot, 1, 0, 0);
	glTranslated(0.04 * eyeTranslationDirection, 0.03, 0.088);

	glutSolidSphere(0.004, 40, 40);
	glPopMatrix();
}

void drawMortyEyes() {
	drawMortyEye(0);
	drawMortyEye(1);
}

void drawMortyEar(int earPosition) {
	double earTranslationDirection = 1;
	if (earPosition == 0) {
		earTranslationDirection *= -1;
	}

	glPushMatrix();
	glColor3d(0.988, 0.788, 0.628);
	if (dancing)
		glRotated(headRot, 1, 0, 0);
	glTranslated(0.07 * earTranslationDirection, -0.013, 0);

	glutSolidSphere(0.02, 40, 40);
	glPopMatrix();

}
void drawMortyNose() {
	glPushMatrix();
	if (dancing) {
		glRotated(headRot, 1, 0, 0);
		glRotated(30 + headRot, 0, 0, 1);
	}

	glTranslated(0, -0.01, 0.1 - 0.02);
	glScaled(0.6, 1.6, 1);

	glutSolidCube(0.02);
	glPopMatrix();

	//Nose Border
	glPushMatrix();
	glColor3d(0, 0, 0);
	if (dancing)
	{
		glRotated(headRot, 1, 0, 0);
		glRotated(30 + headRot, 0, 0, 1);
	}

	glTranslated(0, -0.01, 0.09 - 0.02);
	glScaled(0.7, 1.6, 1);

	glutSolidCube(0.021);
	glPopMatrix();
}

void drawMortyMouth() {
	glPushMatrix();
	glColor3d(0, 0, 0);
	if (dancing)
		glRotated(headRot, 1, 0, 0);
	glTranslated(0, -0.045, 0.07);
	glScaled(1.5, 0.3, 0.1);

	glutSolidCube(0.02);
	glPopMatrix();
}
void drawMortyHair() {
	glPushMatrix();
	glColor3d(0.452, 0.272, 0.156);
	if (dancing)
		glRotated(headRot, 1, 0, 0);
	glTranslated(0, 0.08, 0);
	glScaled(0.9, 0.4, 0.9);

	glutSolidSphere(0.08, 40, 40);
	glPopMatrix();
}
void drawMortyFace() {
	drawMortyHead();
	drawMortyEyes();
	drawMortyEar(0);
	drawMortyEar(1);
	drawMortyNose();
	drawMortyMouth();
	drawMortyHair();
}
void drawMortyMainUpperBody() {
	glPushMatrix();
	glColor3d(1, 0.972, 0.415);
	glTranslated(0, -0.21, 0);
	glScaled(0.61, 1, 0.17);

	glutSolidCube(0.3);
	glPopMatrix();
}
void drawMortyArm(int handPosition) {
	double armDirection = 1;
	if (handPosition == 0) {
		armDirection *= -1;
	}

	// Shirt Sleeve
	glPushMatrix();
	glColor3d(1, 0.972, 0.415);
	if (armDirection == 1 && dancing)
		glRotated((180 * armDirection) + armRot, 0, 0, 1);
	else if (dancing)
		glRotated((180 * armDirection) - armRot, 0, 0, 1);
	glScaled(0.6, 1.35, 0.4);

	glTranslated(0.2 * armDirection, -0.12 + 0.12, 0);

	glutSolidCube(0.1);
	glPopMatrix();

	// Arm Skin
	glPushMatrix();
	glColor3d(0.988, 0.788, 0.628);
	if (armDirection == 1 && dancing)
		glRotated((180 * armDirection) + armRot, 0, 0, 1);
	else if (dancing)
		glRotated((180 * armDirection) - armRot, 0, 0, 1);
	glScaled(0.6, 1.6, 0.4);

	glTranslated(0.2 * armDirection, -0.193 + 0.10, 0);

	glTranslated(0.15 * armDirection, -0.193, 0);
	glutSolidCube(0.1);
	glPopMatrix();

	//Fist
	glPushMatrix();
	glColor3d(0.988, 0.788, 0.628);
	if (armDirection == 1 && dancing)
		glRotated((180 * armDirection && dancing) + armRot, 0, 0, 1);
	else if (dancing)
		glRotated((180 * armDirection) - armRot, 0, 0, 1);
	glTranslated(0.12 * armDirection, -0.41 + 0.2, 0);
	glScaled(1, 0.7, 1);

	glutSolidSphere(0.04, 40, 40);
	glPopMatrix();
}
void drawMortyArmb(int handPosition) {
	double armDirection = 1;
	if (handPosition == 0) {
		armDirection *= -1;
	}

	// Shirt Sleeve
	glPushMatrix();
	glColor3d(1, 0.972, 0.415);
	if (armDirection == 1 && dancing)
		glRotated((30 * armDirection), -1, 0, 0.2 + legRot);
	else if (dancing)
		glRotated((30 * armDirection), 1, 0, 0.2 - legRot);
	glScaled(0.6, 1.35, 0.4);
	glTranslated(0.15 * armDirection, -0.12, 0);

	glutSolidCube(0.1);
	glPopMatrix();

	// Arm Skin
	glPushMatrix();
	glColor3d(0.988, 0.788, 0.628);
	if (armDirection == 1 && dancing)
		glRotated((30 * armDirection), -1, 0, 0.2 + legRot);
	else if (dancing)
		glRotated((30 * armDirection), 1, 0, 0.2 - legRot);
	glScaled(0.6, 1.6, 0.4);

	glTranslated(0.15 * armDirection, -0.193, 0);

	glutSolidCube(0.1);
	glPopMatrix();

	//Fist
	glPushMatrix();
	glColor3d(0.988, 0.788, 0.628);
	if (armDirection == 1 && dancing)
		glRotated((30 * armDirection), -1, 0, 0.2 + legRot);
	else if (dancing)
		glRotated((30 * armDirection), 1, 0, 0.2 - legRot);

	glTranslated(0.09 * armDirection, -0.41, 0);
	glScaled(1, 0.7, 1);

	glutSolidSphere(0.04, 40, 40);
	glPopMatrix();
}

void drawMortyLeg(int legPosition) {
	double LegDirection = 1;
	if (legPosition == 0) {
		LegDirection *= -1;
	}

	glPushMatrix();
	glColor3d(0.188, 0.266, 0.403);

	glScaled(0.8, 3, 0.4);

	glTranslated(-0.064 * LegDirection, -0.2, 0);

	glutSolidCube(0.1);
	glPopMatrix();

	// Foot
	glPushMatrix();
	glColor3d(1, 1, 1);

	glTranslated(-0.05 * LegDirection, -0.765, 0);
	glScaled(1.5, 0.6, 1);

	glutSolidSphere(0.03, 40, 40);
	glPopMatrix();
}
void drawMortyUpperBody() {
	drawMortyMainUpperBody();
	if (counters < 20 && dancing) {

		drawMortyArm(0);
		drawMortyArm(1);
	}
	else if (counters < 40 && dancing) {
		drawMortyArm(0);
		drawMortyArm(1);
		jumpAnim();
	}
	else if (counters < 60 && dancing) {
		jumpY = 0;
		legT1 = 0;
		legT2 = 0;
		legJ1 = 0;
		drawMortyArmb(0);
		drawMortyArmb(1);
	}
	else if (counters < 80 && dancing) {
		legT1 = 0;
		legT2 = 0;
		legJ1 = 0;
		jumpY = 0;
		drawMortyArm(0);
		drawMortyArm(1);
	}

	else if (counters < 100 && dancing) {
		jumpY = 0;
		legT1 = 0;
		legT2 = 0;
		legJ1 = 0;
		drawMortyArmb(0);
		drawMortyArmb(1);
	}
	else if (counters < 120 && dancing) {
		drawMortyArm(0);
		drawMortyArm(1);
		jumpAnim();
	}
	else if (counters < 140 && dancing) {
		jumpY = 0;
		legT1 = 0;
		legT2 = 0;
		legJ1 = 0;
		drawMortyArm(0);
		drawMortyArm(1);
	}
	else if (counters < 160 && dancing) {
		drawMortyArmb(0);
		drawMortyArmb(1);
	}
	else if (counters < 180 && dancing) {
		drawMortyArm(0);
		drawMortyArm(1);
		jumpAnim();
	}
	else if (counters < 210 && dancing) {
		jumpY = 0;
		legT1 = 0;
		legT2 = 0;
		legJ1 = 0;
		drawMortyArmb(0);
		drawMortyArmb(1);
	}

	else if (counters < 221 && dancing) {
		Dance = 2;
		drawMortyArm(0);
		drawMortyArm(1);

	}
	else if (counters < 240 && dancing) {

		jumpAnim();
		drawMortyArm(0);
		drawMortyArm(1);
	}
	else if (counters < 270 && dancing) {
		jumpY = 0;
		legT1 = 0;
		legT2 = 0;
		legJ1 = 0;
		drawMortyArmb(0);
		drawMortyArmb(1);
		jumpAnim();
	}

	else if (counters < 285 && dancing) {
		jumpY = 0;
		legT1 = 0;
		legT2 = 0;
		legJ1 = 0;
		drawMortyArm(0);
		drawMortyArm(1);
	}
	else if (counters < 310 && dancing) {
		drawMortyArmb(0);
		drawMortyArmb(1);
		jumpAnim();
	}
	else if (counters < 320 && dancing) {
		jumpY = 0;
		legT1 = 0;
		legT2 = 0;
		legJ1 = 0;
		drawMortyArm(0);
		drawMortyArm(1);
	}
	else if (counters < 330 && dancing) {
		drawMortyArmb(0);
		drawMortyArmb(1);
	}
	else if (counters < 340 && dancing) {
		drawMortyArm(0);
		drawMortyArm(1);
		jumpAnim();
	}
	else if (counters < 350 && dancing) {
		jumpY = 0;
		legT1 = 0;
		legT2 = 0;
		legJ1 = 0;
		drawMortyArmb(0);
		drawMortyArmb(1);
	}
	else {
		//output(-1, 1, -0.2, 1, 1, 1, "Thanks for watching Dance 1 !");
		drawMortyArm(0);
		drawMortyArm(1);

	}



}
void drawMortyLowerBody() {
	glPushMatrix();
	glColor3d(0.188, 0.266, 0.403);
	glScaled(1.8, 1, 0.5);

	glTranslated(0, -0.41, 0);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();

	glRotated(legJ1, 0, 0, 1);
	glTranslated(-0.13, 0.03, 0);
	glTranslated(legT1 * -1, legT2, 0);
	drawMortyLeg(0);

	glPopMatrix();

	glPushMatrix();

	glRotated(legJ1, 0, 0, -1);
	glTranslated(0.13, 0.03, 0);
	glTranslated(legT1, legT2, 0);
	drawMortyLeg(1);

	glPopMatrix();



}
void drawMorty() {
	if (dancing)
		glTranslated(0, jumpY, 0);
	drawMortyFace();
	drawMortyUpperBody();
	drawMortyLowerBody();
}
void jumpAnim() {
	if (jumpY < 0.3 && up == true) {
		if (sm == 0) {
			jumpY += 0.0015 * 2;
			legT1 += 0.00065 * 2;
			legT2 += 0.00015 * 2;
			legJ1 += 0.1 * 2;
			legJ2 += 0.1 * 2;
		}
		else {
			jumpY += (0.0015 * 2) / 4;
			legT1 += (0.00065 * 2) / 4;
			legT2 += (0.00015 * 2) / 4;
			legJ1 += (0.1 * 2) / 4;
			legJ2 += (0.1 * 2) / 4;
		}

	}
	else {
		up = false;
	}
	if (jumpY >= 0 && up == false) {

		if (sm == 0) {
			jumpY -= 0.0015 * 2;
			legT1 -= 0.00065 * 2;
			legT2 -= 0.00015 * 2;
			legJ1 -= 0.1 * 2;
			legJ2 += 0.1 * 2;
		}
		else {
			jumpY -= (0.0015 * 2) / 4;
			legT1 -= (0.00065 * 2) / 4;
			legT2 -= (0.00015 * 2) / 4;
			legJ1 -= (0.1 * 2) / 4;
			legJ2 -= (0.1 * 2) / 4;
		}
	}
	else {
		up = true;
	}


}
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(1, 1, 1);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-30, 0, -30);
	glTexCoord2f(5, 0);
	glVertex3f(30, 0, -30);
	glTexCoord2f(5, 5);
	glVertex3f(30, 0, 30);
	glTexCoord2f(0, 5);
	glVertex3f(-30, 0, 30);
	glEnd();
	glPopMatrix();


	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}



void renderStage()
{
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glTranslated(10, 0, -15);
	glScaled(20, 4, 15);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.4, 0.4);
	glTranslated(6, 0, -10);

	glScaled(1.5, 8, 1.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.4, 0.4);
	glTranslated(18, 0, -10);

	glScaled(1.5, 8, 1.5);
	glutSolidCube(1);
	glPopMatrix();
}
































void initsecondlevel(int t) {
	start0 = true;
}
void timesupscenario(int t) {
	timesup = true;
	lost0 = true;
}
void finishfactorfactor(int t) {
	finishfactor += 0.00005;
	if (finishfactor < 6)
		glutTimerFunc(50, finishfactorfactor, 0);
	else {
		roadanim = 0;
		distance0 = 0;
		level = 1;
		maxboost = 70;
		start0 = false;
		ground = 800;
		glutTimerFunc(1900, initsecondlevel, 0);
		glutTimerFunc(20000, timesupscenario, 0);
	}
}
void handlecamerajump(int t) {
	cameraupcount += 1;
	camera3upcount += 1.001;
	camerafactor = -0.14;
	camera3factor = -0.140001;
	camera.moveY(camerafactor);
	camera3.moveY(camera3factor);
	if (cameraupcount < 28.5714285714)
		glutTimerFunc(50, handlecamerajump, 0);
	else cameraupcount = 0;
}
void starting0(int t) {
	if (startingcount == 1) { red0 = 0.8; Beep(1100, 200); }
	if (startingcount == 2) { green0 = 1.0; Beep(1200, 400); }
	if (startingcount == 3) { blue0 = 0.0; red0 = 0.0; Beep(1500, 600); }
	startingcount += 1;
	if (startingcount < 4)
		glutTimerFunc(1200, starting0, 0);
	else {
		start0 = true;
		blue0 = 1.0;
		red0 = 0.8;
	}
}
void winanim(int t) {
	if (coinfall > 0)coinfall -= 1;
	glutTimerFunc(50, winanim, 0);
}
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(max(min(fovy * speedfactor, maxboost), 45.0), aspectRatio, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	if (usingcamera2 == 0)camera.look();
	else if (usingcamera2 == 1) camera2.look();
	else if (usingcamera2 == 2)camera3.look();

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

void RenderGround2()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.8, 0.8, 0.8);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground2.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(1, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(1, 1);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void RenderGameOver()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_gameover.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(1, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(1, 1);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void RenderYouWin()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_winscreen.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(1, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(1, 1);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void RenderTimesUp()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_timesup.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(1, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(1, 1);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myInit();


	GLfloat lightIntensity[] = { 1, 1, 1, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 50.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);


	GLfloat lightIntensity1[] = { 0.4, 0, 0, 1.0f };
	GLfloat lightPosition1[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity1);

	glPushMatrix();
	glRotatef(rotatecam, 0, 1, 0);
	glTranslatef(5, 0, 0);
	GLfloat lightIntensity2[] = { 0, 0, 0.3, 1.0f };
	GLfloat lightPosition2[] = { 10.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity2);

	glPopMatrix();
	RenderGround();
	renderDancefloor();
	glPushMatrix();


	glPushMatrix();
	glColor3f(1, 1, 0);
	for (int i = 0; i < coinscollected; i++) {
		glTranslatef(0.5, coinfall, 0);
		glRotatef(ring, 0, 1, 0);
		draw_cylinder(2, 1, 255, 255, 0);
	}
	glPopMatrix();

	glColor3f(1, 1, 1);
	glTranslatef(player1XP, 4, player1ZP);
	glScaled(5, 5, 5);
	glRotated(player1R, 0, 1, 0);
	drawMorty();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(10, 0, 0);
	glScalef(0.7, 0.7, 0.7);
	//	model_tree.Draw();
	glPopMatrix();

	// Draw house Model
	glPushMatrix();
	glRotatef(90.f, 1, 0, 0);
	//	model_house.Draw();
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.5, 0.7, 0.5);
	glTranslated(3, 7, -2);
	glutSolidSphere(2, 40, 40);
	glPopMatrix();

	glPushMatrix();
	glTranslated(25, 0, 15);
	glScaled(0.05, 0.05, 0.05);
	glRotated(90.0f, 0, 1, 0);
	model_chair.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(15, 0, 25);
	glScaled(0.05, 0.05, 0.05);
	glRotated(0, 0, 1, 0);
	model_chair.Draw();
	glPopMatrix();


	glPushMatrix();
	glTranslated(15, 1.5, 28);
	glScaled(0.04, 0.04, 0.04);
	glRotated(-90, 0, 1, 0);
	model_table.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(28, 1.5, 15);
	glScaled(0.04, 0.04, 0.04);
	glRotated(0, 0, 1, 0);
	model_table.Draw();
	glPopMatrix();


	//sky box
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);


	glPopMatrix();

	glPushMatrix();
	renderStage();
	glPopMatrix();

	glPushMatrix();

	glTranslated(32, 0, 0);
	glScaled(1, 1, 30);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-32, 0, 0);
	glScaled(1, 1, 30);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -32);
	glRotatef(90, 0, 1, 0);
	glScaled(1, 1, 30);
	
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, 32);
	glRotatef(90, 0, 1, 0);
	glScaled(1, 1, 30);

	glutSolidCube(2);
	glPopMatrix();









	glutSwapBuffers();
	glutPostRedisplay();
}

void idle() {
	if (lightx > 10)lightx -= 0.01; else lightx += 0.01;

	if (sm == 0)
		armRot += 0.15 * ad;
	else
		armRot += (0.15 / 4) * ad;

	if (armRot < 0 || armRot > 25)
		ad *= -1;

	if (sm == 0)
		headRot += 0.3 * hd;
	else
		headRot += (0.3 / 4) * hd;

	if (headRot < 0 || headRot > 25)
		hd *= -1;

	if (sm == 0)
		legRot += 0.015 * ld;
	else
		legRot += (0.015 / 4) * ld;

	if (legRot < -2 || legRot > 2)
		ld *= -1;

	if (sm == 0)
		counter += 0.005;
	else
		counter += 0.005 / 4;


	if (sm == 0)
		counters += 0.003;
	else
		counters += 0.003 / 4;


	if (counter >= 1) {
		Rr = (float)rand() / (float)RAND_MAX;
		Gr = (float)rand() / (float)RAND_MAX;
		Br = (float)rand() / (float)RAND_MAX;
		r0 = (float)rand() / (float)RAND_MAX;
		r1 = (float)rand() / (float)RAND_MAX;
		r2 = (float)rand() / (float)RAND_MAX;
		r3 = (float)rand() / (float)RAND_MAX;
		r4 = (float)rand() / (float)RAND_MAX;
		r5 = (float)rand() / (float)RAND_MAX;
		r6 = (float)rand() / (float)RAND_MAX;
		r7 = (float)rand() / (float)RAND_MAX;
		r8 = (float)rand() / (float)RAND_MAX;
		r9 = (float)rand() / (float)RAND_MAX;
		r10 = (float)rand() / (float)RAND_MAX;
		r11 = (float)rand() / (float)RAND_MAX;
		counter = 0;
	}


	//cout << Rr;
	//cout << "\n";


	if (state == 0) {
		if (sm == 0)
			G += 0.005;
		else
			G += 0.005 / 4;
		if (G >= 1) {
			state = 1;
			G = 1;
		}
	}
	if (state == 1) {
		if (sm == 0)
			R -= 0.005;
		else
			R -= 0.005 / 4;

		if (R <= 0) {
			state = 2;
			R = 0;
		}
	}
	if (state == 2) {
		if (sm == 0)
			B += 0.005;
		else
			B += 0.005 / 4;
		if (B >= 1) {
			state = 3;
			B = 1;
		}
	}
	if (state == 3) {
		if (sm == 0)
			G -= 0.005;
		else
			G -= 0.005 / 4;
		if (G <= 0) {
			state = 4;
			G = 0;
		}
	}
	if (state == 4) {
		if (sm == 0)
			R += 0.005;
		else
			R += 0.005 / 4;
		if (R >= 1) {
			state = 5;
			R = 1;
		}
	}
	if (state == 5) {
		if (sm == 0)
			B -= 0.005;
		else
			B -= 0.005 / 4;
		if (B <= 0) {
			state = 0;
			B = 0;
		}
	}
	rotatecam += 0.5;
	red = (float)rand() / (float)RAND_MAX;;
	green = (float)rand() / (float)RAND_MAX;
	blue = (float)rand() / (float)RAND_MAX;



	glutPostRedisplay();
}
void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
	if (key == 'w') {
		speedfactor = 1;
		acc0 = 0;
	}
	if (key == 'v') {
		if (usingcamera2 == 0)camera.look();
		else if (usingcamera2 == 1) camera2.look();
		else if (usingcamera2 == 2)camera3.look();
		usingcamera2 += 1;
		usingcamera2 %= 3;
	}
}


bool checkCollision(float objectXP, float objectZP, float objectHeight, float objectWidth, float xStepValue, float zStepValue) {

	if ((player1XP + xStepValue) >= (objectXP - objectHeight) && (player1XP + xStepValue) <= (objectXP + objectHeight)) {
		if ((player1ZP + zStepValue) >= (objectZP - objectWidth) && (player1ZP + zStepValue) <= (objectZP + objectWidth)) {
			return true;
		}
	}
	return false;

}
void Keyboard(unsigned char key, int x, int y)
{
	float camcoef = 0.2;
	keyStates[key] = true;
	if (key == GLUT_KEY_ESCAPE)exit(EXIT_SUCCESS);
	if (key == '2') {
		cout << "\n camera eye : " << camera2.eye.x << " : " << camera2.eye.y << " : " << camera2.eye.z;
		cout << "\n camera up : " << camera2.up.x << " : " << camera2.up.y << " : " << camera2.up.z;
		cout << "\n camera center : " << camera2.center.x << " : " << camera2.center.y << " : " << camera2.center.z;
	}if (key == 'i') camera2.moveY(camcoef);
	if (key == 'k') camera2.moveY(-camcoef);
	if (key == 'j') camera2.moveX(camcoef);
	if (key == 'l') camera2.moveX(-camcoef);
	if (key == 'o') camera2.moveZ(camcoef);
	if (key == 'u') camera2.moveZ(-camcoef);


	/*switch (button)
	{
	case 't':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'r':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	*/

	if (key == 'd')
	{
		//if the letter d is pressed, then the object will be translated in the x axis by 10 (moving to the right).

		if (player1XP <= 29.5)
			if (!checkCollision(15, 25, 3, 1, 0.5, 0))
				if (!checkCollision(25, 15, 1, 3, 0.5, 0))
					if (!checkCollision(28, 15, 1, 1, 0.5, 0))
						if (!checkCollision(15, 28, 1, 1, 0.5, 0))
							if (!checkCollision(10, -15, 10, 7.5, 0.5, 0))
								player1XP += 0.5;
		player1R = 90;


		//if ((player1XP) >= (chair2XP - chairHeight) && (player1XP) <= (chair2XP + chairHeight))
			//if ((player1ZP) >= (chair2ZP - chairWidth) && (player1ZP) <= (chair2ZP + chairHeight))
				//printf("playerx > 5");
				//player1XP -= 0.5;



	}

	if (key == 'a')
	{
		//if the letter a is pressed, then the object will be translated in the x axis by -10 (moving to the left).
		if (player1XP >= -29.5)
			if (!checkCollision(15, 25, 3, 1, -0.5, 0))
				if (!checkCollision(25, 15, 1, 3, -0.5, 0))
					if (!checkCollision(28, 15, 1, 1, -0.5, 0))
						if (!checkCollision(15, 28, 1, 1, -0.5, 0))
							if (!checkCollision(10, -15, 10, 7.5, -0.5, 0))
								player1XP -= 0.5;
		player1R = -90;

	}


	if (key == 'w')
	{
		//if the letter w is pressed, then the object will be translated in the y axis by 10. (moving upwords)
		if (player1ZP <= 29.5)
			if (!checkCollision(15, 25, 3, 1, 0, 0.5))
				if (!checkCollision(25, 15, 1, 3, 0, 0.5))
					if (!checkCollision(28, 15, 1, 1, 0, 0.5))
						if (!checkCollision(15, 28, 1, 1, 0, 0.5))
							if (!checkCollision(10, -15, 10, 7.5, 0, 0.5))
								player1ZP += 0.5;

		player1R = 0;
	}

	if (key == 's')
	{
		//if the letter w is pressed, then the object will be translated in the y axis by -10. (moving downwords)
		if (player1ZP >= -29.5)
			if (!checkCollision(15, 25, 3, 1, 0, -0.5))
				if (!checkCollision(25, 15, 1, 3, 0, -0.5))
					if (!checkCollision(28, 15, 1, 1, 0, -0.5))
						if (!checkCollision(15, 28, 1, 1, 0, -0.5))
							if (!checkCollision(10, -15, 10, 7.5, 0, -0.5))
								player1ZP -= 0.5;

		player1R = 180;
	}
	if (key == 'r') {
		dancing = !dancing;
	}
	if (key == 'q') {
		counters += 10;
	}
	if (key == 'e') {
		counters = 205;
	}
	







	glutPostRedisplay();

}
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (usingcamera2 == 0)camera.look();
	else if (usingcamera2 == 1) camera2.look();
	else if (usingcamera2 == 2)camera3.look();
}
void Special(int key, int x, int y) {
	float a = 8.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera2.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera2.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera2.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera2.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void LoadAssets()
{
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");
	model_chair.Load("Models/chair/odesd2_B2_3ds.3ds");
	model_table.Load("Models/table/0139_3ds.3ds");

	// Loading texture files
	tex_ground.Load("Textures/floor.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;
	x = WIDTH - x;

	if (state == GLUT_DOWN)
	{
		camera2.eye.x = 0.296457;
		camera2.eye.y = 5.02793;
		camera2.eye.z = 10.9389;
		camera2.up.x = 0.274483;
		camera2.up.y = 4.73539;
		camera2.up.z = 9.98293;
		camera2.center.x = 0.0291684;
		camera2.center.y = 0.955638;
		camera2.center.z = -0.293096;
		cout << "mouse";
	}
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(myMouse);

	glutSpecialFunc(Special);
	glutReshapeFunc(myReshape);
	glutIdleFunc(idle);

	camera3.eye.x = -13.4768;
	camera3.eye.y = 2.39413;
	camera3.eye.z = 4.02489;
	camera3.up.x = 0.0322774;
	camera3.up.y = 0.997596;
	camera3.up.z = -0.0613194;
	camera3.center.x = -12.4785;
	camera3.center.y = 2.35896;
	camera3.center.z = 3.97826;

	camera.eye.x = 5.20474;
	camera.eye.y = 13.5713;
	camera.eye.z = 46.7827;
	camera.up.x = 0.0276092;
	camera.up.y = 0.947874;
	camera.up.z = -0.317446;
	camera.center.x = 5.18306;
	camera.center.y = 13.2544;
	camera.center.z = 45.8345;




	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}