#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtx/string_cast.hpp"

#include <vector>
#include <map>
#include <iostream>

struct Camera {
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 up;
	glm::vec3 pos;
	glm::vec3 front;
	float yaw = -90.0f;
	float pitch = 0.0f;
};

struct CameraOrtho: public Camera {
	float left;
	float right;
	float bottom;
	float top;
	float nearZ;
	float farZ;
};

struct CameraPerspective : public Camera {
	float fovy;
	float aspect;
	float nearZ;
	float farZ;
};

class CameraController {
public:

	static CameraController* getInstance();

	CameraController(CameraController const&) = delete;
	void operator=(CameraController const&) = delete;


	int addCamera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, glm::mat4 perspectiveProj);
	
	int addPerpectiveCamera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fovy, float aspect, float nearZ, float farZ);

	int addOrthoCamera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float left, float right, float bottom, float top, float nearZ, float farZ);

	void setProjection(glm::mat4 projMat, int camIndex = -1);

	void setPerspectiveProjection(float fovy, float aspect, float nearZ, float farZ, int camIndex = -1);

	void setOrthoProjection(float left, float right, float bottom , float top, float nearZ, float farZ, int camIndex = -1);

	void setView(glm::vec3 pos, glm::vec3 front, glm::vec3 up, int camIndex = -1);

	void setFront(glm::vec3 front, int camIndex = -1);

	void setUp(glm::vec3 up, int camIndex = -1);

	void setPos(glm::vec3 pos, int camIndex = -1);

	void setActiveCam(int camIndex);

	int getActiveCam();

	void removeCamera(int camIndex);

	void addPitch(float pitch, int camIndex = -1);

	void addYaw(float yaw, int camIndex = -1);

	glm::mat4 getViewProj(int camIndex = -1);

	glm::mat4 getView(int camIndex = -1);

	glm::mat4 getProj(int camIndex = -1);

	glm::vec3 getFront(int camIndex = -1);

	glm::vec3 getPos(int camIndex = -1);

	glm::vec3 getUp(int camIndex = -1);

	float getYaw(int camIndex = -1);

	float getPitch(int camIndex = -1);

	int getCameraCount();
private:

	static CameraController* inst_;   // The one, single instance

	CameraController() : activeCamIndex(-1), cameraCount(0), highestIndex(0) {};

	~CameraController();

	bool containsCameraIndex(int index);

	void updateView(int index);

	int activeCamIndex;

	int highestIndex;

	int cameraCount;

	std::map<int,Camera*> cameras;
};