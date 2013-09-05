#include "ship.h"
#include "util.h"
#include <android/log.h>

#include <algorithm>
#include <math.h>
#include <vector>

#define _USE_MATH_DEFINES

#define APPNAME "com.danegor.asteroidsgame"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,APPNAME,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,APPNAME,__VA_ARGS__)


Ship::Ship(int w, int h) {
	x = 0;
	y = 0;
	dirAngle = 0;
	rotAngle = 0;
	speed = 0;
	acceleration = 0;
	ky = (float) h / w / 10;
	kx = ky * (float) h / w;
	MAX_SPEED = (float) h / w / 25;
	SPEED_DECREASE = (float) h / w / 500;
	BULLET_TIMER = 10;
	BULLETS_SPEED = (float) h / w / 20;
}

void Ship::init() {
	const char gVertexShader[] =
				"attribute vec4 vPosition;\n"
			    "void main() {\n"
			    "  gl_Position = vPosition;\n"
			    "}\n";
	const char gFragmentShader[] =
			    "precision mediump float;\n"
			    "void main() {\n"
			    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
			    "}\n";
	if (!setupGraphics(gVertexShader, gFragmentShader, gProgram, gvPositionHandle))
		LOGE("Ship init error");
}

void Ship::render(int w, int h) {
	glUseProgram(gProgram);

	speed += acceleration;

	if (speed > MAX_SPEED)
		speed = MAX_SPEED;

	x += speed * cos(dirAngle);
	y += speed * sin(dirAngle);

#if 0
	LOGI("SHIP speed=%f", speed);
	LOGI("SHIP acceleration=%f", acceleration);
	LOGI("SHIP x=%f", x);
	LOGI("SHIP y=%f", y);
	LOGI("SHIP rotangle = %f", rotAngle);
#endif

	const GLfloat shipVertices[] = {
			(x + kx * powf(2, 0.5) * cos(rotAngle)), (y + ky * powf(2, 0.5) * sin(rotAngle)),
			(x + kx * cos(rotAngle - 3 * M_PI / 4)), (y + ky * sin(rotAngle - 3 * M_PI / 4)),
			(x + kx * cos(rotAngle + 3 * M_PI / 4)), (y + ky * sin(rotAngle + 3 * M_PI / 4))
	};

	speed -= (speed >= SPEED_DECREASE ? SPEED_DECREASE : speed);

	if (x > 3.0f / 2.0f * (float) h / w + kx)
		x -= 3.0f * (float) h / w + 2 * kx;
	if (x < -3.0f / 2.0f * (float) h / w - kx)
		x += 3.0f * (float) h / w + 2 * kx;
	if (y > 1.0f + ky)
		y -= 2.0f + 2 * ky;
	if (y < -1.0f - ky)
		y += 2.0f + 2 * ky;

	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, shipVertices);
	glEnableVertexAttribArray(gvPositionHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	bulletTimer++;
	bulletTimer %= BULLET_TIMER;
	if (!bulletTimer) {
		Bullet b;
		b.length = (float) h / w / 10;
		b.angle = rotAngle;
		b.x1 = x + kx * powf(2, 0.5) * cos(rotAngle);
		b.y1 = y + ky * powf(2, 0.5) * sin(rotAngle);
		b.x2 = b.x1 + b.length * (float) h / w * cos(b.angle);
		b.y2 = b.y1 + b.length * sin(b.angle);
		bullets.push_back(b);
	}

	drawBullets(w, h);
}

void Ship::setSpeed(float s) {
	speed = s;
}

void Ship::setAcceleration(float a){
	acceleration = a;
}

void Ship::setRotationAngle(float a) {
	rotAngle = a;
}

void Ship::setDirectionAngle(float a) {
	dirAngle = a;
}

float Ship::getDirectionAngle() {
	return dirAngle;
}

float Ship::getSpeed() {
	return speed;
}

float Ship::getAcceleration() {
	return acceleration;
}

float Ship::getRotationAngle() {
	return dirAngle;
}

void Ship::drawBullets(int w, int h) {
	for (std::vector<Bullet>::iterator it = bullets.begin(); it < bullets.end(); it++) {
		float 	x1 = it->x1,
				y1 = it->y1,
				x2 = it->x2,
				y2 = it->y2,
				angle = it->angle;

		if (x1 > 1.0f || x1 < -1.0f || y1 > 1.0f || y1 < -1.0f) {
			bullets.erase(it);
			continue;
		}

		GLfloat vertices[] = {
				x1, y1,
				x2, y2
		};
		glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		glEnableVertexAttribArray(gvPositionHandle);
		glDrawArrays(GL_LINES, 0, 2);

		it->x1 += BULLETS_SPEED * cos(angle) * (float) h / w;
		it->y1 += BULLETS_SPEED * sin(angle);
		it->x2 += BULLETS_SPEED * cos(angle) * (float) h / w;
		it->y2 += BULLETS_SPEED * sin(angle);
	}
}

std::vector<Bullet>* Ship::getBullets() {
	return &bullets;
}

void Ship::getVertices(float* &vertices) {
	vertices[0] = x + kx * powf(2, 0.5) * cos(rotAngle);
	vertices[1] = y + ky * powf(2, 0.5) * sin(rotAngle);
	vertices[2] = x + kx * cos(rotAngle - 3 * M_PI / 4);
	vertices[3] = y + ky * sin(rotAngle - 3 * M_PI / 4);
	vertices[4] = x + kx * cos(rotAngle + 3 * M_PI / 4);
	vertices[5] = y + ky * sin(rotAngle + 3 * M_PI / 4);
}

float Ship::getX() {
	return x;
}

float Ship::getY() {
	return y;
}
