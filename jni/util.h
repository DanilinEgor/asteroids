#ifndef UTIL_H_
#define UTIL_H_

#include <GLES2/gl2.h>
#include "asteroid.h"
#include "ship.h"

bool setupGraphics(const char*, const char*, GLuint&, GLuint&);
GLuint loadShader(GLenum, const char*);
GLuint createProgram(const char*, const char*);
bool checkIntersectionBulletAndAsteroid(Bullet*, Asteroid*);
inline bool checkIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
bool checkIntersectionShipAndAsteroid(Ship*, Asteroid*);

#endif
