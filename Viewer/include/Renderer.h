#pragma once
#include "Scene.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

/*
 * Renderer class.
 */
class Renderer
{
private:
	float *colorBuffer;
	float *zBuffer;
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;

	void putPixel(int x, int y, const glm::vec3& color);
	void createBuffers(int viewportWidth, int viewportHeight);

	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();
	void plotLineHigh(int x1, int y1, int x2, int y2, const glm::vec3& color);
	void plotLineLow(int x1, int y1, int x2, int y2, const glm::vec3& color);
	glm::vec2 translatePointIndicesToPixels (const glm::vec4& point, const glm::mat4x4& fullTransform);

public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	void drawLine(glm::vec2 point1, glm::vec2 point2, const glm::vec3& color);
	void drawTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec3& color);

	void drawModels(const Scene& scene);
	void drawChess();
	// Add more methods/functionality as needed...
};
