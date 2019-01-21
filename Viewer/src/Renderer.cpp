#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "Utils.h"
#include "MeshModel.h"
#include "RotationRules.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>
#include <iostream>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(GetClearColor())
{
	colorShader.loadShaders(".\\..\\..\\project-the-hurricane-troupe\\Viewer\\shaders\\vshader_color.glsl", ".\\..\\..\\project-the-hurricane-troupe\\Viewer\\shaders\\fshader_color.glsl");
	SetViewport(nullptr, viewportWidth, viewportHeight, viewportX, viewportY);
}

Renderer::~Renderer()
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}
}

void Renderer::colorPixel(int x, int y, float z, const glm::vec3 & color)
{
	zBuffer.tryToSetColor(x, y, z, color);
}

void Renderer::putPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
	colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
	colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
}

void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}

	colorBuffer = new float[3 * viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			putPixel(x, y, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Renderer::SetViewport(Scene* const scene, int viewportWidth, int viewportHeight, int viewportX, int viewportY)
{
	this->viewportX = viewportX;
	this->viewportY = viewportY;
	if (scene != nullptr) {
		scene->setActiveCameraAspectRatio(this->viewportWidth, this->viewportHeight, viewportWidth, viewportHeight);
	}
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
	createBuffers(viewportWidth, viewportHeight);
	createOpenGLBuffer();
}

void Renderer::Render(const Scene& scene, const GUIStore& store)
{
	//#############################################
	//## You should override this implementation ##
	//## Here you should render the scene.       ##
	//#############################################
	zBuffer.resetColor();
	zBuffer.setAntiAliasing(store.isAntiAliased());
	drawMeshModels(scene, store);
	//drawCameraModels(scene);
	//drawLightModels(scene, store);
	//updatePixelValues();

}

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewportWidth, viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewportWidth, viewportHeight);
}



float Renderer::getMax(float a, float b)
{
	if (a > b) return a;
	return b;
}

float Renderer::getMax(float a, float b, float c)
{
	return getMax(a, b, c, c);
}


float Renderer::getMax(float a, float b, float c, float d)
{
	return getMax(getMax(a, b), getMax(c, d));
}

float Renderer::getMin(float a, float b)
{
	if (a < b) return a;
	return b;
}

float Renderer::getMin(float a, float b, float c)
{
	return getMin(a, b, c, c);
}

float Renderer::getMin(float a, float b, float c, float d)
{
	return getMin(getMin(a, b), getMin(c, d));
}

glm::vec3 Renderer::getReflectionIllumination(const Light& light, const GUIStore& store, const glm::vec3& lightCenter, const glm::vec3& faceNormal, const glm::mat4x4& transform, int x, int y, const glm::vec3& cameraEye,int& index, bool shouldPixel, bool isParallelLight) const {
	glm::vec3 Ia, Id, Is;
	//========= Ambient
	{
		Ia = store.getALightReflection(index) * store.getAmbientReflectionIntinsety(index);
	}
	glm::vec3 pixelFaceNormal = faceNormal;
	if (shouldPixel) {
		pixelFaceNormal = translatePointIndicesToPixels(glm::vec4(faceNormal, 1.0f), transform);
	}
	//========= defuse reflection
	{
		glm::vec3 Kd = store.getDiffuseReflectionIntensity(index);
		glm::vec3 Kl = store.getDiffuseLightReflection(index);
		glm::vec2 finalL = glm::normalize(glm::vec2(x - lightCenter.x, y - lightCenter.y));
		if (isParallelLight) finalL = glm::normalize(translatePointIndicesToPixels(glm::vec4(light.getDirection(), 1.0f), transform));
		glm::vec2 finalN = glm::normalize(glm::vec2(pixelFaceNormal.x - x, pixelFaceNormal.y - y));
		Id = (Kd*glm::dot(finalL, finalN))*Kl;
	}
	
	//========= specular reflection
	{
		float d = pixelFaceNormal.x - lightCenter.x;
		if (isParallelLight) {
			d = pixelFaceNormal.x - light.getDirection().x;
		}
		glm::vec3 Kd = store.getSpecularReflectionIntensity(index);
		glm::vec3 Ks = store.getSLightReflection(index);
		float shine = store.getShine(index);
		glm::vec2 reflectionPoint((lightCenter.x + abs(2 * d)), lightCenter.y);
		if (isParallelLight) {
			reflectionPoint = glm::vec2((light.getDirection().x + abs(2 * d)), light.getDirection().y);
		}
		glm::vec2 finalR = glm::normalize(glm::vec2(reflectionPoint.x - x, reflectionPoint.y - y));
		glm::vec2 finalV = glm::normalize(glm::vec2(x - cameraEye.x, y - cameraEye.y));
		float cosTeta = glm::dot(finalR, finalV);
		float _pow = pow(abs(cosTeta), shine);
		Is = (Kd*_pow*Ks);
	}
	return Ia + Id + Is;
}

glm::vec3 Renderer::boundColor(const glm::vec3 & color) const
{
	glm::vec3 boundedColor = color;
	if (boundedColor.x >= 1.0f) boundedColor.x = 1.0f;
	if (boundedColor.x <= 0.0f) boundedColor.x = 0.0f;
	if (boundedColor.y >= 1.0f) boundedColor.y = 1.0f;
	if (boundedColor.y <= 0.0f) boundedColor.y = 0.0f;
	if (boundedColor.z >= 1.0f) boundedColor.z = 1.0f;
	if (boundedColor.z <= 0.0f) boundedColor.z = 0.0f;
	return boundedColor;
}

