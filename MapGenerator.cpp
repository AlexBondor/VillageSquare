#include "MapGenerator.h"

GLMmodel *houseModel, *chapelModel, *fountainModel, *fireplaceModel, *rocksModel;
GLuint houseTexture, chapelTexture, fountainTexture, fireplaceTexture, tree1Texture, rocksTexture, fire1Texture, fire2Texture, fire3Texture, fire4Texture, fireTexture;

unsigned int ground;
GLfloat base_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat low_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat full_ambient[]={ 1.0, 1.0, 1.0, 1.0 };

int objCount = 0,
housesCount = 0,
treesCount = 0;

typedef struct corner
{
	float x, z;
}Corner;

typedef struct box
{
	Corner a, b, c, d;
}CBox;

CBox objBoxes[100];

typedef struct p
{
	GLfloat posX, posZ, rotY;
}BuildingParams;

BuildingParams houses[10], chapel, fountain, fireplace, trees[50], rocks;

GLfloat fire_delay = 150; // ms

void setCollidingBox(int index, GLfloat onX, GLfloat onZ, int type)
{
	GLfloat posX, posZ;
	int rotY;

	switch (type)
	{
	case 0:
		posX = houses[index].posX;
		posZ = houses[index].posZ;
		rotY = houses[index].rotY;
		break;
	case 1:
		posX = chapel.posX;
		posZ = chapel.posZ - 1.5;
		rotY = chapel.rotY;
		break;
	case 2:
		posX = fountain.posX;
		posZ = fountain.posZ;
		rotY = fountain.rotY;
		break;
	case 3:
		posX = fireplace.posX;
		posZ = fireplace.posZ;
		rotY = fireplace.rotY;
		break;
	case 4:
		posX = trees[index].posX;
		posZ = trees[index].posZ;
		rotY = trees[index].rotY;
		break;
	case 5:
		posX = rocks.posX;
		posZ = rocks.posZ;
		rotY = rocks.rotY;
		break;
	}

	float s, c;

	switch (rotY)
	{
	case 0:
		s = sin(0.0);
		c = cos(0.0);
		objBoxes[objCount].a.x = posX - onX * c - onZ * s;
		objBoxes[objCount].a.z = posZ - onX * s + onZ * c;

		objBoxes[objCount].b.x = posX + onX * c - onZ * s;
		objBoxes[objCount].b.z = posZ + onX * s + onZ * c;

		objBoxes[objCount].c.x = posX + onX * c + onZ * s;
		objBoxes[objCount].c.z = posZ + onX * s - onZ * c;

		objBoxes[objCount].d.x = posX - onX * c + onZ * s;
		objBoxes[objCount].d.z = posZ - onX * s - onZ * c;
		break;
	case 1:
		s = sin(PI / 2);
		c = cos(PI / 2);
		objBoxes[objCount].d.x = posX - onX * c - onZ * s;
		objBoxes[objCount].d.z = posZ - onX * s + onZ * c;

		objBoxes[objCount].a.x = posX + onX * c - onZ * s;
		objBoxes[objCount].a.z = posZ + onX * s + onZ * c;

		objBoxes[objCount].b.x = posX + onX * c + onZ * s;
		objBoxes[objCount].b.z = posZ + onX * s - onZ * c;

		objBoxes[objCount].c.x = posX - onX * c + onZ * s;
		objBoxes[objCount].c.z = posZ - onX * s - onZ * c;
		break;
	case 2:
		s = sin(PI);
		c = cos(PI);
		objBoxes[objCount].c.x = posX - onX * c - onZ * s;
		objBoxes[objCount].c.z = posZ - onX * s + onZ * c;

		objBoxes[objCount].d.x = posX + onX * c - onZ * s;
		objBoxes[objCount].d.z = posZ + onX * s + onZ * c;

		objBoxes[objCount].a.x = posX + onX * c + onZ * s;
		objBoxes[objCount].a.z = posZ + onX * s - onZ * c;

		objBoxes[objCount].b.x = posX - onX * c + onZ * s;
		objBoxes[objCount].b.z = posZ - onX * s - onZ * c;
		break;
	case 3:
		s = sin(3 * PI / 2);
		c = cos(3 * PI / 2);
		objBoxes[objCount].b.x = posX - onX * c - onZ * s;
		objBoxes[objCount].b.z = posZ - onX * s + onZ * c;

		objBoxes[objCount].c.x = posX + onX * c - onZ * s;
		objBoxes[objCount].c.z = posZ + onX * s + onZ * c;

		objBoxes[objCount].d.x = posX + onX * c + onZ * s;
		objBoxes[objCount].d.z = posZ + onX * s - onZ * c;

		objBoxes[objCount].a.x = posX - onX * c + onZ * s;
		objBoxes[objCount].a.z = posZ - onX * s - onZ * c;
		break;
	}
}

