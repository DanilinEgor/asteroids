LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := asteroids
LOCAL_CFLAGS    := -Wall -Wextra
LOCAL_SRC_FILES := asteroids.cpp \
					game.cpp \
					ship.cpp \
					controller.cpp \
					space.cpp \
					asteroid.cpp \
					util.cpp
LOCAL_LDLIBS := -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