void Renderer::colorYsInTriangle(int x, int minY, int maxY,int midX, int midY, const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, const glm::vec3& color, const GUIStore& store, const Face& face, const glm::mat4x4& transform, const Scene& scene, const MeshModel& model, int index, std::map<int, glm::vec3>& normalsMap)
{
	for (int y = maxY; y >= minY; --y) {
		float pixelZ = -1000.0f;
		if (isPointInTriangle(x, y, point1, point2, point3, &pixelZ)) {
			glm::vec3 finalColor = generateColorCorrectly(x, y, midX, midY,  pixelZ, color, scene, store, transform, face, model,index, point1, point2, point3, normalsMap);
			colorPixel(x, y, pixelZ, finalColor);
		}
	}
}

bool Renderer::getLinearInterpolationOfPoints(int x, int y, const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, float* const alpha, float* const beta, float* const gama, bool* const changed) const
{
	if (changed != nullptr) {
		*changed = false;
	}
	//source: http://totologic.blogspot.com/2014/01/accurate-point-in-triangle-test.html
	int x1 = point1.x, x2 = point2.x, x3 = point3.x;
	int y1 = point1.y, y2 = point2.y, y3 = point3.y;
	float z1 = point1.z, z2 = point2.z, z3 = point3.z;
	float diva = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
	if (diva == 0) return false;
	float a = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / diva;
	if (a < 0.0f || a > 1.0f) return false;
	float divb = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
	if (divb == 0) return false;
	float b = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / divb;
	if (b < 0.0f || b > 1.0f) return false;
	float c = 1.0f - a - b;
	if (c < 0.0f || c > 1.0f) return false;
	if (alpha == nullptr || beta == nullptr || gama == nullptr) {
		//do nothing
	}
	else {
		*alpha = a;
		*beta = b;
		*gama = c;
		if (changed != nullptr) {
			*changed = true;
		}
	}
	return true;
}

bool Renderer::isPointInTriangle(int x, int y, const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, float* const pixelZ)
{
	float a = 0.0f, b = 0.0f, c = 0.0f;
	bool value = getLinearInterpolationOfPoints(x, y, point1, point2, point3, &a, &b, &c, nullptr);
	if (value == false) return false;
	float z1 = point1.z, z2 = point2.z, z3 = point3.z;
	if (pixelZ != nullptr) {
		*pixelZ = a * z1 + b * z2 + c * z3;
	}
	return true;
}

glm::vec3 Renderer::generateColorVariation(const glm::vec3& color, float variation)
{
	return glm::vec3(
		(((int)(color.x * 255.0f + (variation))) % 255) / 255.0f,
		(((int)(color.y * 255.0f + (variation))) % 255) / 255.0f,
		(((int)(color.z * 255.0f + (variation))) % 255) / 255.0f
	);
}

glm::vec3 Renderer::Color256ToMinimizedScaling(const glm::vec3& color, float variation)
{
	return glm::vec3(
		(((int)(color.x + (variation))) % 255) / 255.0f,
		(((int)(color.y + (variation))) % 255) / 255.0f,
		(((int)(color.z + (variation))) % 255) / 255.0f
	);
}

float Renderer::getZOnLine(int x, int y, int x1, int y1, float z1, int x2, int y2, float z2)
{
	float divBeta = (y2 * x1 - y1 * x2);
	if (x1 == 0.0f || divBeta == 0.0f) return -100000;
	float beta = (y * x1 - x * y1) / divBeta;
	float alpha = (x - x2 * beta) / x1;
	float z = alpha * z1 + beta * z2;
	return z;
}

glm::vec3 Renderer::generateColorFromFog(int x, int y, float pixelZ, const glm::vec3& originalColor, const GUIStore & store) const
{
	if (store.getFog()) {
		float distance = pixelZ;
		float fogDensity = store.getFogDensity();
		glm::vec3 fogColor = store.getFogColor();
		float f = (exp(fogDensity*distance));
		glm::vec3 finalColor = ((f - 1.0f)* fogColor) / f + originalColor / f;
		return finalColor;
	}
	return originalColor;
}



glm::vec3 Renderer::generateColorFromLightSources(const glm::vec3& color, std::vector<std::shared_ptr<Light>> lights, const ShadingType& shade, const GUIStore& store, const Face& face, const glm::mat4x4& transform, int x, int y, int midX, int midY, const glm::vec3& cameraEye, const MeshModel& model, int& index, const glm::vec3& originalPoint1, const glm::vec3& originalPoint2, const glm::vec3& originalPoint3, std::map<int, glm::vec3>& normalsMap) const
{
	if (index == -1) return color;
	std::vector<std::shared_ptr<LightParallelSource>> parallelLights(0);
	std::vector<std::shared_ptr<LightPointSource>> pointLights(0);
	filterLightSources(lights, &parallelLights, &pointLights);

	glm::vec3 currentColor = color;
	currentColor = computeColorsFromParallelLights(parallelLights, currentColor, shade, store, face, transform, x, y, midX, midY, cameraEye, model, index, originalPoint1, originalPoint2, originalPoint3, normalsMap);
	currentColor = computeColorsFromPointLights(pointLights, currentColor, shade, store, face, transform, x, y, midX, midY, cameraEye, model, index, originalPoint1, originalPoint2, originalPoint3, normalsMap);
	return currentColor;
}

