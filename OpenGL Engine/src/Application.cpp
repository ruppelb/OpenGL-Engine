#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "ObjectLoader.h"
#include "CameraController.h"
#include "game_objects/GameObject.h"
#include "LightController.h"
#include "Material.h"
#include "ogl_UI.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define G_WIDTH 16
#define G_HEIGHT 9
int width;
int height;

using namespace std;

//create GameObjects. Initially one for each kind of marker
std::vector<std::shared_ptr<Texture> > ui_textures;
std::vector<std::shared_ptr<GameObject> > gameObjects;

std::shared_ptr<GameObject> cameraZero;
std::shared_ptr<GameObject> cameraOne;

std::shared_ptr<GameObject> lightZero;
std::shared_ptr<GameObject> lightOne;

//TODO: Add gameobjects for the two initial cameras and light source. Also add one initial primitive shape

//---------------------------------------------------------------------------
void reshape(GLFWwindow *window, int in_width, int in_height)
{
	//---------------------------------------------------------------------------
	// Set a whole-window viewport

	width = in_width;
	height = in_height;
	glViewport(0, 0, (GLsizei)in_width, (GLsizei)in_height);

	float near = 0.1f;
	float far = 500.0f;
	int max_d = cv::max(in_width, in_height);
	float fx = max_d;
	float fy = max_d;
	float cx = (float)in_width / 2.0f;
	float cy = (float)in_height / 2.0f;

	float A = -(far + near) / (far - near);
	float B = -(2 * far * near) / (far - near);
	glm::mat4 projt = {fx / cx, 0.0f, 0.0f, 0.0f,
					   0.0f, fy / cy, 0.0f, 0.0f,
					   0.0f, 0.0f, A, B,
					   0.0f, 0.0f, -1.0f, 0.0f};

	CameraController::getInstance()->setProjection(glm::transpose(projt));

	/*
	// Calculate the proper aspect ratio to use based on window ratio
	float ratioX = width / (float)G_WIDTH;
	float ratioY = height / (float)G_HEIGHT;
	float ratio = ratioX < ratioY ? ratioX : ratioY;
	// Calculate the width and height that the will be rendered to
	float viewWidth = (int) (G_WIDTH * ratio);
	float viewHeight = (int) (G_HEIGHT * ratio);
	// Calculate the position, which will apply proper "pillar" or "letterbox"
	float viewX = (int) ((width - G_WIDTH * ratio) / 2);
	float viewY = (int)((height - G_HEIGHT * ratio) / 2);
	// Apply the viewport and switch back to the GL_MODELVIEW matrix mode
	glViewport(viewX, viewY, viewWidth, viewHeight);*/
}

