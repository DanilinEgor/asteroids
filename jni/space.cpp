#include "space.h"
#include "util.h"
#include <android/log.h>
#include <GLES2/gl2.h>
#include <algorithm>

#define APPNAME "com.danegor.asteroidsgame"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,APPNAME,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,APPNAME,__VA_ARGS__)

Space::Space() {
}

void Space::init() {
	const char gVertexShader[] =
					"attribute vec4 vPosition;\n"
				    "void main() {\n"
				    "  gl_Position = vPosition;\n"
				    "}\n";
	const char gFragmentShader[] =
					"precision mediump float;\n"
					"void main() {\n"
			    	"  gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0);\n"
			    	"}\n";
	if (!setupGraphics(gVertexShader, gFragmentShader, gProgram, gvPositionHandle))
		LOGE("Space init error");
}

void Space::render(int w, int h) {
	glUseProgram(gProgram);

	GLfloat vertices1[] = {
			-1.0f, 1.0f,
			-1.0f, -1.0f,
			-3.0f / 2.0f * (float) h / w, -1.0f,
			-3.0f / 2.0f * (float) h / w, 1.0f
	};

	GLfloat vertices2[] = {
				3.0f / 2.0f * (float) h / w, 1.0f,
				3.0f / 2.0f * (float) h / w, -1.0f,
				1.0f, -1.0f,
				1.0f, 1.0f
		};

	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices1);
	glEnableVertexAttribArray(gvPositionHandle);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices2);
	glEnableVertexAttribArray(gvPositionHandle);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