void Renderer::filterLightSources(const std::vector<std::shared_ptr<Light>>& allLights, std::vector<std::shared_ptr<LightParallelSource>>* const parallelLights, std::vector<std::shared_ptr<LightPointSource>>* const pointLights) const
{
	for each (std::shared_ptr<Light> light in allLights)
	{
		std::shared_ptr<LightParallelSource> PLS = std::dynamic_pointer_cast<LightParallelSource>(light);
		if (PLS != nullptr) {
			parallelLights->push_back(PLS);
		}
		std::shared_ptr<LightPointSource> LPS = std::dynamic_pointer_cast<LightPointSource>(light);
		if (LPS != nullptr) {
			pointLights->push_back(LPS);
		}
	}
}

glm::vec3 Renderer::computeColorsFromPointLights(const std::vector<std::shared_ptr<LightPointSource>>& lights, const glm::vec3& color, const ShadingType& shade, const GUIStore& store, const Face& face, const glm::mat4x4& transform, int x, int y, int midX, int midY, const glm::vec3& cameraEye, const MeshModel& model,int& index, const glm::vec3& originalPoint1, const glm::vec3& originalPoint2, const glm::vec3& originalPoint3, std::map<int, glm::vec3>& normalsMap) const
{
	glm::vec3 currentColor = color;
	for each (std::shared_ptr<LightPointSource> light in lights)
	{

		if (shade == ShadingType::FLAT) {
			x = midX; y = midY;
			currentColor += getReflectionIllumination(*light, store, getLightCenter(*light, transform), face.getFaceNormal(), transform, midX, midY, cameraEye, index);
		}
		else if (shade == ShadingType::GOURAUD)
		{
		//	//manipulate color
			//std::map<int, glm::vec3> normals = model.getNormalForVertices();
			if (face.hasNormals() == false) continue;
			glm::vec3 normal1 = translatePointIndicesToPixels(glm::vec4(normalsMap.at(face.GetVertexIndex(0)), 1.0f), transform);
			glm::vec3 normal2 = translatePointIndicesToPixels(glm::vec4(normalsMap.at(face.GetVertexIndex(1)), 1.0f), transform);
			glm::vec3 normal3 = translatePointIndicesToPixels(glm::vec4(normalsMap.at(face.GetVertexIndex(2)), 1.0f), transform);
			
			glm::vec3 colorForVertex1 = getReflectionIllumination(*light, store, getLightCenter(*light, transform), normal1, transform, x, y, cameraEye, index,false);
			glm::vec3 colorForVertex2 = getReflectionIllumination(*light, store, getLightCenter(*light, transform), normal2, transform, x, y, cameraEye, index,false);
			glm::vec3 colorForVertex3 = getReflectionIllumination(*light, store, getLightCenter(*light, transform), normal3, transform, x, y, cameraEye, index,false);
			float a = 0.0f, b = 0.0f, c = 0.0f;
			bool changed = false;
			getLinearInterpolationOfPoints(x, y, originalPoint1, originalPoint2, originalPoint3, &a, &b, &c, &changed);
			if (changed == false) continue;

			currentColor += a * colorForVertex1 + b * colorForVertex2 + c * colorForVertex3;
		}
		else if (shade == ShadingType::PHONG)
		{
			if (face.hasNormals() == false) continue;
			glm::vec3 normal1 = translatePointIndicesToPixels(glm::vec4(normalsMap[face.GetVertexIndex(0)], 1.0f), transform);
			glm::vec3 normal2 = translatePointIndicesToPixels(glm::vec4(normalsMap[face.GetVertexIndex(1)], 1.0f), transform);
			glm::vec3 normal3 = translatePointIndicesToPixels(glm::vec4(normalsMap[face.GetVertexIndex(2)], 1.0f), transform);
			float a = 0.0f, b = 0.0f, c = 0.0f;
			bool changed = false;
			getLinearInterpolationOfPoints(x, y, originalPoint1, originalPoint2, originalPoint3, &a, &b, &c, &changed);
			if (changed == false) continue;
			glm::vec3 pixelNormal = a*normal1 + b*normal2 + c*normal3;
			currentColor += getReflectionIllumination(*light, store, getLightCenter(*light, transform), pixelNormal, transform, x, y, cameraEye, index, false);
		}
	}

	//if (store.isNonUniformMaterial()) {
	//	currentColor = glm::abs(glm::sin(std::rand() / (2.0f*3.14f)))*currentColor;
	//}
	if (store.isNonUniformMaterial()) { //if beautiful non-uniform material
		// 1 -> 2.0*3.14
		// f -> ??
		//==> ?? = f*2.0*3.14/1
		float multiplier = 2.0f * 3.14f; // can be changed to 1 to a less beautiful non-uniform material..
		currentColor = glm::vec3(glm::abs(glm::sin(currentColor.x*multiplier)), glm::abs(glm::sin(currentColor.y*multiplier)), glm::abs(glm::sin(currentColor.z*multiplier)));
	}
	return boundColor(currentColor);
}

