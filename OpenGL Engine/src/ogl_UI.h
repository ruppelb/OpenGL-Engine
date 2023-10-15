#pragma once

/*********
* ogl_UI.h
*********/

#ifndef ogl_UI_H
#define ogl_UI_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "CameraController.h"

//--------------
class UI_handler
//--------------
{
	bool W_pressed = false;
	bool A_pressed = false;
	bool S_pressed = false;
	bool D_pressed = false;
	bool SPACE_pressed = false;
	bool LCTRL_pressed = false;
	bool ESC_pressed = false;
	bool TAB_pressed = false;
	bool C_pressed = false;

	bool _contLoop = true;

	int _imgWidth = 1.0f;
	int _imgHeight = 1.0f;
	float _imgAspect = 1.0f;
	int _prevWinWidth = 0.0f;
	int _prevWinHeight = 0.0f;

	float lastX = 0;
	float lastY = 0;
	int currentCamera = 0;
	bool firstCam = true;
	bool cameraFlightMode = false;

public:
	// constructors
	UI_handler(int w, int h) {
		_imgWidth = w;
		_imgHeight = h;
		_imgAspect = (float)w / (float)h;
	}

	// getters and setters
	bool contLoop() { return _contLoop; }

	// methods
	void setCallbacks(GLFWwindow* window);
	void win_reshape(GLFWwindow* window, int winWidth, int winHeight);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursor_callback(GLFWwindow* window, double xposIn, double yposIn);

	void keyboard_EventHandler(GLFWwindow* window, float deltaTime);
};

#endif
