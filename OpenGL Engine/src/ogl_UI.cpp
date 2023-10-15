/***********
* ogl_UI.cpp
***********/

#include "ogl_UI.h"

using namespace std;

// GLOBAL callbacks from GLFW (global functions needed since GLFW is written in pure C code)
void reshape_global(GLFWwindow* window, int winWidth, int winHeight)
{
	static_cast<UI_handler*>(glfwGetWindowUserPointer(window))->
		win_reshape(window, winWidth, winHeight);
}

void key_callback_global(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static_cast<UI_handler*>(glfwGetWindowUserPointer(window))->
		key_callback(window, key, scancode, action, mods);
}

void cursor_callback_global(GLFWwindow* window, double xpos, double ypos) {
	static_cast<UI_handler*>(glfwGetWindowUserPointer(window))->
		cursor_callback(window, xpos,ypos);
}



// register the callbacks with the window manager
void UI_handler::setCallbacks(GLFWwindow* window)
{
	//glfwSetFramebufferSizeCallback(window, reshape_global);
	glfwSetKeyCallback(window, key_callback_global);
	glfwSetCursorPosCallback(window, cursor_callback_global);
}



// LOCAL callbacks redirected from the GLOBAL callbacks
void UI_handler::win_reshape(GLFWwindow* window, int winWidth, int winHeight)
{
	if (winHeight != _prevWinHeight)
		winWidth = _imgAspect * winHeight;
	else
		winHeight = winWidth / _imgAspect;

	//glfwSetWindowSize(window, winWidth, winHeight);

	_prevWinWidth = winWidth;
	_prevWinHeight = winHeight;
}

void UI_handler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	switch (key)
	{
	case GLFW_KEY_W:
		if (action == GLFW_REPEAT ||action == GLFW_PRESS) W_pressed = true;
		break;
	case GLFW_KEY_A:
		if (action == GLFW_REPEAT || action == GLFW_PRESS) A_pressed = true;
		break;
	case GLFW_KEY_S:
		if (action == GLFW_REPEAT || action == GLFW_PRESS) S_pressed = true;
		break;
	case GLFW_KEY_D:
		if (action == GLFW_REPEAT || action == GLFW_PRESS) D_pressed = true;
		break;
	case GLFW_KEY_C:
		if (action == GLFW_PRESS) C_pressed = true;
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_REPEAT || action == GLFW_PRESS) SPACE_pressed = true;
		break;
	case GLFW_KEY_LEFT_CONTROL:
		if (action == GLFW_REPEAT || action == GLFW_PRESS) LCTRL_pressed = true;
		break;
	case GLFW_KEY_TAB:
		if (action == GLFW_PRESS) TAB_pressed = true;
		break;
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS) ESC_pressed = true;
		break;
	}
}