glm::vec3 Renderer::computeColorsFromParallelLights(const std::vector<std::shared_ptr<LightParallelSource>>& lights, const glm::vec3 & color, const ShadingType & shade, const GUIStore & store, const Face & face, const glm::mat4x4 & transform, int x, int y, int midX, int midY, const glm::vec3 & cameraEye, const MeshModel & model, int & index, const glm::vec3 & originalPoint1, const glm::vec3 & originalPoint2, const glm::vec3 & originalPoint3, std::map<int, glm::vec3>& normalsMap) const
{
	glm::vec3 currentColor = color;
	for each (std::shared_ptr<LightParallelSource> light in lights)
	{
		if (shade == ShadingType::FLAT) {
			if (store.isNonUniformMaterial()) {
				currentColor += getReflectionIllumination(*light, store, getLightCenter(*light, transform), face.getFaceNormal(), transform, x, y, cameraEye, index, true);
				currentColor = glm::abs(glm::sin(std::rand() / (2.0f*3.14f)))*currentColor;
			}
			currentColor += getReflectionIllumination(*light, store, getLightCenter(*light, transform), face.getFaceNormal(), transform, midX, midY, cameraEye, index, true);
		}
		else if (shade == ShadingType::GOURAUD)
		{
			if (face.hasNormals() == false) continue;
			glm::vec3 normal1 = translatePointIndicesToPixels(glm::vec4(normalsMap.at(face.GetVertexIndex(0)), 1.0f), transform);
			glm::vec3 normal2 = translatePointIndicesToPixels(glm::vec4(normalsMap.at(face.GetVertexIndex(1)), 1.0f), transform);
			glm::vec3 normal3 = translatePointIndicesToPixels(glm::vec4(normalsMap.at(face.GetVertexIndex(2)), 1.0f), transform);

			glm::vec3 colorForVertex1 = getReflectionIllumination(*light, store, getLightCenter(*light, transform), normal1, transform, x, y, cameraEye, index, false, true);
			glm::vec3 colorForVertex2 = getReflectionIllumination(*light, store, getLightCenter(*light, transform), normal2, transform, x, y, cameraEye, index, false, true);
			glm::vec3 colorForVertex3 = getReflectionIllumination(*light, store, getLightCenter(*light, transform), normal3, transform, x, y, cameraEye, index, false, true);

			float a = 0.0f, b = 0.0f, c = 0.0f;
			bool changed = false;
			getLinearInterpolationOfPoints(x, y, originalPoint1, originalPoint2, originalPoint3, &a, &b, &c, &changed);
			if (changed == false) continue;

			currentColor += a * colorForVertex1 + b * colorForVertex2 + c * colorForVertex3;
		}
		else if (shade == ShadingType::PHONG)
		{
			if (face.hasNormals() == false) continue;
			glm::vec3 normal1 = translatePointIndicesToPixels(glm::vec4(normalsMap[face.GetVertexIndex(0)], 1.0f), transform);
			glm::vec3 normal2 = translatePointIndicesToPixels(glm::vec4(normalsMap[face.GetVertexIndex(1)], 1.0f), transform);
			glm::vec3 normal3 = translatePointIndicesToPixels(glm::vec4(normalsMap[face.GetVertexIndex(2)], 1.0f), transform);
			float a = 0.0f, b = 0.0f, c = 0.0f;
			bool changed = false;
			getLinearInterpolationOfPoints(x, y, originalPoint1, originalPoint2, originalPoint3, &a, &b, &c, &changed);
			if (changed == false) continue;
			glm::vec3 pixelNormal = a * normal1 + b * normal2 + c * normal3;
			currentColor += getReflectionIllumination(*light, store, getLightCenter(*light, transform), pixelNormal, transform, x, y, cameraEye, index, false, true);
		}
	}

	return boundColor(currentColor);
}

glm::vec3 Renderer::generateColorCorrectly(int x, int y, int midX, int midY, float pixelZ, const glm::vec3 originalColor, const Scene& scene, const GUIStore & store, const glm::mat4x4& transform, const Face& face, const MeshModel& model, int& index, const glm::vec3& originalPoint1, const glm::vec3& originalPoint2, const glm::vec3& originalPoint3, std::map<int, glm::vec3>& normalsMap) const
{
	glm::vec3 newColor = originalColor;
	ShadingType shading = store.getShading();
	// Order of these calls is important!
	glm::vec3 pixelCameraEye = translatePointIndicesToPixels(scene.getActiveCamera().getEyeVector(), transform);
	newColor = generateColorFromLightSources(newColor, scene.getLights(), shading, store, face, transform, x, y, midX, midY, pixelCameraEye, model,index, originalPoint1, originalPoint2, originalPoint3, normalsMap);
	newColor = generateColorFromFog(x, y, pixelZ, newColor, store);
	// Do not change the above order of these calls!
	return newColor;
}


void Renderer::plotLineHigh(int x1, int y1, float z1, int x2, int y2, float z2, const glm::vec3& color) {
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int xi = 1;
	if (deltaX < 0) {
		xi = -1;
		deltaX = -deltaX;
	}
	int d = 2 * deltaX - deltaY;
	int x = x1;
	int y = y1;
	while (y < y2) {
		float newZ = getZOnLine(x, y, x2, y1, z1, x2, y2, z2);
		colorPixel(x, y, newZ, color);
		if (d > 0) {
			x = x + xi;
			d = d - 2 * deltaY;
		}
		d = d + 2 * deltaX;
		++y;
	}
}