void initBuildings()
{
	// house 1
	houses[objCount].posX = -5;
	houses[objCount].posZ = -10;
	houses[objCount].rotY = 1;
	setCollidingBox(housesCount, 4.5, 3, 0);
	housesCount++;
	objCount++;

	// house 2
	houses[objCount].posX = 5;
	houses[objCount].posZ = -10;
	houses[objCount].rotY = 0;
	setCollidingBox(housesCount, 4.5, 3, 0);
	housesCount++;
	objCount++;

	// house 3
	houses[objCount].posX = -10;
	houses[objCount].posZ = 10;
	houses[objCount].rotY = 2;
	setCollidingBox(housesCount, 4.5, 3, 0);
	housesCount++;
	objCount++;

	// house 4
	houses[objCount].posX = 10;
	houses[objCount].posZ = 10;
	houses[objCount].rotY = 3;
	setCollidingBox(housesCount, 4.5, 3, 0);
	housesCount++;
	objCount++;

	// chapel
	chapel.posX = 15;
	chapel.posZ = -1;
	chapel.rotY = 0;
	setCollidingBox(-1, 4.5, 3, 1);
	objCount++;

	// fountain
	fountain.posX = 0;
	fountain.posZ = 0;
	fountain.rotY = 0;
	setCollidingBox(-1, 1, 1, 2);
	objCount++;

	// fireplace
	fireplace.posX = 0;
	fireplace.posZ = 7;
	fireplace.rotY = 0;
	setCollidingBox(-1, 1, 1, 3);
	objCount++;
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	// TREES ON LEFT
	////////////////////////////////////////////////////////////////////////////////////////////
	//tree1
	trees[treesCount].posX = -10;
	trees[treesCount].posZ = -10;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree2
	trees[treesCount].posX = -7;
	trees[treesCount].posZ = -15;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree3
	trees[treesCount].posX = -2;
	trees[treesCount].posZ = -18;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree4
	trees[treesCount].posX = 3;
	trees[treesCount].posZ = -15;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree5
	trees[treesCount].posX = 1;
	trees[treesCount].posZ = -20;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree6
	trees[treesCount].posX = 5;
	trees[treesCount].posZ = -18;
	setCollidingBox(treesCount, 0.2, 0.2, 4);
	treesCount++;
	objCount++;

	//tree7
	trees[treesCount].posX = 8;
	trees[treesCount].posZ = -17;
	setCollidingBox(treesCount, 0.2, 0.2, 4);
	treesCount++;
	objCount++;


	/////////////////////////////////////////////////////////////////////////////////////////////
	// TREES ON BACK
	////////////////////////////////////////////////////////////////////////////////////////////
	//tree1
	trees[treesCount].posX = 14;
	trees[treesCount].posZ = -13;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree2
	trees[treesCount].posX = 18;
	trees[treesCount].posZ = -11;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree3
	trees[treesCount].posX = 15;
	trees[treesCount].posZ = -7;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree4
	trees[treesCount].posX = 20;
	trees[treesCount].posZ = -5;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree5
	trees[treesCount].posX = 20;
	trees[treesCount].posZ = 0;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree6
	trees[treesCount].posX = 16;
	trees[treesCount].posZ = 5;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree7
	trees[treesCount].posX = 16;
	trees[treesCount].posZ = 10;
	setCollidingBox(treesCount, 0.2, 0.2, 4);
	treesCount++;
	objCount++;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// TREES ON RIGHT
	////////////////////////////////////////////////////////////////////////////////////////////
	//tree1
	trees[treesCount].posX = 14;
	trees[treesCount].posZ = 17;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree2
	trees[treesCount].posX = 12;
	trees[treesCount].posZ = 15;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree3
	trees[treesCount].posX = 10;
	trees[treesCount].posZ = 19;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree4
	trees[treesCount].posX = 7;
	trees[treesCount].posZ = 15;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree5
	trees[treesCount].posX = 3;
	trees[treesCount].posZ = 14;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree6
	trees[treesCount].posX = 0;
	trees[treesCount].posZ = 16;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree7
	trees[treesCount].posX = -2;
	trees[treesCount].posZ = 15;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree8
	trees[treesCount].posX = -7;
	trees[treesCount].posZ = 13;
	setCollidingBox(treesCount, 0.2, 0.2, 4);
	treesCount++;
	objCount++;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// TREES ON BACK
	////////////////////////////////////////////////////////////////////////////////////////////
	//tree1
	trees[treesCount].posX = -15;
	trees[treesCount].posZ = 12;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree2
	trees[treesCount].posX = -17;
	trees[treesCount].posZ = 7;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree3
	trees[treesCount].posX = -22;
	trees[treesCount].posZ = 2;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree4
	trees[treesCount].posX = -20;
	trees[treesCount].posZ = -2;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree5
	trees[treesCount].posX = -19;
	trees[treesCount].posZ = -7;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree6
	trees[treesCount].posX = -16;
	trees[treesCount].posZ = -10;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree7
	trees[treesCount].posX = -14;
	trees[treesCount].posZ = -15;
	setCollidingBox(treesCount, 0.5, 0.5, 4);
	treesCount++;
	objCount++;

	//tree8
	trees[treesCount].posX = -7;
	trees[treesCount].posZ = 13;
	setCollidingBox(treesCount, 0.2, 0.2, 4);
	treesCount++;
	objCount++;

	rocks.posX = -20;
	rocks.posZ = -1;
	setCollidingBox(-1, 6.0, 6.0, 5);
	treesCount++;
	objCount++;
}