void UI_handler::cursor_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	//cout << "x " << xpos << "  y " << ypos << endl;

	if (cameraFlightMode) {
		if (firstCam) {
			lastX = xpos;
			lastY = ypos;

			//calculate initial pitch and yaw
			glm::vec3 front = CameraController::getInstance()->getFront();
			float pitch = glm::degrees(asin(front.y));
			float yaw = -glm::degrees(acos(front.x / cos(glm::radians(pitch))));
			//cout << " " << pitch << " " << yaw << endl;
			float pdiff = pitch - CameraController::getInstance()->getPitch();
			float ydiff = yaw - CameraController::getInstance()->getYaw();
			CameraController::getInstance()->addPitch(pdiff);
			CameraController::getInstance()->addYaw(ydiff);

			firstCam = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; //z reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f; // change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		CameraController::getInstance()->addYaw(xoffset);
		CameraController::getInstance()->addPitch(yoffset);

		float pitch = CameraController::getInstance()->getPitch();
		float yaw = CameraController::getInstance()->getYaw();

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		CameraController::getInstance()->setFront(glm::normalize(front));
	}
	

	

	//CameraController::getInstance()->setView();

	/*
	First try at camera motion:

	float xNDC = (xpos + 0.5f) / _imgWidth * 2.0f - 1;  // range is -1 to +1
	float yNDC = ((ypos + 0.5f) / _imgHeight * 2.0f - 1) * -1;
	cout << "x: " << xNDC << " " << "y: " << yNDC << endl;
	glm::vec3 clipVector = glm::inverse(CameraController::getInstance()->getProj()) * glm::vec4(xNDC, yNDC, 1.0, 1.0);
	cout << "Clipped x: " << clipVector.x << " y: " << clipVector.y << " z: " << clipVector.z << endl;
	glm::vec3 cross = glm::cross(clipVector, glm::vec3(0, 0, -1));
	float dot = glm::dot(clipVector, glm::vec3(0, 0, -1));
	glm::mat3 skew (0,cross.z,-cross.y,-cross.z,0, cross.x, cross.y,-cross.x,0);
	glm::mat3 newRot = glm::mat3(1.0);
	newRot = newRot + skew + (glm::matrixCompMult(skew,skew) * (1 / (1 + dot)));

	glm::mat4 currentPose = CameraController::getInstance()->getView();
	cout << "Current pose: \n" << glm::to_string(currentPose) << "\n" << "New rot: \n" << glm::to_string(newRot) << "\n" << endl;

	glm::mat4 newPose = glm::mat4(1.0);
	newPose[0] = glm::vec4(newRot[0], currentPose[0][3]);
	newPose[1] = glm::vec4(newRot[1], currentPose[1][3]);
	newPose[2] = glm::vec4(newRot[2], currentPose[2][3]);
	newPose[3] = currentPose[3];*/
}



