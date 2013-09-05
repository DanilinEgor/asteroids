#include "asteroid.h"
#include "util.h"
#include <android/log.h>

#define APPNAME "com.danegor.asteroidsgame"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,APPNAME,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,APPNAME,__VA_ARGS__)

float randFloat(float low, float high) {
	return low + (float) rand() / ((float) RAND_MAX / (high - low));
}

Asteroid::~Asteroid() {
	delete[] vertices;
}

Asteroid::Asteroid(int w, int h, bool bigIn, float xIn, float yIn, unsigned int program, unsigned int pos) {
	MIN_RADIUS = (float) h / w * 3.0f / 5.0f;
	MAX_RADIUS = (float) h / w * 4.0f / 5.0f;

	big = bigIn;

	if (!bigIn) {
		MIN_RADIUS /= 3;
		MAX_RADIUS /= 3;
	}
	gProgram = program;
	gvPositionHandle = pos;

	if (bigIn) {
		int k = rand() % 8;

		/*
		 * Расположение астероида
		 * 3|2|1
		 * 4|X|0
		 * 5|6|7
		 */

		switch (k) {
			case 0:
				x = 1.1f + MAX_RADIUS;
				y = randFloat(-1.0f, 1.0f);
				angle = randFloat(-M_PI - atan2(2.1f, 1 - y), -M_PI + atan2(2.1f, y + 1));
				break;
			case 1:
				x = randFloat(1.0f, 2.0f) + MAX_RADIUS;
				y = randFloat(1.0f, 2.0f) + MAX_RADIUS;
				angle = randFloat(-M_PI / 2.0f - atan2(y - 1, x), -M_PI / 2.0f - atan2(y, x - 1));
				break;
			case 2:
				x = randFloat(-1.0f, 1.0f);
				y = 1.1f + MAX_RADIUS;
				angle = randFloat(-M_PI / 2.0f - atan2(2.1f, x + 1), -M_PI / 2.0f + atan2(2.1f, 1 - x));
				break;
			case 3:
				x = randFloat(-2.0f, -1.0f) - MAX_RADIUS;
				y = randFloat(1.0f, 2.0f) + MAX_RADIUS;
				angle = randFloat(-M_PI / 2.0f + atan2(y - 1, x), -M_PI / 2.0f + atan2(y, x + 1));
				break;
			case 4:
				x = -1.1f - MAX_RADIUS;
				y = randFloat(-1.0f, 1.0f);
				angle = randFloat(-atan2(2.1f, y + 1), atan2(2.1f, 1 - y));
				break;
			case 5:
				x = randFloat(-2.0f, -1.0f) - MAX_RADIUS;
				y = randFloat(-2.0f, -1.0f) - MAX_RADIUS;
				angle = randFloat(M_PI / 2.0f - atan2(y + 1, x), M_PI / 2.0f - atan2(y, x + 1));
				break;
			case 6:
				x = randFloat(-1.0f, 1.0f);
				y = -1.1f - MAX_RADIUS;
				angle = randFloat(M_PI / 2.0f + atan2(2.1f, 1 - x), M_PI / 2.0f - atan2(2.1f, x + 1));
				break;
			case 7:
				x = randFloat(1.0f, 2.0f) + MAX_RADIUS;
				y = randFloat(-2.0f, -1.0f) - MAX_RADIUS;
				angle = randFloat(M_PI / 2.0f + atan2(y, x - 1), M_PI / 2.0f + atan2(y + 1, x));
				break;
			default:
				break;
		}
	}
	else {
		x = xIn;
		y = yIn;
		angle = randFloat(0, M_PI * 2.0f);
	}

	numOfVertices = rand() % 8 + 5;

	vertices = new float[2 * (numOfVertices + 1)];
	int idx = 0;
	vertices[idx++] = x;
	vertices[idx++] = y;
	float a0 = randFloat(0, M_PI * 2.0f);
	for (int i = 0;  i < numOfVertices; ++i) {
		float r = randFloat(MIN_RADIUS, MAX_RADIUS);
		float a = randFloat(M_PI / 180.0f, M_PI * 2.0f / numOfVertices);
		vertices[idx++] = x + r * cos(a0) * (float) h / w;
		vertices[idx++] = y + r * sin(a0);
		a0 += a;
	}
	speed = randFloat((float) h / w / 100, (float) h / w / 50);
#if 0
	LOGI("ASTEROID k = %d, x = %f, y = %f, angle = %f", k, x, y, angle);
	LOGI("ASTEROID num = %d", numOfVertices);
#endif
}

void Asteroid::init(unsigned int &program, unsigned int &handle) {
	const char gVertexShader[] =
				"attribute vec4 vPosition;\n"
			    "void main() {\n"
			    "  gl_Position = vPosition;\n"
			    "}\n";
	const char gFragmentShader[] =
			    "precision mediump float;\n"
			    "void main() {\n"
			    "  gl_FragColor = vec4(0.8, 0.8, 0.8, 1.0);\n"
			    "}\n";
	if (!setupGraphics(gVertexShader, gFragmentShader, gProgram, gvPositionHandle))
		LOGE("Asteroid init error");
	program = gProgram;
	handle = gvPositionHandle;
}

void Asteroid::render() {
	glUseProgram(gProgram);

#if 0
	for (int i = 0; i < 2*numOfVertices+2; ++i) {
		LOGI("ASTEROID vertices[%d]=%f", i, vertices[i]);
	}
#endif


	for (int i = 0; i < 2*numOfVertices+2; i+=2) {
		vertices[i] += speed * cos(angle);
		vertices[i+1] += speed * sin(angle);
	}


	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(gvPositionHandle);
	glDrawArrays(GL_TRIANGLE_FAN, 0, (numOfVertices+1));
}

float Asteroid::getX() {
	return vertices[0];
}

float Asteroid::getY() {
	return vertices[1];
}

float* Asteroid::getVertices() {
	return vertices;
}

int Asteroid::getNumOfVertices() {
	return numOfVertices;
}

bool Asteroid::getBig() {
	return big;
}