void initMap()
{
	initBuildings();

	glGenTextures(1, &ground);
	// load ground texture
	ground = loadTexture("data/ground.bmp");

	// HOUSE
	initModel("data/Objects/House/house_obj.obj", &houseModel);	//Create the texture object
	glGenTextures(1, &houseTexture);
	//Assign the texture file to the texture object
	loadTGA("data/Objects/House/house_diffuse.tga", houseTexture);

	// CHAPPEL
	initModel("data/Objects/Chapel/chapel_obj.obj", &chapelModel);	//Create the texture object
	glGenTextures(1, &chapelTexture);
	//Assign the texture file to the texture object
	loadTGA("data/Objects/Chapel/chapel_diffuse.tga", chapelTexture);

	// FOUNTAIN
	initModel("data/Objects/Fountain/fountain.obj", &fountainModel);	//Create the texture object
	glGenTextures(1, &fountainTexture);
	//Assign the texture file to the texture object
	loadTGA("data/Objects/Fountain/fountainDe.tga", fountainTexture);

	// FIREPLACE
	initModel("data/Objects/Fireplace/fireplace.obj", &fireplaceModel);	//Create the texture object
	glGenTextures(1, &fireplaceTexture);
	//Assign the texture file to the texture object
	loadTGA("data/Objects/Fireplace/fireplace.tga", fireplaceTexture);

	// TREE 1
	glGenTextures(1, &tree1Texture);
	//Assign the texture file to the texture object
	loadTGA("data/Objects/Trees/tree1.tga", tree1Texture);

	// FIRE 1
	glGenTextures(1, &fire1Texture);
	//Assign the texture file to the texture object
	fire1Texture = loadTexture("data/Objects/Fire/fire1.png");

	fireTexture = fire1Texture;
	pulse(1);

	// FIRE 2
	glGenTextures(1, &fire2Texture);
	//Assign the texture file to the texture object
	fire2Texture = loadTexture("data/Objects/Fire/fire2.png");

	// FIRE 3
	glGenTextures(1, &fire3Texture);
	//Assign the texture file to the texture object
	fire3Texture = loadTexture("data/Objects/Fire/fire3.png");

	// FIRE 4
	glGenTextures(1, &fire4Texture);
	//Assign the texture file to the texture object
	fire4Texture = loadTexture("data/Objects/Fire/fire4.png");

	// ROCKS
	initModel("data/Objects/Rocks/rocks_01_model.obj", &rocksModel);	//Create the texture object
	glGenTextures(1, &rocksTexture);
	//Assign the texture file to the texture object
	loadTGA("data/Objects/Rocks/rocks_01_dif.tga", rocksTexture);
}
void initModel(char* filename, GLMmodel **pModel)
{
	if (!*pModel)
	{
		*pModel = glmReadOBJ(filename);
		if (!pModel)

			exit(0);
		//Fit the original model inside a unit cube
		glmUnitize(*pModel);
		//Generate facet normal vectors for model
		glmFacetNormals(*pModel);
		//Generate vertex normal vectors (called only after generating facet normals)
		glmVertexNormals(*pModel, 90.0);
	}
}

