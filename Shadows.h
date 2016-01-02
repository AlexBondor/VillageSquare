#ifndef _SHADOWS_H
#define _SHADOWS_H

#include "VillageSquare.h"

void PlaneEq(GLfloat plane[4], GLfloat p0[4], GLfloat p1[4], GLfloat p2[4]);
void ComputeShadowMatrix(GLfloat shadowMat[16], GLfloat plane[4], GLfloat lightPos[4]);

#endif