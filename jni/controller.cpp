#include "controller.h"
#include "util.h"
#include <android/log.h>
#include <GLES2/gl2.h>
#include <algorithm>
#include <math.h>

#define APPNAME "com.danegor.asteroidsgame"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,APPNAME,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,APPNAME,__VA_ARGS__)

#define _USE_MATH_DEFINES

GLfloat* createCircle(int width, int height, float radius, float center_x, float center_y){
	int vertexCount = 30;

	float* buffer = new float[vertexCount*2];
	int idx = 0;

	float ratio = (float) height / width;

	buffer[idx++] = center_x;
	buffer[idx++] = center_y;

	int outerVertexCount = vertexCount-1;

	for (int i = 0; i < outerVertexCount; ++i){
	    float percent = (i / (float) (outerVertexCount-1));
		float rad = percent * 2 * M_PI;

	    float outer_x = center_x + radius * cos(rad) * ratio;
	    float outer_y = center_y + radius * sin(rad);

	    buffer[idx++] = outer_x;
	    buffer[idx++] = outer_y;
	}
	return buffer;
}


bool Controller::getAccelerationAndDirectionAngle(int width, int height, float* xs, float* ys, int touchCount, float& acceleration, float& angle) {
	acceleration = 0;
	angle = 0;
	float xmax = center_x + radius;
	float ymax = center_y + radius;
	for (int i = 0; i < touchCount; ++i){
		float x = xs[i] * 2 / width - 1;
		float y = 1 - ys[i] * 2 / height;
		if (x < xmax && y < ymax) {
			acceleration = sqrt(powf(x-center_x, 2) + powf(y-center_y, 2));
			angle = atan2(y-center_y, x-center_x);
			return true;
		}
	}
	return false;
}

float Controller::getRotationAngle(int width, int height, float* xs, float* ys, int touchCount) {
	float angle = 0;
	float xmin = -center_x - radius;
	float ymax = center_y + radius;
	for (int i = 0; i < touchCount; ++i){
			float x = xs[i] * 2 / width - 1;
			float y = 1 - ys[i] * 2 / height;
			if (x > xmin && y < ymax)
				angle = atan2(y-center_y, x+center_x);
		}
	return angle;
}

Controller::Controller(int width, int height) {
	center_x = (float) height / width * (1 - 0.65f) - 1;
	center_y = -0.65f;
	radius = 0.3f;
}

void Controller::init() {
	const char gVertexShader[] =
					"attribute vec4 vPosition;\n"
				    "void main() {\n"
				    "  gl_Position = vPosition;\n"
				    "}\n";
	const char gFragmentShader[] =
				    "precision mediump float;\n"
				    "void main() {\n"
				    "  gl_FragColor = vec4(1.0, 1.0, 1.0, 0.5);\n"
				    "}\n";
	if (!setupGraphics(gVertexShader, gFragmentShader, gProgram, gvPositionHandle))
		LOGE("Controller init error");
}

void Controller::render(int width, int height) {
	glUseProgram(gProgram);

	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, createCircle(width, height, radius, center_x, center_y));
	glEnableVertexAttribArray(gvPositionHandle);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 30);

	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, createCircle(width, height, radius, -center_x, center_y));
	glEnableVertexAttribArray(gvPositionHandle);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 30);
}