void Renderer::plotLineLow(int x1, int y1, float z1, int x2, int y2, float z2, const glm::vec3& color) {
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int yi = 1;
	if (deltaY < 0) {
		yi = -1;
		deltaY = -deltaY;
	}
	int d = 2 * deltaY - deltaX;
	int x = x1;
	int y = y1;
	while (x < x2) {
		float newZ = getZOnLine(x, y, x1, y1, z1, x2, y2, z2);
		colorPixel(x, y, newZ, color);
		if (d > 0) {
			y = y + yi;
			d = d - 2 * deltaX;
		}
		d = d + 2 * deltaY;
		++x;
	}
}

glm::vec3 Renderer::translatePointIndicesToPixels(const glm::vec4 & _point, const glm::mat4x4 & fullTransform) const
{
	glm::vec4 point = fullTransform * _point;

	float wDivision = 1.0f;
	if (point[3] != 0.0f) {
		wDivision = (1 / point[3]);
	}
	float newX = point[0] * wDivision;
	newX = ((newX + 1) * viewportWidth) / 2.0f;
	float newY = point[1] * wDivision;
	newY = ((newY + 1) * viewportHeight) / 2.0f;
	float newZ = point[2] * wDivision;
	//float newW = point[3] / wDivision;
	return glm::vec3(newX, newY, newZ);
}

void Renderer::drawLine(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& color) {
	int x1 = (int)point1[0], y1 = (int)point1[1];
	int x2 = (int)point2[0], y2 = (int)point2[1];
	float z1 = point1[2], z2 = point2[2];
	if (std::abs(y2 - y1) < std::abs(x2 - x1)) {
		if (x1 > x2) {
			plotLineLow(x2, y2, z2, x1, y1, z1, color);
		}
		else {
			plotLineLow(x1, y1, z1, x2, y2, z2, color);
		}
	}
	else {
		if (y1 > y2) {
			plotLineHigh(x2, y2, z2, x1, y1, z1, color);
		}
		else {
			plotLineHigh(x1, y1, z1, x2, y2, z2, color);
		}
	}
}

void Renderer::colorTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color, const GUIStore& store, const Face& face, const glm::mat4x4& completeTransform, const Scene& scene, const MeshModel& model, const int& index, std::map<int, glm::vec3>& normalsMap)
{
	//steps:
	/*
	1. Find bounding box for the triangle defined by (p1,p2,p3)
	2. For each pixel in Bounding Box
		2.1. if pixel is outside triangle, do nothing
		2.2. else color pixel
	*/
	int minX = getMin(p1.x, p2.x, p3.x);
	int maxX = getMax(p1.x, p2.x, p3.x);
	int minY = getMin(p1.y, p2.y, p3.y);
	int maxY = getMax(p1.y, p2.y, p3.y);
	int midX = ((float)(minX + maxX)) / 2.0f;
	int midY = ((float)(minY + maxY)) / 2.0f;
	for (int x = minX; x <= maxX; ++x) {
		colorYsInTriangle(x, minY, maxY, midX, midY, p1, p2, p3, color, store, face, completeTransform, scene, model, index, normalsMap);
	}
}


void Renderer::drawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color)
{
	drawLine(p1, p2, color);
	drawLine(p1, p3, color);
	drawLine(p2, p3, color);
}


void Renderer::drawNormalsPerFace(
	const Face& face,
	const glm::vec4& originalPoint1, const glm::vec2& pixelPoint1,
	const glm::vec4& originalPoint2, const glm::vec2& pixelPoint2,
	const glm::vec4& originalPoint3, const glm::vec2& pixelPoint3,
	const float normalLength,
	const glm::mat4x4& transform,
	const glm::vec3& color)
{

	glm::vec4 newPoint = (originalPoint1 + originalPoint2 + originalPoint3) / 3.0f;
	glm::vec3 pixelConcentratedPoint = translatePointIndicesToPixels(newPoint, transform);

	glm::vec3 _normalPoint = face.getFaceNormal();
	glm::vec4 normalPoint = glm::vec4(_normalPoint.x, _normalPoint.y, _normalPoint.z, 0.0f);

	glm::vec4 normal = normalLength * normalPoint + newPoint;

	glm::vec3 pixelNormal = translatePointIndicesToPixels(normal, transform);

	drawLine(pixelConcentratedPoint, pixelNormal, color);
}

