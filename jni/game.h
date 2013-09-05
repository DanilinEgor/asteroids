#ifndef GAME_H_
#define GAME_H_

void on_init(int, int);
void on_render();
void on_resize();
void on_pause();
void on_resume();
void on_touch(float*, float*, int, int);
void on_done();

#endif