// keyboard events
void UI_handler::keyboard_EventHandler(GLFWwindow* window,float deltaTime)
{
	glfwPollEvents();

	float cameraSpeed = .2;//static_cast<float>(2.5 * deltaTime);
	if (W_pressed) {
		//go in camera forward direction
		
		//cout << "W Pressed   " <<  endl;
		/*
		glm::mat4 oldView = glm::inverse(CameraController::getInstance()->getView());

		glm::vec3 oldPos = oldView *  glm::vec4(0, 0, 0, 1);
		glm::vec3 one  = oldView * glm::vec4(0, 0, -1, 1);
		glm::vec3 newPos = (0.1f * glm::normalize(one - oldPos)) + oldPos;
		oldView[3] = glm::vec4(newPos, 1.0);*/
		if (cameraFlightMode) {
			glm::vec3 newPos = CameraController::getInstance()->getPos();
			newPos += CameraController::getInstance()->getFront() * cameraSpeed;
			CameraController::getInstance()->setPos(newPos);
		}
		W_pressed = false;
	}

	if (A_pressed) {
		/*
		glm::mat4 oldView = glm::inverse(CameraController::getInstance()->getView());

		glm::vec3 oldPos = oldView * glm::vec4(0, 0, 0, 1);
		glm::vec3 one = oldView * glm::vec4(-1, 0, 0, 1);
		glm::vec3 newPos = (0.1f * glm::normalize(one - oldPos)) + oldPos;
		oldView[3] = glm::vec4(newPos, 1.0);

		CameraController::getInstance()->setView(glm::inverse(oldView));
		*/

		if (cameraFlightMode) {
			glm::vec3 newPos = CameraController::getInstance()->getPos();
			newPos -= glm::normalize(glm::cross(CameraController::getInstance()->getFront(), CameraController::getInstance()->getUp())) * cameraSpeed;
			CameraController::getInstance()->setPos(newPos);
		}
		A_pressed = false;
		//cout << "A Pressed" << endl;
	}
	if (S_pressed) {

		/*
		glm::mat4 oldView = glm::inverse(CameraController::getInstance()->getView());

		glm::vec3 oldPos = oldView * glm::vec4(0, 0, 0, 1);
		glm::vec3 one = oldView * glm::vec4(0, 0, 1, 1);
		glm::vec3 newPos = (0.1f * glm::normalize(one - oldPos)) + oldPos;
		oldView[3] = glm::vec4(newPos, 1.0);

		CameraController::getInstance()->setView(glm::inverse(oldView));*/
		if (cameraFlightMode) {
			glm::vec3 newPos = CameraController::getInstance()->getPos();
			newPos -= CameraController::getInstance()->getFront() * cameraSpeed;
			CameraController::getInstance()->setPos(newPos);
		}
		S_pressed = false;


		//cout << "S Pressed" << endl;
	}
	if (D_pressed) {

		/*
		glm::mat4 oldView = glm::inverse(CameraController::getInstance()->getView());
		
		glm::vec3 oldPos = oldView * glm::vec4(0, 0, 0, 1);
		glm::vec3 one = oldView * glm::vec4(1, 0, 0, 1);
		glm::vec3 newPos = (0.1f * glm::normalize(one - oldPos)) + oldPos;
		oldView[3] = glm::vec4(newPos, 1.0);

		CameraController::getInstance()->setView(glm::inverse(oldView));
		*/
		if (cameraFlightMode) {
			glm::vec3 newPos = CameraController::getInstance()->getPos();
			newPos += glm::normalize(glm::cross(CameraController::getInstance()->getFront(), CameraController::getInstance()->getUp())) * cameraSpeed;
			CameraController::getInstance()->setPos(newPos);
		}
		D_pressed = false;


		//cout << "D Pressed" << endl;
	}

	if (SPACE_pressed) {
		/*
		glm::mat4 oldView = glm::inverse(CameraController::getInstance()->getView());

		glm::vec3 oldPos = oldView * glm::vec4(0, 0, 0, 1);
		glm::vec3 one = oldView * glm::vec4(0, 1, 0, 1);
		glm::vec3 newPos = (0.1f * glm::normalize(one - oldPos)) + oldPos;
		oldView[3] = glm::vec4(newPos, 1.0);

		CameraController::getInstance()->setView(glm::inverse(oldView));
		*/
		if (cameraFlightMode) {
			glm::vec3 newPos = CameraController::getInstance()->getPos();
			newPos += CameraController::getInstance()->getUp() * cameraSpeed;
			CameraController::getInstance()->setPos(newPos);
		}
		SPACE_pressed = false;
	}

	if (LCTRL_pressed) {
		/*
		glm::mat4 oldView = glm::inverse(CameraController::getInstance()->getView());

		glm::vec3 oldPos = oldView * glm::vec4(0, 0, 0, 1);
		glm::vec3 one = oldView * glm::vec4(0, -1, 0, 1);
		glm::vec3 newPos = (0.1f * glm::normalize(one - oldPos)) + oldPos;
		oldView[3] = glm::vec4(newPos, 1.0);

		CameraController::getInstance()->setView(glm::inverse(oldView));
		*/
		if (cameraFlightMode) {
			glm::vec3 newPos = CameraController::getInstance()->getPos();
			newPos -= CameraController::getInstance()->getUp() * cameraSpeed;
			CameraController::getInstance()->setPos(newPos);
		}
		LCTRL_pressed = false;
	}

	if (C_pressed) {
		cameraFlightMode = !cameraFlightMode;

		if (cameraFlightMode) {
			//glfwSetCursorPos(window, _imgWidth / 2, _imgHeight / 2);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			firstCam = true;
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		C_pressed = false;
	}

	if (TAB_pressed) {
		currentCamera = ((currentCamera + 1) % CameraController::getInstance()->getCameraCount());
		CameraController::getInstance()->setActiveCam(currentCamera+1);

		//skipCursorUpdate = true;
		//glfwSetCursorPos(window, _imgWidth / 2, _imgHeight / 2);
		//cout << "Camera Changed" << endl;
		TAB_pressed = false;
	}

	if (ESC_pressed || glfwWindowShouldClose(window)) {
		_contLoop = false;
		ESC_pressed = false;
	}
}
