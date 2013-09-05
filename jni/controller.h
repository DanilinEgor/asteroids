#ifndef CONTROLLER_H_
#define CONTROLLER_H_

class Controller {
public:
	Controller(int, int);
	~Controller();
	void init();
	void render(int, int);
	bool getAccelerationAndDirectionAngle(int, int, float*, float*, int, float&, float&);
	float getRotationAngle(int, int, float*, float*, int);
private:
	float center_x, center_y, radius;
	unsigned int gProgram, gvPositionHandle;
};
#endif
