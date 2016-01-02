#include "VillageSquare.h"

typedef struct c
{
	float x, y, z;
}Camera;

Camera camera;

int screen_width = 1366,
	screen_height = 768,
	skyboxSize = 650;

bool keystates[256], 
toggleFog = false, 
togglePShadows = false,
toggleVShadows = false,
toggleWireframe = false;

float eyeAngleX = 0,
	  eyeAngleY = 0, 
	  rotateX = 0,
	  rotateY = 0,
	  rotateZ = 0,
	  sidewaysX = 0,
	  sidewaysZ = 0,
	  cameraSpeed = 0.015f;

// SUN light propreties
GLfloat sun_ambient[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat sun_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat sun_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat sun_position[] = { -650.0, 40.0, 0.0, 0.0 };

// FIRE light propreties
GLfloat fire_diffuse[] = { 1.0, 0.0, 0.0, 0.5 };
GLfloat fire_position[] = { 0.0, 3.0, 7.0, 1.0 };
GLfloat fire_direction[] = { 0.0, -1.0, 0.0};


GLfloat crow_diffuse[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat crow_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat wingY = 5, moveCrow = 20, rotateCrow = 0;
int wingDir = 1;
bool rotateCrowFlag = false;

GLfloat fogColor[] = { 0.42, 0.40, 0.36, 0.0 };

//equations of the planes on which the shadow is cast
GLfloat fvFloorPlaneEq[4];

//projected shadows matrices
GLfloat fvFloorShadowMat[16];

void initCamera()
{
	camera.x = -10;
	camera.y = 1.0;
	camera.z = 1;
}

void initKeys()
{
	for (int i = 0; i < 256; i++)
	{
		keystates[i] = false;
	}
}

void ComputePlaneEquations()
{
	//floor points
	GLfloat fvFloorP0[4] = { 75, 0, 75, 1.0 };
	GLfloat fvFloorP1[4] = { 75, 0, -75, 1.0 };
	GLfloat fvFloorP2[4] = { -75, 0, -75, 1.0 };

	PlaneEq(fvFloorPlaneEq, fvFloorP0, fvFloorP1, fvFloorP2);
}

void ComputeShadowMatrices()
{
	ComputeShadowMatrix(fvFloorShadowMat, fvFloorPlaneEq, sun_position);
}

void pulseFire(int value)
{
	switch (value)
	{
	case 1:
		fire_diffuse[0] = 0.8;
		fire_diffuse[1] = 0;
		fire_diffuse[2] = 0;
		break;
	case 2:
		fire_diffuse[0] = 1;
		fire_diffuse[1] = 0.1;
		fire_diffuse[2] = 0;
		break;
	case 3:
		fire_diffuse[0] = 0.8;
		fire_diffuse[1] = 0.05;
		fire_diffuse[2] = 0;
		break;
	case 4:
		fire_diffuse[0] = 1;
		fire_diffuse[1] = 0.07;
		fire_diffuse[2] = 0;
		break;
	}
	if (value == 4)
	{
		value = 1;
	}
	else
	{
		value++;
	}
	glutTimerFunc(200, pulseFire, value);
}

void initOpenGL()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearStencil(0);
	glClearDepth(1.0f);
	glShadeModel(GL_SMOOTH);
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 2000.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_MODELVIEW);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT0, GL_POSITION, sun_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_specular);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
	pulseFire(1);


	// load skybox
	initSkybox();

	// load map elements
	initMap();

	initCamera();

	initKeys();

	ComputePlaneEquations();
	ComputeShadowMatrices();
}

void drawCrow()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, crow_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, crow_ambient);

	glPushMatrix();
	if (!rotateCrowFlag)
	{
		glTranslatef(16, 2, moveCrow);
	}
	else
	{
		// rotate around a point
		glTranslatef(13, 2, moveCrow);
		glRotatef(rotateCrow, 0, 1, 0);
		glTranslatef(3, 0, 0);
	}
	glBegin(GL_TRIANGLES);
	//right wing
	glVertex3f(0, 5, 0);
	glVertex3f(0.5, wingY, 0.2); // end of wing
	glVertex3f(0, 5, 0.2);

	//left wing
	glVertex3f(0, 5, 0);
	glVertex3f(0, 5, 0.2);
	glVertex3f(-0.5, wingY, 0.2); // end of wing
	glEnd();
	glPopMatrix();

	// wings movement on Y axis
	wingY += 0.01 * wingDir;
	if (wingY >= 5.3 || wingY <= 4.7)
	{
		wingDir = wingDir * -1;
	}
	if (!rotateCrowFlag)
	{
		// crow speed
		moveCrow -= 0.05;
		if (moveCrow <= -20)
		{
			moveCrow = 20;
		}
		// rotate crow
		if (moveCrow <= -2.55 && moveCrow >= -2.7)
		{
			rotateCrowFlag = true;
		}
	}
	else
	{
		// rotation angle
		rotateCrow += 0.7;
		if (rotateCrow >= 360)
		{
			rotateCrowFlag = false;
			rotateCrow = 0;
		}
	}
}