void Renderer::drawNormalsPerVertex(
	const Face& face,
	const std::vector<glm::vec3>& normals,
	const glm::vec4& originalPoint1, const glm::vec3& pixelPoint1,
	const glm::vec4& originalPoint2, const glm::vec3& pixelPoint2,
	const glm::vec4& originalPoint3, const glm::vec3& pixelPoint3,
	const float normalLength,
	const glm::mat4x4& transform,
	const glm::vec3& color)
{
	int normalIndex1 = face.GetNormalIndex(0);
	float normalX1 = normals[normalIndex1].x, normalY1 = normals[normalIndex1].y, normalZ1 = normals[normalIndex1].z;
	int normalIndex2 = face.GetNormalIndex(1);
	float normalX2 = normals[normalIndex2].x, normalY2 = normals[normalIndex2].y, normalZ2 = normals[normalIndex2].z;
	int normalIndex3 = face.GetNormalIndex(2);
	float normalX3 = normals[normalIndex3].x, normalY3 = normals[normalIndex3].y, normalZ3 = normals[normalIndex3].z;

	glm::vec4 normal1 = normalLength * glm::vec4(normalX1, normalY1, normalZ1, 0.0f) + originalPoint1;
	glm::vec4 normal2 = normalLength * glm::vec4(normalX2, normalY2, normalZ2, 0.0f) + originalPoint2;
	glm::vec4 normal3 = normalLength * glm::vec4(normalX3, normalY3, normalZ3, 0.0f) + originalPoint3;

	glm::vec3 pixelNormal1 = translatePointIndicesToPixels(normal1, transform);
	glm::vec3 pixelNormal2 = translatePointIndicesToPixels(normal2, transform);
	glm::vec3 pixelNormal3 = translatePointIndicesToPixels(normal3, transform);

	drawLine(pixelPoint1, pixelNormal1, color);
	drawLine(pixelPoint2, pixelNormal2, color);
	drawLine(pixelPoint3, pixelNormal3, color);
}



void Renderer::handleFaceNormalsDrawing(Utils::Normals normalType, const GUIStore& store, const Face& face, std::vector<glm::vec3>& normalsPerPoint, const glm::vec4& originalPoint1, const glm::vec3& pixelPoint1, const glm::vec4& originalPoint2, const glm::vec3& pixelPoint2, const glm::vec4& originalPoint3, const glm::vec3& pixelPoint3, const glm::mat4x4& transform, int modelGUIIndex, const glm::vec3& colorPerFaceNormal, const glm::vec3& colorPerVertexNormal)
{
	switch (normalType) {
	case Utils::Normals::PerFACE:
	{
		float len = store.getModelNormalLength(modelGUIIndex);
		drawNormalsPerFace(face, originalPoint1, pixelPoint1, originalPoint2, pixelPoint2, originalPoint3, pixelPoint3, len, transform, colorPerFaceNormal);
	}
	break;
	case Utils::Normals::PerVERTEX:
	{
		if (face.hasNormals() == true) {
			float len = store.getModelNormalLength(modelGUIIndex);
			drawNormalsPerVertex(face, normalsPerPoint, originalPoint1, pixelPoint1, originalPoint2, pixelPoint2, originalPoint3, pixelPoint3, len, transform, colorPerVertexNormal);
		}
	}
	break;
	case Utils::Normals::NONE:
		//do nothing.
		break;
	default:
		//do nothing.
		break;
	}
}

void Renderer::handleBoundingBoxDrawing(const GUIStore& store, int modelGUIIndex, float x1, float y1, float z1, float x2, float y2, float z2, const glm::mat4x4& transform, const glm::vec3& color)
{
	if (store.isModelBoundingBoxOn(modelGUIIndex) == false) return;
	float _z1 = 0.0f;
	float _z2 = 0.0f;
	float _z3 = 0.0f;
	float _z4 = 0.0f;
	float _z5 = 0.0f;
	float _z6 = 0.0f;
	float _z7 = 0.0f;
	float _z8 = 0.0f;
	//right side of the cube:
	glm::vec3 point1 = translatePointIndicesToPixels(glm::vec4(x1, y1, z1, 1.0f), transform);
	glm::vec3 point2 = translatePointIndicesToPixels(glm::vec4(x1, y1, z2, 1.0f), transform);
	glm::vec3 point3 = translatePointIndicesToPixels(glm::vec4(x1, y2, z1, 1.0f), transform);
	glm::vec3 point4 = translatePointIndicesToPixels(glm::vec4(x1, y2, z2, 1.0f), transform);

	//left side of the cube:
	glm::vec3 point5 = translatePointIndicesToPixels(glm::vec4(x2, y2, z1, 1.0f), transform);
	glm::vec3 point6 = translatePointIndicesToPixels(glm::vec4(x2, y2, z2, 1.0f), transform);
	glm::vec3 point7 = translatePointIndicesToPixels(glm::vec4(x2, y1, z2, 1.0f), transform);
	glm::vec3 point8 = translatePointIndicesToPixels(glm::vec4(x2, y1, z1, 1.0f), transform);

	//the cube:
	drawLine(point1, point2, color);
	drawLine(point1, point3, color);
	drawLine(point1, point8, color);
	drawLine(point2, point7, color);
	drawLine(point2, point4, color);
	drawLine(point3, point4, color);
	drawLine(point3, point5, color);
	drawLine(point4, point6, color);
	drawLine(point5, point8, color);
	drawLine(point5, point6, color);
	drawLine(point6, point7, color);
	drawLine(point7, point8, color);
}

glm::vec3 Renderer::getLightCenter(const Light& light, const glm::mat4x4& transform) const {
	glm::vec3 min = light.getMinBoundingBoxVec();
	glm::vec3 max = light.getMaxBoundingBoxVec();
	glm::vec3 avg = (min + max)*0.5f;
	glm::vec4 point = glm::vec4(avg, 1.0f);
	return translatePointIndicesToPixels(point, transform);
}

