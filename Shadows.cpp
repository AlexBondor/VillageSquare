#include "Shadows.h"

//////////////////////////////////////////////////////////////////////////////////////////
//																						//
//	Name:  void PlaneEq(GLfloat plane[4], GLfloat p0[4], GLfloat p1[4], GLfloat p2[4])	//
//																						//
//	Description: Finds the A, B, C and D coefficients for the equation AX+BY+CZ+D=0		//
//				 of a plane determined by three points p0, p1 and p2					//
//																						//
//	Inputs:	GLfloat p0[4], p1[4] and p2[4] -> three arrays containing the X, Y, Z		//
//											  and W coordinates of points that belong	//
//											  to the plane								//
//																						//
//	Outputs: GLfloat plane[4] -> array containing the A, B, C and D coefficients for	//
//								 the equation AX+BY+CZ+D=0 of the plane					//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////
void PlaneEq(GLfloat plane[4], GLfloat p0[4], GLfloat p1[4], GLfloat p2[4])
{
	GLfloat vec0[3], vec1[3];

	vec0[0] = p1[0] - p0[0];
	vec0[1] = p1[1] - p0[1];
	vec0[2] = p1[2] - p0[2];

	vec1[0] = p2[0] - p0[0];
	vec1[1] = p2[1] - p0[1];
	vec1[2] = p2[2] - p0[2];

	plane[0] = vec0[1] * vec1[2] - vec0[2] * vec1[1];
	plane[1] = -(vec0[0] * vec1[2] - vec0[2] * vec1[0]);
	plane[2] = vec0[0] * vec1[1] - vec0[1] * vec1[0];

	plane[3] = -(plane[0] * p0[0] + plane[1] * p0[1] + plane[2] * p0[2]);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Name: void ComputeShadowMatrix(GLfloat shadowMat[16], GLfloat plane[4], GLfloat lightPos[4])	//
//																									//
//	Description: Computes the matrix used to project the shadow on a plane							//
//																									//
//	Input: GLfloat plane[4] -> array containing the A, B, C and D coefficients for the equation		//
//							   of the plane on which the shadow will be cast						//
//		   GLfloat lightPos[4] -> array containing the X, Y, Z and W coordinates of the light		//
//								  source															//
//																									//
//	Outputs: Glfloat shadowMat[16] -> array containing the column-major matrix used to project		//
//									  the shadow on the given plane									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////
void ComputeShadowMatrix(GLfloat shadowMat[16], GLfloat plane[4], GLfloat lightPos[4])
{
	GLfloat dotProduct;

	dotProduct = plane[0] * lightPos[0] +
		plane[1] * lightPos[1] +
		plane[2] * lightPos[2] +
		plane[3] * lightPos[3];

	shadowMat[0] = dotProduct - lightPos[0] * plane[0];
	shadowMat[1] = 0.0f - lightPos[1] * plane[0];
	shadowMat[2] = 0.0f - lightPos[2] * plane[0];
	shadowMat[3] = 0.0f - lightPos[3] * plane[0];

	shadowMat[4] = 0.0f - lightPos[0] * plane[1];
	shadowMat[5] = dotProduct - lightPos[1] * plane[1];
	shadowMat[6] = 0.0f - lightPos[2] * plane[1];
	shadowMat[7] = 0.0f - lightPos[3] * plane[1];

	shadowMat[8] = 0.0f - lightPos[0] * plane[2];
	shadowMat[9] = 0.0f - lightPos[1] * plane[2];
	shadowMat[10] = dotProduct - lightPos[2] * plane[2];
	shadowMat[11] = 0.0f - lightPos[3] * plane[2];

	shadowMat[12] = 0.0f - lightPos[0] * plane[3];
	shadowMat[13] = 0.0f - lightPos[1] * plane[3];
	shadowMat[14] = 0.0f - lightPos[2] * plane[3];
	shadowMat[15] = dotProduct - lightPos[3] * plane[3];
}