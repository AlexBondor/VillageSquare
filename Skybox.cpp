#include "Skybox.h"

unsigned int skybox[6];
GLfloat skybox_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat skybox_ambient[] = { 1.0, 1.0, 1.0, 1.0 };

void initSkybox()
{
	skybox[SKY_RIGHT] = loadTexture("data/Skybox/right.bmp");
	skybox[SKY_LEFT] = loadTexture("data/Skybox/left.bmp");
	skybox[SKY_FRONT] = loadTexture("data/Skybox/front.bmp");
	skybox[SKY_BACK] = loadTexture("data/Skybox/back.bmp");
	skybox[SKY_TOP] = loadTexture("data/Skybox/top.bmp");
	skybox[SKY_BOTTOM] = loadTexture("data/Skybox/bottom.bmp");
}

void killSkybox()
{
	glDeleteTextures(6, skybox);
}

void drawSkybox(int skyboxSize)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, skybox_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, skybox_ambient);

	int X = skyboxSize;
	int Y = skyboxSize / 6;
	glEnable(GL_TEXTURE_2D);
	// left face
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_LEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-X, -Y, X);
	glTexCoord2f(1, 0); glVertex3f(-X, -Y, -X);
	glTexCoord2f(1, 1); glVertex3f(-X, Y, -X);
	glTexCoord2f(0, 1); glVertex3f(-X, Y, X);
	glEnd();

	// right face
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_RIGHT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);	glVertex3f(X, -Y, -X);
	glTexCoord2f(1, 0); glVertex3f(X, -Y, X);
	glTexCoord2f(1, 1); glVertex3f(X, Y, X);
	glTexCoord2f(0, 1); glVertex3f(X, Y, -X);
	glEnd();

	// front face
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_FRONT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-X, -Y, -X);
	glTexCoord2f(1, 0); glVertex3f(X, -Y, -X);
	glTexCoord2f(1, 1); glVertex3f(X, Y, -X);
	glTexCoord2f(0, 1); glVertex3f(-X, Y, -X);
	glEnd();

	// back face
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_BACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(X, -Y, X);
	glTexCoord2f(1, 0); glVertex3f(-X, -Y, X);
	glTexCoord2f(1, 1); glVertex3f(-X, Y, X);
	glTexCoord2f(0, 1); glVertex3f(X, Y, X);
	glEnd();

	// top face
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_TOP]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-X, Y, -X);
	glTexCoord2f(1, 0); glVertex3f(X, Y, -X);
	glTexCoord2f(1, 1); glVertex3f(X, Y, X);
	glTexCoord2f(0, 1); glVertex3f(-X, Y, X);
	glEnd();

	// bottom face
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_BOTTOM]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-X, -Y, X);
	glTexCoord2f(1, 0); glVertex3f(X, -Y, X);
	glTexCoord2f(1, 1); glVertex3f(X, -Y, -X);
	glTexCoord2f(0, 1); glVertex3f(-X, -Y, -X);
	glEnd();
}