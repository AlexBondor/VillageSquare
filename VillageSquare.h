#ifndef _MAIN_H
#define _MAIN_H

// Standard libraries
#include "stdafx.h"
#include "glut.h"
#include <gl/gl.h>
#include "math.h"
#include "glm.h"
#include "tga.h"

// Used for texture loading
#include "SOIL.h"   

// Skybox code
#include "Skybox.h" 

// Map generator
#include "MapGenerator.h"

// Texture loader
#include "TextureLoader.h"

// Shadow computations
#include "Shadows.h"

#define PI 3.14159265359

#define SKY_RIGHT  0
#define SKY_LEFT   1
#define SKY_FRONT  2
#define SKY_BACK   3
#define SKY_TOP    4
#define SKY_BOTTOM 5

#endif