#pragma once
#include "Scene.h"
#include "ImguiMenus.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "ClosestPixel.h"
#include <memory>

/*
 * Renderer class.
 */
class Renderer
{
private:
	float *colorBuffer;
	ClosestPixel zBuffer;
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;

	void colorPixel(int x, int y, float z, const glm::vec3& color);
	void putPixel(int x, int y, const glm::vec3& color);
	void createBuffers(int viewportWidth, int viewportHeight);

	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();
	void plotLineHigh(int x1, int y1, float z1, int x2, int y2, float z2, const glm::vec3& color);
	void plotLineLow(int x1, int y1, float z1, int x2, int y2, float z2, const glm::vec3& color);
	glm::vec3 translatePointIndicesToPixels (const glm::vec4& point, const glm::mat4x4& fullTransform) const;
	void drawNormalsPerVertex(const Face& face, const std::vector<glm::vec3>& normals, const glm::vec4& originalPoint1, const glm::vec3& pixelPoint1, const glm::vec4& originalPoint2, const glm::vec3& pixelPoint2, const glm::vec4& originalPoint3, const glm::vec3& pixelPoint3, const float normalLength, const glm::mat4x4& transform, const glm::vec3& color);
	void drawNormalsPerFace(const Face& face, const glm::vec4& originalPoint1, const glm::vec2& pixelPoint1, const glm::vec4& originalPoint2, const glm::vec2& pixelPoint2, const glm::vec4& originalPoint3, const glm::vec2& pixelPoint3, const float normalLength, const glm::mat4x4& transform, const glm::vec3& color);
	float getMax(float a, float b);
	float getMax(float a, float b, float c);
	float getMax(float a, float b, float c, float d);
	float getMin(float a, float b);
	float getMin(float a, float b, float c);
	float getMin(float a, float b, float c, float d);
	void colorYsInTriangle(int x, int minY, int maxY, const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, const glm::vec3& color, const GUIStore& store, const Face& face, const glm::mat4x4& transform, const Scene& scene, const MeshModel& model);
	bool isPointInTriangle(int x, int y, const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, float* const pixelZ);
	glm::vec3 generateColorVariation(const glm::vec3& color, float variation);
	glm::vec3 Color256ToMinimizedScaling(const glm::vec3 & color, float variation);
	float getZOnLine(int x, int y, int x1, int y1, float z1, int x2, int y2, float z2);
	glm::vec3 generateColorFromFog(int x, int y, float pixelZ, const glm::vec3& originalColor, const GUIStore& store) const;
	glm::vec3 generateColorCorrectly(int x, int y, float pixelZ, const glm::vec3 originalColor, const Scene& scene, const GUIStore & store, const glm::mat4x4& transform, const Face& face, const MeshModel& model) const;
	glm::vec3 generateColorFromAmbientLighting(const glm::vec3& ambientColor, const float intensity, const glm::vec3& color) const;
	glm::vec3 generateColorFromLightSources(const glm::vec3& color, std::vector<std::shared_ptr<Light>> lights, const ShadingType& shade, const GUIStore& store, const Face& face, const glm::mat4x4& transform, int x, int y, const glm::vec3& cameraEye, const MeshModel& model) const;
	void filterLightSources(const std::vector<std::shared_ptr<Light>>& allLights, std::vector<std::shared_ptr<LightParallelSource>>* const parallelLights, std::vector<std::shared_ptr<LightPointSource>>* const pointLights) const;
	glm::vec3 computeColorsFromPointLights(const std::vector<std::shared_ptr<LightPointSource>>& lights, const glm::vec3& color, const ShadingType& shade, const GUIStore& store, const Face& face, const glm::mat4x4& transform, int x, int y, const glm::vec3& cameraEye, const MeshModel& model) const;
	glm::vec3 getLightCenter(const LightPointSource& light, const glm::mat4x4& transform) const;

public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render(const Scene& scene, const GUIStore& store);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(Scene* const scene, int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	void drawLine(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& color);
	void colorTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color, const GUIStore& store, const Face& face, const glm::mat4x4& completeTransform, const Scene& scene, const MeshModel& model);
	void drawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color);
	void handleFaceNormalsDrawing(Utils::Normals normalType, const GUIStore& store, const Face& face, std::vector<glm::vec3>& normalsPerPoint, const glm::vec4& originalPoint1, const glm::vec3& pixelPoint1, const glm::vec4& originalPoint2, const glm::vec3& pixelPoint2, const glm::vec4& originalPoint3, const glm::vec3& pixelPoint3, const glm::mat4x4& transform, int modelGUIIndex, const glm::vec3& colorPerFaceNormal, const glm::vec3& colorPerVertexNormal);
	void handleBoundingBoxDrawing(const GUIStore& store, int modelGUIIndex, float x1, float y1, float z1, float x2, float y2, float z2, const glm::mat4x4& transform, const glm::vec3& color);

	void drawMeshModels(const Scene& scene, const GUIStore& store);
	void drawChess();
	void drawCameraModels(const Scene& scene);
	void drawLightModels(const Scene& scene, const GUIStore& store);
	void updatePixelValues();
	float getFlatReflectionIllumination(const GUIStore& store, const glm::vec3& lightCenter, const glm::vec3& face, const glm::mat4x4& transform, int x, int y, const glm::vec3& cameraEye) const;
	// Add more methods/functionality as needed...
};
