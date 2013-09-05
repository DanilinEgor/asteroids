#ifndef ASTEROID_H_
#define ASTEROID_H_

class Asteroid {
public:
	Asteroid(int, int, bool, float, float, unsigned int, unsigned int);
	~Asteroid();
	void init(unsigned int&, unsigned int&);
	void render();
	float getX();
	float getY();
	bool getBig();
	float* getVertices();
	int getNumOfVertices();
	float MAX_RADIUS, MIN_RADIUS;
private:
	float x, y, angle, speed;
	int numOfVertices;
	unsigned int gProgram, gvPositionHandle;
	float* vertices;
	bool big;
};


#endif