void moveCamera()
{
	if (keystates['w'] && ! collisionDetected(camera.x, camera.z, rotateX, rotateZ, 0))
	{
		// move forward
		camera.x += rotateX * cameraSpeed;
		camera.z += rotateZ * cameraSpeed;
		if (keystates['a'] && !collisionDetected(camera.x, camera.z, rotateX, rotateZ, 2))
		{
			// slide left
			camera.x += sidewaysX * cameraSpeed;
			camera.z += sidewaysZ * cameraSpeed;
		}
		else
		{
			if (keystates['d'] && !collisionDetected(camera.x, camera.z, rotateX, rotateZ, 3))
			{
				// slide right
				camera.x += sidewaysX * -cameraSpeed;
				camera.z += sidewaysZ * -cameraSpeed;
			}
		}
	}
	else
	{
		if (keystates['s'] && !collisionDetected(camera.x, camera.z, rotateX, rotateZ, 1))
		{
			// move backward
			camera.x += rotateX * -cameraSpeed;
			camera.z += rotateZ * -cameraSpeed;
			if (keystates['a'] && !collisionDetected(camera.x, camera.z, rotateX, rotateZ, 2))
			{
				// slide left
				camera.x += sidewaysX * cameraSpeed;
				camera.z += sidewaysZ * cameraSpeed;
			}
			else
			{
				if (keystates['d'] && !collisionDetected(camera.x, camera.z, rotateX, rotateZ, 3))
				{
					// slide right
					camera.x += sidewaysX * -cameraSpeed;
					camera.z += sidewaysZ * -cameraSpeed;
				}
			}
		}
		else
		{
			if (keystates['a'] && !collisionDetected(camera.x, camera.z, rotateX, rotateZ, 2))
			{
				// move left
				camera.x += sidewaysX * cameraSpeed;
				camera.z += sidewaysZ * cameraSpeed;
			}
			else
			{
				if (keystates['d'] && !collisionDetected(camera.x, camera.z, rotateX, rotateZ, 3))
				{
					// move right
					camera.x += sidewaysX * -cameraSpeed;
					camera.z += sidewaysZ * -cameraSpeed;
				}
			}
		}
	}
}

void EnableFog()
{
	glClearColor(fogColor[0], fogColor[1], fogColor[2], 1.0);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_EXP);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_DENSITY, 0.1f); 
	glFogf(GL_FOG_START, 1.0f);             // Fog Start Depth
	glFogf(GL_FOG_END, 8.0f);
	glEnable(GL_FOG);
}
void EnablePShadows()
{
	//enable stencil testing
	glEnable(GL_STENCIL_TEST);
	//set the stencil function to keep all data except when depth test fails
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//set stencil function to allways pass
	//this way the stencil buffer will be 1 in the area where the floor is drawn
	glStencilFunc(GL_ALWAYS, 1, 0);
	//clear the stencil buffer
	glClear(GL_STENCIL_BUFFER_BIT);

	drawGround();

	//set the stencil function to pass only when the stencil buffer is 1
	//this way the shadow will appear only on the surface previously drawn
	glStencilFunc(GL_EQUAL, 1, 1);
	//disable depth testing for the shadow
	glDisable(GL_DEPTH_TEST);
	//disable lighting, in order to have a black shadow
	glDisable(GL_LIGHTING);

	//floor shadow	
	glColor3f(0.05f, 0.05f, 0.05f);//set the shadow color
	glPushMatrix();
	//project the teapot onto the floor
	glMultMatrixf(fvFloorShadowMat);
	//draw the projected teapot (the shadow)

	drawMap();

	glPopMatrix();
	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	//enable lighting
	glEnable(GL_LIGHTING);

	//set stencil function to allways pass
	//this way the stencil buffer will be 1 in the area where the left wall is drawn
	glStencilFunc(GL_ALWAYS, 1, 0);
	//clear the stencil buffer
	glClear(GL_STENCIL_BUFFER_BIT);

	drawMap();
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	moveCamera();
	gluLookAt(camera.x, camera.y, camera.z,
		camera.x + rotateX, camera.y + rotateY, camera.z + rotateZ,
		0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, sun_position);

	glLightfv(GL_LIGHT1, GL_POSITION, fire_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, fire_direction);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, fire_diffuse);

	if (toggleFog)
	{
		EnableFog();
	}
	if (togglePShadows)
	{
		EnablePShadows();
	}
	else
	{
		drawGround();

		drawMap();
	}
	drawSkybox(skyboxSize);

	drawCrow();

	glDisable(GL_FOG);

	if (toggleWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glutSwapBuffers();
}