void initGL(int argc, char *argv[])
{

	glClearColor(0.1, 0.1, 0.1, 1.0);

	// Enable and set depth parameters
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	//texture Blending
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

glm::mat4 createProj(int in_width, int in_height)
{
	float near = 0.1f;
	float far = 500.0f;
	int max_d = cv::max(in_width, in_height);
	float fx = max_d;
	float fy = max_d;
	float cx = (float)in_width / 2.0f;
	float cy = (float)in_height / 2.0f;

	float A = -(far + near) / (far - near);
	float B = -(2 * far * near) / (far - near);
	glm::mat4 projt = {fx / cx, 0.0f, 0.0f, 0.0f,
					   0.0f, fy / cy, 0.0f, 0.0f,
					   0.0f, 0.0f, A, B,
					   0.0f, 0.0f, -1.0f, 0.0f};
	return glm::transpose(projt);
}

std::shared_ptr<Renderer> loadAssets()
{

	//load objects
	ObjectLoader *objL = ObjectLoader::getInstance();
	//std::vector<Mesh> meshesObj1 = objL->loadObjectFromFile("res/objects/monkey.obj", Meshtype::PN);
	//Mesh cubeMesh = objL->loadCubeVTN();

	std::vector<std::shared_ptr<Mesh>> cubeMesh = objL->loadObjectFromFile("res/objects/cube.obj",Meshtype::PNC);
	std::vector<std::shared_ptr<Mesh>> desertMesh = objL->loadObjectFromFile("res/objects/desert.obj", Meshtype::PNC);
	std::vector<std::shared_ptr<Mesh>> cameraMesh = objL->loadObjectFromFile("res/objects/camera.obj", Meshtype::PNC);
	std::vector<std::shared_ptr<Mesh>> pointLightMesh = objL->loadObjectFromFile("res/objects/pointLight.obj", Meshtype::PNC);
	std::vector<std::shared_ptr<Mesh>> directionalLightMesh = objL->loadObjectFromFile("res/objects/directionalLight.obj", Meshtype::PNC);
	objL->~ObjectLoader();

	//load shaders
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("res/shaders/lightingMaterial.shader");
	std::shared_ptr<Shader> normalShader = std::make_shared<Shader>("res/shaders/normal.shader");

	//load textures
	//std::shared_ptr<Texture> containerDiffuseTexture = std::make_shared<Texture>("res/textures/container_d.png");
	//std::shared_ptr<Texture> containerSpecularTexture = std::make_shared<Texture>("res/textures/container_s.png");

	//create materials
	//Material gold (glm::vec4(0.24725,0.1995,0.0745,1.0),glm::vec4(0.75164,0.60648,0.22648,1.0),glm::vec4(0.628281,0.555802,0.366065,1.0),0.1);
	//Material box(containerDiffuseTexture, containerSpecularTexture, 10.0,shader);

	//create renderer
	std::shared_ptr<Renderer> renderer;

	//create initial objects

	//cameras
	cameraZero = std::make_shared<GameObject>("CameraZero",cameraMesh, shader, renderer);
	cameraZero->setHidden(false);
	gameObjects.push_back(cameraZero);

	cameraOne = std::make_shared<GameObject>("CameraOne", cameraMesh, shader, renderer);
	cameraOne->setHidden(false);
	gameObjects.push_back(cameraOne);
	
	//light
	lightZero = std::make_shared<GameObject>("LightZero",directionalLightMesh, shader, renderer);
	lightZero->setHidden(false);
	gameObjects.push_back(lightZero);

	lightOne = std::make_shared<GameObject>("LightOne",pointLightMesh, shader, renderer);
	lightOne->setHidden(false);
	gameObjects.push_back(lightOne);
	
	std::shared_ptr<GameObject> test = std::make_shared<GameObject>("TestMesh", desertMesh, shader, renderer);
	//test->setModelMatrix(glm::rotate(glm::translate(glm::mat4(1.0), ), glm::radians(45.0f), glm::vec3(0.0f,	1.0f, 0.0f)));
	test->setTranslation(glm::vec3(0, 0, -10));
	test->setRotationY(glm::radians(45.0f));

	test->setScale(glm::vec3(0.1));
	test->setHidden(false);
	gameObjects.push_back(test);

	return renderer;
}

void renderUI()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//TODO: render UI



	for (std::shared_ptr<GameObject> object : gameObjects) {
		//if (object == lightOne) {
			object->onImGuiRender();
		//}
	}

	//finally render ImGUI windwos
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(int argc, char *argv[])
{

	//-------------
	// Setup OpenGL
	//-------------

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char *glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char *glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
																   //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	GLFWwindow *window;

	if (!glfwInit())
		return -1;

	//get primary monitor width and height
	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width = mode->width, height = mode->height;
	

	//at the moment windowed mode.Add glfwGetPrimaryMonitor() for full screen
	//                                                    \/
	window = glfwCreateWindow(width, height, "", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//glfwMaximizeWindow(window);
	//glfwFocusWindow(window);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//-----------------
	// user interaction
	//-----------------
	UI_handler UI(width, height);

	glfwSetWindowUserPointer(window, &UI);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	UI.setCallbacks(window);
	glfwSetFramebufferSizeCallback(window, reshape);
	reshape(window, width, height);
	// Initialize the GL library
	initGL(argc, argv);

	// Initialize Glew with valid rendering context
	if (glewInit() != GLEW_OK)
		return -1;

	{

		//create renderer and load assets
		std::shared_ptr<Renderer> renderer = loadAssets();

		//setup Lighting
		LightController *lightController = LightController::getInstance();
		glm::vec3 newDir = glm::normalize(glm::vec3(1.0, -1.0, -1.0));
		int light0 = lightController->addDirectionalLightSource(newDir, glm::vec4(1.0, 1.0, 1.0, 1.0));

		
		//create rotation matrix from new direction and old direction
		glm::vec3 cross = glm::normalize(glm::cross(newDir, glm::vec3(0.0, 0.0, -1.0)));
		float dot = glm::dot(newDir, glm::vec3(0.0, 0.0, -1.0));
		glm::mat3 skew(0, cross.z, -cross.y, -cross.z, 0, cross.x, cross.y, -cross.x, 0);
		glm::mat3 newRot = glm::mat3(1.0);
		newRot = newRot + skew + (glm::matrixCompMult(skew, skew) * (1 / (1 + dot)));
		cout << glm::to_string(newRot) << endl;
		glm::mat4 currentPose = lightZero->getModelMatrix();
		glm::mat4 newPose = glm::mat4(1.0);
		newPose[0] = glm::vec4(newRot[0], currentPose[0][3]);
		newPose[1] = glm::vec4(newRot[1], currentPose[1][3]);
		newPose[2] = glm::vec4(newRot[2], currentPose[2][3]);
		newPose[3] = currentPose[3];
		lightZero->setModelMatrix(glm::inverse(newPose));
		

		glm::vec3 newPos = glm::vec3(10, 0.0, 0.0);
		int light1 = lightController->addLightSource(newPos,glm::vec4(1.0, 1.0, 1.0, 1.0));
		lightOne->setTranslation(newPos);


		//create CameraController
		CameraController *cameraController = CameraController::getInstance();

		//add camera
		//glm::mat4 proj = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, 0.0f, 100.0f)
		//glm::mat4 proj = glm::perspective(45.0f, (width / (float)height), 0.1f, 100.0f);
		int cam0 = cameraController->addCamera(glm::vec3(10.0, 10.0, 5.0), glm::normalize(glm::vec3(-1.0, -1.0, -1.0)), glm::vec3(0.0, 1.0, 0.0), createProj(width, height));
		int cam1 = cameraController->addCamera(glm::vec3(0.0, 0.0, 0.0), glm::normalize(glm::vec3(0.0, 0.0, -1.0)), glm::vec3(0.0, 1.0, 0.0), createProj(width, height));

		//setup ImGUI
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
		//setup font
		io.Fonts->AddFontFromFileTTF("res\\fonts\\PressStart2P-Regular.ttf", 30.0f);
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		//init time measurement
		float time = glfwGetTime();
		
		// Loop until the user closes the window
		while (UI.contLoop())
		{
			//update camera positions
			cameraZero->setModelMatrix(glm::inverse(cameraController->getView(cam0)));
			cameraOne->setModelMatrix(glm::inverse(cameraController->getView(cam1)));


			// Render here
			renderer->clear();

			//render objects
			float deltaTime = glfwGetTime() - time;
			for (size_t j = 0; j < gameObjects.size(); j++)
			{
				gameObjects[j]->onUpdate(deltaTime);
				gameObjects[j]->onRender();
			}

			//render ui
			renderUI();

			glfwSwapBuffers(window);

			// UI (GUI: event handling)
			UI.keyboard_EventHandler(window,deltaTime);

			//update time
			time = glfwGetTime();
		}

		gameObjects.clear();
		ui_textures.clear();
		cameraOne.reset();
		cameraZero.reset();
		lightZero.reset();
		lightOne.reset();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
