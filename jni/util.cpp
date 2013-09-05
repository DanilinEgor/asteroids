#include "util.h"
#include <android/log.h>

#define APPNAME "com.danegor.asteroidsgame"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,APPNAME,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,APPNAME,__VA_ARGS__)


static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}



GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

bool setupGraphics(const char gVertexShader[], const char gFragmentShader[], GLuint& gProgram, GLuint& gvPositionHandle) {
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
#if 0
    LOGI("UTIL glGetAttribLocation(\"vPosition\") = %d\n", gvPositionHandle);
#endif

    return true;
}

inline bool checkIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
	return	((x1-x3)*(y4-y3)-(y1-y3)*(x4-x3)) * ((x2-x3)*(y4-y3)-(y2-y3)*(x4-x3)) < 0
			&& ((x3-x1)*(y2-y1)-(y3-y1)*(x2-x1)) * ((x4-x1)*(y2-y1)-(y4-y1)*(x2-x1)) < 0;
}

inline float norm(float x1, float y1, float x2, float y2) {
	return sqrt(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

bool checkIntersectionBulletAndAsteroid(Bullet* b, Asteroid* a) {
	float asteroidX = a->getX();
	float asteroidY = a->getY();
	float bulletX = (b->x1 + b->x2) / 2.0f;
	float bulletY = (b->y1 + b->y2) / 2.0f;
	if (norm(bulletX, bulletY, asteroidX, asteroidY) > a->MAX_RADIUS) return false;
	float* vertices = a->getVertices();
	int numOfVertices = a->getNumOfVertices();
	for (int i = 2; i < numOfVertices * 2 - 2; i+=2)
		if (checkIntersection(b->x1, b->y1, b->x2, b->y2, vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]))
			return true;
	if (checkIntersection(b->x1, b->y1, b->x2, b->y2, vertices[2*numOfVertices], vertices[2*numOfVertices+1], vertices[2], vertices[3]))
		return true;
	return false;
}

bool checkIntersectionShipAndAsteroid(Ship* s, Asteroid* a) {
	LOGI("GAME 03");
	float asteroidX = a->getX();
	float asteroidY = a->getY();
	float shipX = s->getX();
	float shipY = s->getY();
	if (norm(shipX, shipY, asteroidX, asteroidY) > a->MAX_RADIUS) return false;
	float* shipVertices = new float[6];
	s->getVertices(shipVertices);
	float* asteroidVertices = a->getVertices();
	int numOfVertices = a->getNumOfVertices();
	LOGI("GAME 04");
#if 0
	for (int i = 0; i < 6; ++i)
		LOGI("UTIL ship[%d]=%f", i, shipVertices[i]);
	for (int i = 0; i < numOfVertices; ++i)
		LOGI("UTIL asteroid[%d]=%f", i, asteroidVertices[i]);
#endif
	for (int j = 0; j < 6; j+=2) {
		for (int i = 2; i < numOfVertices * 2 - 2; i+=2)
			if (checkIntersection(shipVertices[j], shipVertices[j+1], shipVertices[(j+2)%6], shipVertices[(j+3)%6], asteroidVertices[i], asteroidVertices[i+1], asteroidVertices[i+2], asteroidVertices[i+3])) {
				LOGI("UTIL shipVertices[j]=%f, shipVertices[j+1]=%f, shipVertices[(j+2)%%6]=%f, shipVertices[(j+3)%%6]=%f, asteroidVertices[i]=%f, asteroidVertices[i+1]=%f, asteroidVertices[i+2]=%f, asteroidVertices[i+3]=%f", shipVertices[j], shipVertices[j+1], shipVertices[(j+2)%6], shipVertices[(j+3)%6], asteroidVertices[i], asteroidVertices[i+1], asteroidVertices[i+2], asteroidVertices[i+3]);
				delete[] shipVertices;
				return true;
			}
		if (checkIntersection(shipVertices[j], shipVertices[j+1], shipVertices[(j+2)%6], shipVertices[(j+3)%6], asteroidVertices[2*numOfVertices], asteroidVertices[2*numOfVertices+1], asteroidVertices[2], asteroidVertices[3])) {
			LOGI("UTIL shipVertices[j]=%f, shipVertices[j+1]=%f, shipVertices[(j+2)%%6]=%f, shipVertices[(j+3)%%6]=%f, asteroidVertices[2*numOfVertices]=%f, asteroidVertices[2*numOfVertices+1]=%f, asteroidVertices[2]=%f, asteroidVertices[3]=%f", shipVertices[j], shipVertices[j+1], shipVertices[(j+2)%6], shipVertices[(j+3)%6], asteroidVertices[2*numOfVertices], asteroidVertices[2*numOfVertices+1], asteroidVertices[2], asteroidVertices[3]);
			delete[] shipVertices;
			return true;
		}
	}
	LOGI("GAME 05");
	delete[] shipVertices;
	return false;
}
