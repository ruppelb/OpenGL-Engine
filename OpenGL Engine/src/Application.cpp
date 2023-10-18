#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "SceneManager.h"
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
std::vector<std::shared_ptr<Texture>> ui_textures;
SceneManager* sceneManager;

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

void renderUI()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//TODO: render UI
	ImGui::Begin("Objects");

	sceneManager->renderImGui();

	ImGui::End();

	

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
		//Load scene and create Renderer
		sceneManager = SceneManager::getInstance();
		sceneManager->init(width, height);
		std::shared_ptr<Renderer> renderer = sceneManager->loadInitialScene(); 

		//setup ImGUI
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
		//setup font
		io.Fonts->AddFontFromFileTTF("res\\fonts\\PressStart2P-Regular.ttf", 10.0f);
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		//init time measurement
		float time = glfwGetTime();
		float deltaTime = 0.0;
		
		// Loop until the user closes the window
		while (UI.contLoop())
		{
			//update time
			float currentTime = static_cast<float>(glfwGetTime());
			deltaTime = currentTime - time;
			time = currentTime;

			// Render here
			renderer->clear();

			//render objects
			sceneManager->updateObjects(deltaTime);
			sceneManager->renderObjects();

			//render ui
			renderUI();

			glfwSwapBuffers(window);

			// UI (GUI: event handling)
			UI.keyboard_EventHandler(window,deltaTime);
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
