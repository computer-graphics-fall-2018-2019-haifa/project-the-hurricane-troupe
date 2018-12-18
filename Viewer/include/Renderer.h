#pragma once
#include "Scene.h"
#include "ImguiMenus.h"
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
	void drawNormalsPerVertex(const Face& face, const std::vector<glm::vec3>& normals, const glm::vec4& originalPoint1, const glm::vec2& pixelPoint1, const glm::vec4& originalPoint2, const glm::vec2& pixelPoint2, const glm::vec4& originalPoint3, const glm::vec2& pixelPoint3, const float normalLength, const glm::mat4x4& transform, const glm::vec3& color);
	void drawNormalsPerFace(const Face& face, const glm::vec4& originalPoint1, const glm::vec2& pixelPoint1, const glm::vec4& originalPoint2, const glm::vec2& pixelPoint2, const glm::vec4& originalPoint3, const glm::vec2& pixelPoint3, const float normalLength, const glm::mat4x4& transform, const glm::vec3& color);
	float getMax(float a, float b);
	float getMax(float a, float b, float c);
	float getMax(float a, float b, float c, float d);
	float getMin(float a, float b);
	float getMin(float a, float b, float c);
	float getMin(float a, float b, float c, float d);
	void colorYsInTriangle(int x, int minY, int maxY, const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3, const glm::vec3& color);
	bool isPointInTriangle(int x, int y, const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3);

public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render(const Scene& scene, const GUIStore& store);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	void drawLine(const glm::vec2 point1, const glm::vec2 point2, const glm::vec3& color);
	void colorTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec3& color);
	void drawTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec3& color);
	void handleFaceNormalsDrawing(Utils::Normals normalType, const GUIStore& store, const Face& face, std::vector<glm::vec3>& normalsPerPoint, const glm::vec4& originalPoint1, const glm::vec2& pixelPoint1, const glm::vec4& originalPoint2, const glm::vec2& pixelPoint2, const glm::vec4& originalPoint3, const glm::vec2& pixelPoint3, const glm::mat4x4& transform, int modelGUIIndex, const glm::vec3& colorPerFaceNormal, const glm::vec3& colorPerVertexNormal);
	void handleBoundingBoxDrawing(const GUIStore& store, int modelGUIIndex, float x1, float y1, float z1, float x2, float y2, float z2, const glm::mat4x4& transform, const glm::vec3& color);

	void drawMeshModels(const Scene& scene, const GUIStore& store);
	void drawChess();
	void drawCameraModels(const Scene& scene);
	// Add more methods/functionality as needed...
};
