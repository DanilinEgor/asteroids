#include "game.h"
#include "ship.h"
#include "controller.h"
#include "space.h"
#include "asteroid.h"
#include "util.h"
#include "jni.h"

#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <math.h>
#include <ctime>
#include <time.h>
#include <vector>

#define LOG_TAG "com.danegor.asteroidsgame"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

Ship* ship;
Controller* controller;
Space* space;
Asteroid* asteroid;
std::vector<Asteroid*> asteroids;
std::vector<Bullet>* bullets;
int gameW, gameH;
int asteroidTimer;
unsigned int asteroidProgram = 0, asteroidHandle = 0;

float time1 = 0, time2 = 0, time3 = 0, time4 = 0;

void on_init(int width, int height) {
	srand((unsigned)time(0));

	glViewport(0, 0, width, height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	gameW = width;
	gameH = height;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	ship = new Ship(gameW, gameH);
    ship->init();

    space = new Space();
    space->init();

    controller = new Controller(gameW, gameH);
    controller->init();
}

void on_resize() {
    // No-op
}

void on_render() {
	glClear(GL_COLOR_BUFFER_BIT);

	timeval start;
	timeval end;

	LOGI("GAME asteroidProgram=%i", asteroidProgram);

	asteroidTimer++;
	asteroidTimer %= 50;
	if (!asteroidTimer){
		Asteroid* asteroid = new Asteroid(gameW, gameH, true, 0, 0, asteroidProgram, asteroidHandle);
		if (!asteroidProgram)
			asteroid->init(asteroidProgram, asteroidHandle);
		asteroids.push_back(asteroid);
	}


	gettimeofday(&start, NULL);
    ship->render(gameW, gameH);
    gettimeofday(&end, NULL);
    LOGI("GAME 1 time=%i msec", (end.tv_usec - start.tv_usec));

    bullets = ship->getBullets();

#if 1
    LOGI("GAME asteroids.size = %d", asteroids.size());
#endif

	gettimeofday(&start, NULL);
    std::vector<Asteroid*>::iterator it = asteroids.begin();
    bool toCreateNewAsteroid = false;
    float xNewAsteroid, yNewAsteroid;
    while (it < asteroids.end()) {
    	LOGI("GAME 01");
    	LOGI("GAME null? %d", (*it)==NULL);
    	float x = (*it)->getX(), y = (*it)->getY();
    	LOGI("GAME 02");
    	if (checkIntersectionShipAndAsteroid(ship, *it)) {
    		LOGI("SHIP crashed");
    		exit(0);
    	}
    	LOGI("GAME 1");
    	bool intersectedWithBullet = false;
    	for (std::vector<Bullet>::iterator itb = bullets->begin(); itb < bullets->end(); ++itb) {
    		if (checkIntersectionBulletAndAsteroid(itb, *it)) {
    			bullets->erase(itb);
    			intersectedWithBullet = true;
    			break;
    	    }
    	}
    	LOGI("GAME 2");
    	if (intersectedWithBullet) {
    		LOGI("GAME 7");
    		if ((*it)->getBig()) {
    			toCreateNewAsteroid = true;
    			xNewAsteroid = x;
    			yNewAsteroid = y;
    		}
    		LOGI("GAME 8");
    		std::vector<Asteroid*>::iterator ita(it);
    		++ita;
    		(*it)->~Asteroid();
    		asteroids.erase(it);
    		it = ita;
    		LOGI("GAME 9");
    		continue;
    	}
    	LOGI("GAME 3");
    	if (x < -2.5f || x > 2.5f || y < -2.5f || y > 2.5f) {
    		LOGI("GAME 41");
    		std::vector<Asteroid*>::iterator ita(it);
    		++ita;
    		(*it)->~Asteroid();
    		asteroids.erase(it);
			it = ita;
    		LOGI("GAME 42");
    		continue;
    	}
    	else {
    		LOGI("GAME 5");
    		(*it)->render();
    		++it;
    	}
    	LOGI("GAME 6");
    }
    gettimeofday(&end, NULL);
    LOGI("GAME 2 time=%i msec", (end.tv_usec - start.tv_usec));

    if (toCreateNewAsteroid) {
		for (int i = 0; i < 3; ++i) {
			Asteroid* asteroid = new Asteroid(gameW, gameH, false, xNewAsteroid, yNewAsteroid, asteroidProgram, asteroidHandle);
			asteroids.push_back(asteroid);
		}
	}

    gettimeofday(&start, NULL);

    space->render(gameW, gameH);

    gettimeofday(&end, NULL);
    LOGI("GAME 3 time=%i msec", (end.tv_usec - start.tv_usec));

    gettimeofday(&start, NULL);

    controller->render(gameW, gameH);

    gettimeofday(&end, NULL);
    LOGI("GAME 4 time=%i msec", (end.tv_usec - start.tv_usec));

}



void on_pause() {
}

void on_resume() {
}

void on_touch(float* xs, float* ys, int touchCount, int ev) {
	float acceleration = 0, angle = 0; //, predAngle = ship->getDirectionAngle();
	if (controller->getAccelerationAndDirectionAngle(gameW, gameH, xs, ys, touchCount, acceleration, angle)) {
		ship->setAcceleration(acceleration);
		ship->setDirectionAngle(angle);
	}
	float rotAngle = controller->getRotationAngle(gameW, gameH, xs, ys, touchCount);
	if (rotAngle != 0) 	ship->setRotationAngle(rotAngle);

#if 0
	LOGI("GAME ev=%d", ev);
	LOGI("GAME acc = %f", acceleration);
	LOGI("GAME angle = %f", angle);
#endif

	if (ev == 1 || ev == 6) {
		ship->setAcceleration(0);
//		ship->setDirectionAngle(predAngle);
	}
}

void on_done() {
}
