#include "game.h"
#include "ship.h"
#include "controller.h"
#include "space.h"
#include "asteroid.h"
#include "util.h"
#include "jni.h"

#include <android/log.h>

#include <ctime>
#include <unistd.h>

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

void on_render() {
	glClear(GL_COLOR_BUFFER_BIT);

	asteroidTimer++;
	asteroidTimer %= 50;
	if (!asteroidTimer){
		Asteroid* asteroid = new Asteroid(gameW, gameH, true, 0, 0, asteroidProgram, asteroidHandle);
		if (!asteroidProgram)
			asteroid->init(asteroidProgram, asteroidHandle);
		asteroids.push_back(asteroid);
	}


    ship->render(gameW, gameH);

    bullets = ship->getBullets();

#if 0
    LOGI("GAME asteroids.size = %d", asteroids.size());
#endif

    std::vector<Asteroid*>::iterator it = asteroids.begin();
    bool toCreateNewAsteroid = false;
    bool toReset = false;
    float xNewAsteroid, yNewAsteroid;
    while (it < asteroids.end()) {
    	float x = (*it)->getX(), y = (*it)->getY();
    	if (checkIntersectionShipAndAsteroid(ship, *it)) {
    		toReset = true;
    		break;
    	}
    	bool intersectedWithBullet = false;
    	for (std::vector<Bullet>::iterator itb = bullets->begin(); itb < bullets->end(); ++itb) {
    		if (checkIntersectionBulletAndAsteroid(itb, *it)) {
    			bullets->erase(itb);
    			intersectedWithBullet = true;
    			break;
    	    }
    	}
    	if (intersectedWithBullet) {
    		if ((*it)->getBig()) {
    			toCreateNewAsteroid = true;
    			xNewAsteroid = x;
    			yNewAsteroid = y;
    		}
    		(*it)->~Asteroid();
    		asteroids.erase(it++);
    		continue;
    	}
    	if (x < -2.5f || x > 2.5f || y < -2.5f || y > 2.5f) {
    		(*it)->~Asteroid();
    		asteroids.erase(it++);
    		continue;
    	}
    	else {
    		(*it)->render();
    		++it;
    	}
    }

    if (toCreateNewAsteroid) {
		for (int i = 0; i < 3; ++i) {
			Asteroid* asteroid = new Asteroid(gameW, gameH, false, xNewAsteroid, yNewAsteroid, asteroidProgram, asteroidHandle);
			asteroids.push_back(asteroid);
		}
	}

    if (toReset) {
    	sleep(2);
		ship->setAcceleration(0);
		ship->setDirectionAngle(0);
		ship->setRotationAngle(0);
		ship->setSpeed(0);
		ship->setX(0);
		ship->setY(0);
		bullets->clear();
    	for (std::vector<Asteroid*>::iterator ita = asteroids.begin(); ita < asteroids.end(); ++ita)
    		(*ita)->~Asteroid();
    	asteroids.clear();
    }

    space->render(gameW, gameH);
    controller->render(gameW, gameH);
}

void on_touch(float* xs, float* ys, int touchCount, int ev) {
	float acceleration = 0, angle = 0;
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
	}
}
