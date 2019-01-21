#define _USE_MATH_DEFINES

#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "ImguiMenus.h"

#define WINDOW_INITIALIZATION_FAILED -10

// Function declarations
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io, const GUIStore& store);
void Cleanup(GLFWwindow* window);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	
	// Handle mouse scrolling here...
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void window_size_callback(GLFWwindow* window, int width, int height)
{
}


int main(int argc, char **argv)
{
	// Create GLFW window
	int windowWidth = 2048, windowHeight = 1152;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "hurricane troupe rulez xxx");
	if (!window)
	{
		std::cout << "Could not open window!" << std::endl;
		return WINDOW_INITIALIZATION_FAILED;
	}

	// Move OpenGL context to the newly created window
	glfwMakeContextCurrent(window);

	// Get the current width/height of the frame buffer
	int frameBufferWidth, frameBufferHeight;
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	// Create the renderer and the scene
	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();

	// Setup ImGui
	ImGuiIO& io = SetupDearImgui(window);
	glEnable(GL_DEPTH_TEST);

	GUIStore store = GUIStore(scene);

	// Register a mouse scroll-wheel callback
	glfwSetScrollCallback(window, ScrollCallback);
	Camera mainCam = Camera(
		glm::vec4(0.0f, 0.0f, 14.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)
	);
	scene.AddCamera(mainCam);
	scene.SetActiveCameraIndex(0);

	// This is the main game loop..
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();

		// Here we build the menus for the next frame. Feel free to pass more arguments to this function call
		//DrawImguiMenus(io, scene);

		store.sync(scene);
		GenerateGUI(io, scene, store);
		
		// Render the next frame
		RenderFrame(window, scene, renderer, io, store);
    }

	// If we're here, then we're done. Cleanup memory.
	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io, const GUIStore& store)
{
	// Render the menus
	ImGui::Render();

	glfwMakeContextCurrent(window);
	// clear color buffer, zbuffer and set background color
	glm::vec4 clearColor = GetClearColor();
	//glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.w);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the scene
	renderer.Render(scene,store);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
}

//
//void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io, const GUIStore& store)
//{
//	// Render the menus
//	ImGui::Render();
//
//	// That's how you get the current width/height of the frame buffer (for example, after the window was resized)
//	int frameBufferWidth, frameBufferHeight;
//	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
//
//	// Resize handling here... (a suggestion)
//	renderer.SetViewport(&scene, frameBufferWidth, frameBufferHeight);
//
//	// Clear the frame buffer
//	renderer.ClearColorBuffer(GetClearColor());
//
//	// Render the scene
//	renderer.Render(scene, store);
//
//	// Swap buffers
//	renderer.SwapBuffers();
//
//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//	glfwSwapBuffers(window);
//
//}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}