void processKeysUp(unsigned char key, int x, int y)
{
	if (keystates[key] == true)
	{
		switch (key)
		{
		case 'f':
			toggleFog = !toggleFog;
			break;
		case 'p':
			togglePShadows = !togglePShadows;
			break;
		case 'v':
			toggleVShadows = !toggleVShadows;
			break;
		case 'q':
			toggleWireframe = !toggleWireframe;
			break;
		}
	}
	keystates[key] = false;
}

void processKeysDown(unsigned char key, int x, int y)
{
	keystates[key] = true;
	switch (key)
	{
	case 27:
		killSkybox();
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	case GLUT_KEY_LEFT:
		sun_position[2] -= 10;
		ComputeShadowMatrices();
		break;
	case GLUT_KEY_RIGHT:
		sun_position[2] += 10;
		ComputeShadowMatrices();
		break;
	}
	glutPostRedisplay();
}

void processMouseMovement(int x, int y)
{
	int difX = x - screen_width / 2,
		difY = y - screen_height / 2;

	// difX > 0 -> right
	// difY > 0 -> down


	if (difX > 0)
	{
		if (eyeAngleX > 359)
		{
			eyeAngleX = 0;
		}
		else
		{
			eyeAngleX += 1;
		}
	}
	else
	{
		if (difX < 0)
		{
			if (eyeAngleX < 1)
			{
				eyeAngleX = 360;
			}
			else
			{
				eyeAngleX -= 1;
			}
		}
	}

	if (difY > 0)
	{
		// looking up
		if (eyeAngleY > -90)
		{
			eyeAngleY -= 0.5;
		}
	}
	else
	{
		if (difY < 0)
		{
			// looking down
			if (eyeAngleY < 90)
			{
				eyeAngleY += 0.5;
			}
		}
	}

	rotateX = cos(eyeAngleX * PI / 180);
	rotateY = sin(eyeAngleY * PI / 180);
	rotateZ = sin(eyeAngleX * PI / 180);

	sidewaysX = cos((eyeAngleX - 90) * PI / 180);
	sidewaysZ = sin((eyeAngleX - 90) * PI / 180);

	if (difX != 0 || difY != 0)
	{
		glutWarpPointer(screen_width / 2, screen_height / 2);
	}
}

void changeSize(int w, int h)
{
	screen_width = w;
	screen_height = h;

	if (h == 0)
		h = 1;

	float ratio = 1.0*w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 50.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
}

int main(int argc, char* argv[])
{
	//Initialize the GLUT library
	glutInit(&argc, argv);
	//Set the display mode
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	//Set the initial position and dimensions of the window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(screen_width, screen_height);
	//creates the window
	glutCreateWindow("FPShooter");
	//Specifies the function to call when the window needs to be redisplayed
	glutDisplayFunc(renderScene);


	glutSetCursor(GLUT_CURSOR_NONE);
	//glutFullScreen();

	//Sets the idle callback function
	glutIdleFunc(renderScene);
	//Sets the reshape callback function
	glutReshapeFunc(changeSize);
	//Keyboard callback function
	glutKeyboardFunc(processKeysDown);
	glutKeyboardUpFunc(processKeysUp);

	glutSpecialFunc(processSpecialKeys);
	glutPassiveMotionFunc(processMouseMovement);

	//Initialize some OpenGL parameters
	initOpenGL();

	//Starts the GLUT infinite loop
	glutMainLoop();
	return 0;
}