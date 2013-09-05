#ifndef SHIP_H_
#define SHIP_H_
#include <vector>

struct Bullet {
	float x1, y1, x2, y2, angle, length;
};

class Ship {
public:
	Ship(int, int);
	~Ship();
	void init();
	void render(int, int);
	void setSpeed(float);
	void setRotationAngle(float);
	void setDirectionAngle(float);
	void setAcceleration(float);
	float getSpeed();
	float getAcceleration();
	float getDirectionAngle();
	float getRotationAngle();
	void drawBullets(int, int);
	std::vector<Bullet>* getBullets();
	void getVertices(float*&);
	float getX();
	float getY();
private:
	float MAX_SPEED, SPEED_DECREASE, BULLETS_SPEED;
	int BULLET_TIMER;
	float x, y, dirAngle, rotAngle, speed, acceleration, ky, kx;
	unsigned int gProgram, gvPositionHandle;
	std::vector<Bullet> bullets;
	int bulletTimer;
};


#endif
