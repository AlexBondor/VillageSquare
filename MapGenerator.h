#ifndef _MAPGENERATOR_H
#define _MAPGENERATOR_H

#include "VillageSquare.h"

void setCollidingBox(int index, GLfloat onX, GLfloat onZ, int type);
void initBuildings();
void initMap();
void initModel(char* filename, GLMmodel **pModel);
void drawGround();
void drawHouses();
void drawHouse(int index);
void drawMap();
bool collisionDetected(GLfloat camX, GLfloat camZ, GLfloat rotateX, GLfloat rotateZ, int dir);
void pulse(int value);

#endif