void Renderer::drawMeshModels(const Scene& scene, const GUIStore& store) {
	glm::vec3 redColor(1.0f, 0.0f, 0.0f);
	glm::vec3 blueColor(0.0f, 1.0f, 1.0f);
	glm::vec3 greenColor(0.0f, 1.0f, 0.0f);
	glm::vec3 yellowColor(1.0f, 1.0f, 0.0f);
	glm::vec3 pinkColor(1.0f, 20.0f / 255.0f, 147.0f / 255.0f);
	Camera activeCam = scene.getActiveCamera();
	glm::vec3 cameraEye = activeCam.getEyeVector();
	glm::mat4x4 camTransformation = activeCam.getProjectionTransformation() * activeCam.getViewTransformationInverse();
	int index = -1;
	std::vector<std::shared_ptr<MeshModel>> models = scene.getSceneModels();
	for each (std::shared_ptr<MeshModel> model in models)
	{
		// Activate the 'colorShader' program (vertex and fragment shaders)
		colorShader.use();

		// Set the uniform variables
		colorShader.setUniform("model", model->GetWorldTransformation());
		colorShader.setUniform("view", activeCam.getViewTransformationInverse());
		colorShader.setUniform("projection", activeCam.getProjectionTransformation());
		colorShader.setUniform("material.textureMap", 0);

		// Set 'texture1' as the active texture at slot #0
		//texture1.bind(0);

		// Drag our model's faces (triangles) in fill mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(model->GetVAO());
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)model->getModelVertices().size());
		glBindVertexArray(0);

		// Unset 'texture1' as the active texture at slot #0
		//texture1.unbind(0);

		colorShader.setUniform("color", glm::vec3(1.0f, 1.0f, 1.0f));

		// Drag our model's faces (triangles) in line mode (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(model->GetVAO());
		glDrawArrays(GL_TRIANGLES, 0, model->getModelVertices().size());
		glBindVertexArray(0);

		{
			++index;
			std::map<int, glm::vec3> normalsMap = model->getNormalForVertices();
			glm::vec3 modelColor = store.getModelColor(index);
			glm::vec3 triangleColor = generateColorVariation(modelColor, 30.0f);
			std::vector<glm::vec2> textures = model->getTextures();
			std::vector<glm::vec3> normals = model->getNormals();
			std::vector<glm::vec3> vertices = model->getVertices();
			std::vector<Face> faces = model->getFaces();
			glm::mat4x4 modelTransform = model->GetWorldTransformation();
			glm::mat4x4 completeTransform = camTransformation * modelTransform;
			Utils::Normals whichNormal = store.getModelNormalStatus(index);
			float maxX = 0, maxY = 0, maxZ = 0, minX = 0, minY = 0, minZ = 0;
			if (faces.size() > 0) {
				int i = faces[0].GetVertexIndex(0);
				maxX = vertices[i].x; minX = vertices[i].x;
				maxY = vertices[i].y; minY = vertices[i].y;
				maxZ = vertices[i].z; minZ = vertices[i].z;
			}
			for each (Face face in faces)
			{
				int v1 = face.GetVertexIndex(0);
				float x1 = vertices[v1].x, y1 = vertices[v1].y, z1 = vertices[v1].z;
				int v2 = face.GetVertexIndex(1);
				float x2 = vertices[v2].x, y2 = vertices[v2].y, z2 = vertices[v2].z;
				int v3 = face.GetVertexIndex(2);
				float x3 = vertices[v3].x, y3 = vertices[v3].y, z3 = vertices[v3].z;


				glm::vec4 w1 = glm::vec4(x1, y1, z1, 1.0f);
				glm::vec4 w2 = glm::vec4(x2, y2, z2, 1.0f);
				glm::vec4 w3 = glm::vec4(x3, y3, z3, 1.0f);

				glm::vec3 p1 = translatePointIndicesToPixels(w1, completeTransform);
				glm::vec3 p2 = translatePointIndicesToPixels(w2, completeTransform);
				glm::vec3 p3 = translatePointIndicesToPixels(w3, completeTransform);
				//colorTriangle(p1, p2, p3, modelColor, store, face, completeTransform, scene, *model, index,normalsMap);
				/* --------------------------------------------------------------------------------------------- */
				/* Normal calculations */
				handleFaceNormalsDrawing(whichNormal, store, face, normals, w1, p1, w2, p2, w3, p3, completeTransform, index, greenColor, blueColor);

				/* Max and Min Points in the mesh */
				maxX = getMax(x1, x2, x3, maxX); minX = getMin(x1, x2, x3, minX);
				maxY = getMax(y1, y2, y3, maxY); minY = getMin(y1, y2, y3, minY);
				maxZ = getMax(z1, z2, z3, maxZ); minZ = getMin(z1, z2, z3, minZ);

			}

			handleBoundingBoxDrawing(store, index, maxX, maxY, maxZ, minX, minY, minZ, completeTransform, pinkColor);
			model->setMaxBoundingBoxVec(completeTransform*glm::vec4(maxX, maxY, maxZ, 1.0f));
			model->setMinBoundingBoxVec(completeTransform*glm::vec4(minX, minY, minZ, 1.0f));
		}
	}
}