void drawGround()
{

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, base_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, full_ambient);

	glEnable(GL_TEXTURE_2D);
	// left face
	glBindTexture(GL_TEXTURE_2D, ground);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-75, 0, 75);
	glTexCoord2f(1, 0); glVertex3f(75, 0, 75);
	glTexCoord2f(1, 1); glVertex3f(75, 0, -75);
	glTexCoord2f(0, 1); glVertex3f(-75, 0, -75);
	glEnd();
}

void drawSlice(GLfloat size, GLuint texture)
{
	glEnable(GL_TEXTURE_2D); //enable 2D texturing		
	glBindTexture(GL_TEXTURE_2D, texture); //select the current texture

	glEnable(GL_ALPHA_TEST); //enable alpha testing
	glAlphaFunc(GL_GREATER, 0.1f); //select the alpha testing function
	//GL_GREATER function passes if the incoming alpha value is greater than the reference value

	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(size, size, 0.0); //assign each corner of the texture to a 3D vertex in the OpenGL scene
	glTexCoord2f(0, 1);
	glVertex3f(-size, size, 0.0); //(0,0) is the left lower corner, while (1,1) is the right upper corner of the texture
	glTexCoord2f(0, 0);
	glVertex3f(-size, -size, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(size, -size, 0.0);
	glEnd();

	glDisable(GL_ALPHA_TEST); //disable alpha testing
	glDisable(GL_TEXTURE_2D); //disable 2D texturing
}

void drawTree(GLfloat posX, GLfloat posZ)
{
	glPushMatrix();
	glTranslatef(posX, 4, posZ);
	glScalef(0.8, 0.8, 0.8);
	drawSlice(5, tree1Texture);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	drawSlice(5, tree1Texture);
	glPopMatrix();
	glPopMatrix();
}

void pulse(int value)
{
	switch (value)
	{
	case 1:
		fireTexture = fire1Texture;
		break;
	case 2:
		fireTexture = fire2Texture;
		break;
	case 3:
		fireTexture = fire3Texture;
		break;
	case 4:
		fireTexture = fire4Texture;
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
	glutTimerFunc(fire_delay, pulse, value);
}

void drawFire(GLfloat posX, GLfloat posZ, GLuint texture)
{
	glPushMatrix();
	glTranslatef(posX, 0.3, posZ);
	drawSlice(0.2, texture);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	drawSlice(0.2, texture);
	glPopMatrix();
	glPopMatrix();
}

void draw(BuildingParams params, GLMmodel *model, GLuint texture, GLfloat scale, GLfloat upper)
{
	GLfloat posX = params.posX,
		posZ = params.posZ,
		rotY = params.rotY * 90;

	glPushMatrix();
	glTranslatef(posX, 0, posZ);
	glRotatef(rotY, 0, 1, 0);

	glPushMatrix();
	glScalef(scale, scale, scale);
	glTranslatef(0, upper, 0);
	//Enable 2D texturing
	glEnable(GL_TEXTURE_2D);
	//Select current texture
	glBindTexture(GL_TEXTURE_2D, texture);
	//Draw the Obj file along with the texture
	glmDraw(model, GLM_SMOOTH | GLM_TEXTURE);
	//Disable 2D Texturing
	glDisable(GL_TEXTURE_2D);	glPopMatrix();	glPopMatrix();
}

void drawMap()
{
	for (int i = 0; i < treesCount - 1; i++)
	{
		if (trees[i].posX == 0 && trees[i].posZ == 0)
		{
			printf("%d ouch\n", i);
		}
		drawTree(trees[i].posX, trees[i].posZ);
	}
	drawFire(0, 7, fireTexture);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, low_ambient);
	for (int i = 0; i < housesCount; i++)
	{
		draw(houses[i], houseModel, houseTexture, 4, 0.5);
	}
	draw(chapel, chapelModel, chapelTexture, 4.5, 1);
	draw(fountain, fountainModel, fountainTexture, 1, 0.5);
	draw(rocks, rocksModel, rocksTexture, 6, 0.5);
	draw(fireplace, fireplaceModel, fireplaceTexture, 0.7, 0.2);
}

// 0 fwd
// 1 bwd
// 2 left
// 3 right
bool collisionDetected(GLfloat eyeX, GLfloat eyeZ, GLfloat rotateX, GLfloat rotateZ, int dir)
{
	Corner a, c, camW, camS, camA, camD;
	float sinus, cosinus;

	// set to first quadrand (easy comp)
	eyeX += 1000;
	eyeZ -= 1000;

	camW.x = eyeX + rotateX;
	camW.z = eyeZ + rotateZ;

	// cam right
	sinus = sin(PI / 2);
	cosinus = cos(PI / 2);
	camD.x = eyeX + rotateX * cosinus - rotateZ * sinus;
	camD.z = eyeZ + rotateX * sinus + rotateZ * cosinus;

	// cam back
	sinus = sin(PI);
	cosinus = cos(PI);
	camS.x = eyeX + rotateX * cosinus - rotateZ * sinus;
	camS.z = eyeZ + rotateX * sinus + rotateZ * cosinus;

	// cam left
	sinus = sin(3 * PI / 2);
	cosinus = cos(3 * PI / 2);
	camA.x = eyeX + rotateX * cosinus - rotateZ * sinus;
	camA.z = eyeZ + rotateX * sinus + rotateZ * cosinus;


	for (int i = 0; i < objCount; i++)
	{
		// transpose cam and objBox coordinates to first quadrant
		a = objBoxes[i].a;
		c = objBoxes[i].c;

		a.x += 1000;
		a.z -= 1000;
		c.x += 1000;
		c.z -= 1000;

		// working quadrant 1
		if (dir == 0)
		{
			// check forward
			if (camW.x >= a.x && camW.x <= c.x && camW.z <= a.z && camW.z >= c.z)
			{
				return true;
			}
		}
		else
		{
			if (dir == 1)
			{
				// check backward
				if (camS.x >= a.x && camS.x <= c.x && camS.z <= a.z && camS.z >= c.z)
				{
					return true;
				}
			}
			else
			{
				if (dir == 2)
				{
					// check left
					if (camA.x >= a.x && camA.x <= c.x && camA.z <= a.z && camA.z >= c.z)
					{
						return true;
					}
				}
				else
				{
					if (dir == 3)
					{
						// check right
						if (camD.x >= a.x && camD.x <= c.x && camD.z <= a.z && camD.z >= c.z)
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}