#ifndef SPACE_H_
#define SPACE_H_

class Space {
public:
	Space();
	~Space();
	void init();
	void render(int, int);
private:
	unsigned int gProgram, gvPositionHandle;
};
#endif