void Renderer::drawCameraModels(const Scene& scene)
{
	glm::vec3 blackColor(0.0f, 0.0f, 0.0f);
	Camera activeCam = scene.getActiveCamera();
	std::vector<Camera> camList = scene.GetCameras();
	MeshModel model = (Utils::LoadMeshModel(".\\..\\..\\project-the-hurricane-troupe\\Data\\camera.obj"));
	for each(Camera cam in camList) {
		if (scene.GetActiveCameraIndex() != cam.getIndex()) {
			glm::vec4 position = cam.getEyeVector();
			model.setPosition(position.x, position.y, position.z);
			std::vector<glm::vec2> textures = model.getTextures();
			std::vector<glm::vec3> normals = model.getNormals();
			std::vector<glm::vec3> vertices = model.getVertices();
			std::vector<Face> faces = model.getFaces();
			for each (Face face in faces)
			{
				int v1 = face.GetVertexIndex(0);
				float x1 = vertices[v1].x, y1 = vertices[v1].y, z1 = vertices[v1].z;
				int v2 = face.GetVertexIndex(1);
				float x2 = vertices[v2].x, y2 = vertices[v2].y, z2 = vertices[v2].z;
				int v3 = face.GetVertexIndex(2);
				float x3 = vertices[v3].x, y3 = vertices[v3].y, z3 = vertices[v3].z;

				glm::vec4 w1 = glm::vec4(x1, y1, z1, 1.0f);
				glm::vec4 w2 = glm::vec4(x2, y2, z2, 1.0f);
				glm::vec4 w3 = glm::vec4(x3, y3, z3, 1.0f);

				glm::mat4x4 camTransformation = activeCam.getProjectionTransformation() * activeCam.getViewTransformationInverse();
				glm::mat4x4 modelTransform = model.GetWorldTransformation();
				glm::mat4x4 completeTransform = camTransformation * modelTransform;

				glm::vec3 p1 = translatePointIndicesToPixels(w1, completeTransform);
				glm::vec3 p2 = translatePointIndicesToPixels(w2, completeTransform);
				glm::vec3 p3 = translatePointIndicesToPixels(w3, completeTransform);

				drawTriangle(p1, p2, p3, blackColor);
			}
		}
	}
}

void Renderer::drawLightModels(const Scene & scene, const GUIStore& store)
{
	Camera activeCam = scene.getActiveCamera();
	glm::mat4x4 camTransformation = activeCam.getProjectionTransformation() * activeCam.getViewTransformationInverse();
	std::vector<std::shared_ptr<Light>> lights = scene.getLights();
	int index = -1;
	for each (std::shared_ptr<Light> light in lights) {
		++index;
		if (light->getLightType() == LightType::PARALLEL) continue;
		std::map<int, glm::vec3>& normalsMap = light->getNormalForVertices();
		glm::vec3 color = light->getLightColor();
		std::vector<glm::vec2> textures = light->getTextures();
		std::vector<glm::vec3> normals = light->getNormals();
		std::vector<glm::vec3> vertices = light->getVertices();
		std::vector<Face> faces = light->getFaces();
		glm::mat4x4 modelTransform = light->GetWorldTransformation();
		glm::mat4x4 completeTransform = camTransformation * modelTransform;
		float maxX = 0, maxY = 0, maxZ = 0, minX = 0, minY = 0, minZ = 0;
		if (faces.size() > 0) {
			int i = faces[0].GetVertexIndex(0);
			maxX = vertices[i].x; minX = vertices[i].x;
			maxY = vertices[i].y; minY = vertices[i].y;
			maxZ = vertices[i].z; minZ = vertices[i].z;
		}
		for each (Face face in faces)
		{
			int v1 = face.GetVertexIndex(0);
			float x1 = vertices[v1].x, y1 = vertices[v1].y, z1 = vertices[v1].z;
			int v2 = face.GetVertexIndex(1);
			float x2 = vertices[v2].x, y2 = vertices[v2].y, z2 = vertices[v2].z;
			int v3 = face.GetVertexIndex(2);
			float x3 = vertices[v3].x, y3 = vertices[v3].y, z3 = vertices[v3].z;

			glm::vec4 w1 = glm::vec4(x1, y1, z1, 1.0f);
			glm::vec4 w2 = glm::vec4(x2, y2, z2, 1.0f);
			glm::vec4 w3 = glm::vec4(x3, y3, z3, 1.0f);


			glm::vec3 p1 = translatePointIndicesToPixels(w1, completeTransform);
			glm::vec3 p2 = translatePointIndicesToPixels(w2, completeTransform);
			glm::vec3 p3 = translatePointIndicesToPixels(w3, completeTransform);


			colorTriangle(p1, p2, p3, color, store, face, completeTransform, scene, *light, -1, normalsMap);

			maxX = getMax(x1, x2, x3, maxX); minX = getMin(x1, x2, x3, minX);
			maxY = getMax(y1, y2, y3, maxY); minY = getMin(y1, y2, y3, minY);
			maxZ = getMax(z1, z2, z3, maxZ); minZ = getMin(z1, z2, z3, minZ);

		}

		light->setMaxBoundingBoxVec(completeTransform*glm::vec4(maxX, maxY, maxZ, 1.0f));
		light->setMinBoundingBoxVec(completeTransform*glm::vec4(minX, minY, minZ, 1.0f));
	}
}


void Renderer::updatePixelValues()
{
	std::vector<std::pair<int, int>> coordinates = zBuffer.getCoordinates();
	for each (std::pair<int, int> xyPair in coordinates)
	{
		int x = xyPair.first;
		int y = xyPair.second;
		glm::vec3 color = zBuffer.getPixelColor(x, y);
		putPixel(x, y, color);
	}
}
