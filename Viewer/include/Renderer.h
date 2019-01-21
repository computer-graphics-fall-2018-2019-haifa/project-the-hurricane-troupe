#pragma once
#include "Scene.h"
#include "ImguiMenus.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "ClosestPixel.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include <memory>

/*
 * Renderer class.
 */
class Renderer
{
private:
	ShaderProgram lightShader;
	ShaderProgram colorShader;
	//Texture2D texture1;



	//===============================
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
	void colorYsInTriangle(int x, int minY, int maxY, int midX, int midY, const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, const glm::vec3& color, const GUIStore& store, const Face& face, const glm::mat4x4& transform, const Scene& scene, const MeshModel& model, int index, std::map<int, glm::vec3>& normalsMap);
	bool isPointInTriangle(int x, int y, const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, float* const pixelZ);
	glm::vec3 generateColorVariation(const glm::vec3& color, float variation);
	glm::vec3 Color256ToMinimizedScaling(const glm::vec3 & color, float variation);
	float getZOnLine(int x, int y, int x1, int y1, float z1, int x2, int y2, float z2);
	glm::vec3 generateColorFromFog(int x, int y, float pixelZ, const glm::vec3& originalColor, const GUIStore& store) const;
	glm::vec3 generateColorCorrectly(int x, int y, int midX, int midY, float pixelZ, const glm::vec3 originalColor, const Scene& scene, const GUIStore & store, const glm::mat4x4& transform, const Face& face, const MeshModel& model, int& index, const glm::vec3& originalPoint1, const glm::vec3& originalPoint2, const glm::vec3& originalPoint3, std::map<int, glm::vec3>& normalsMap) const;
	glm::vec3 generateColorFromLightSources(const glm::vec3& color, std::vector<std::shared_ptr<Light>> lights, const ShadingType& shade, const GUIStore& store, const Face& face, const glm::mat4x4& transform, int x, int y, int midX, int midY, const glm::vec3& cameraEye, const MeshModel& model, int& index, const glm::vec3& originalPoint1, const glm::vec3& originalPoint2, const glm::vec3& originalPoint3, std::map<int, glm::vec3>& normalsMap) const;
	void filterLightSources(const std::vector<std::shared_ptr<Light>>& allLights, std::vector<std::shared_ptr<LightParallelSource>>* const parallelLights, std::vector<std::shared_ptr<LightPointSource>>* const pointLights) const;
	glm::vec3 computeColorsFromPointLights(const std::vector<std::shared_ptr<LightPointSource>>& lights, const glm::vec3& color, const ShadingType& shade, const GUIStore& store, const Face& face, const glm::mat4x4& transform, int x, int y, int midX, int midY, const glm::vec3& cameraEye, const MeshModel& model, int& index, const glm::vec3& originalPoint1, const glm::vec3& originalPoint2, const glm::vec3& originalPoint3, std::map<int, glm::vec3>& normalsMap) const;
	glm::vec3 computeColorsFromParallelLights(const std::vector<std::shared_ptr<LightParallelSource>>& lights, const glm::vec3& color, const ShadingType& shade, const GUIStore& store, const Face& face, const glm::mat4x4& transform, int x, int y, int midX, int midY, const glm::vec3& cameraEye, const MeshModel& model, int& index, const glm::vec3& originalPoint1, const glm::vec3& originalPoint2, const glm::vec3& originalPoint3, std::map<int, glm::vec3>& normalsMap) const;
	glm::vec3 getLightCenter(const Light& light, const glm::mat4x4& transform) const;
	bool Renderer::getLinearInterpolationOfPoints(int x, int y, const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, float* const alpha, float* const beta, float* const gama, bool* const changed) const;
	glm::vec3 getReflectionIllumination(const Light& light, const GUIStore& store, const glm::vec3& lightCenter, const glm::vec3& face, const glm::mat4x4& transform, int x, int y, const glm::vec3& cameraEye, int& index, bool shouldPixel = true, bool isParallelLight = false) const;
	glm::vec3 boundColor(const glm::vec3& color) const;

public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render(const Scene& scene, const GUIStore& store);
	void SetViewport(Scene* const scene, int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	void drawLine(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& color);
	void colorTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color, const GUIStore& store, const Face& face, const glm::mat4x4& completeTransform, const Scene& scene, const MeshModel& model, const int& index, std::map<int, glm::vec3>& normalsMap);
	void drawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color);
	void handleFaceNormalsDrawing(Utils::Normals normalType, const GUIStore& store, const Face& face, std::vector<glm::vec3>& normalsPerPoint, const glm::vec4& originalPoint1, const glm::vec3& pixelPoint1, const glm::vec4& originalPoint2, const glm::vec3& pixelPoint2, const glm::vec4& originalPoint3, const glm::vec3& pixelPoint3, const glm::mat4x4& transform, int modelGUIIndex, const glm::vec3& colorPerFaceNormal, const glm::vec3& colorPerVertexNormal);
	void handleBoundingBoxDrawing(const GUIStore& store, int modelGUIIndex, float x1, float y1, float z1, float x2, float y2, float z2, const glm::mat4x4& transform, const glm::vec3& color);

	void drawMeshModels(const Scene& scene, const GUIStore& store);
	void drawCameraModels(const Scene& scene);
	void drawLightModels(const Scene& scene, const GUIStore& store);
	void updatePixelValues();
	// Add more methods/functionality as needed...
};
