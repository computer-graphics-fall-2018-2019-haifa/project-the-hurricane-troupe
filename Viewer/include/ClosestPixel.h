#pragma once
#include <map>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include "Utils.h"

class ZColor {
private:
	float _z;
	glm::vec3 _color;
public:
	ZColor();
	ZColor(float z, const glm::vec3& color);
	void setZAndColor(float z, const glm::vec3& color);
	void setColor(const glm::vec3& color);
	float getZ() const;
	glm::vec3 getColor() const;
};


class ClosestPixel {

private:
	std::hash<int> xHashes;
	std::hash<int> yHashes;
	std::map<std::pair<size_t, size_t>, ZColor> closestPixelColor;
	std::vector<std::pair<int, int>> xyPixelCoordinatesAffected;
	bool antiAliasing;

	const float INVALID_Z = -100000000.0f;

	glm::vec3 getExactPixelColor(int x, int y) const;
public:
	const glm::vec3 bgColor;
	ClosestPixel(const glm::vec3& backgroundColor);

	void tryToSetColor(const glm::vec3& point, const glm::vec3& color);
	void tryToSetColor(int x, int y, float z, const glm::vec3& color);
	glm::vec3 getPixelColor(const glm::vec2& point);
	glm::vec3 getPixelColor(int x, int y);
	std::vector<std::pair<int, int>> getCoordinates() const;
	void setAntiAliasing(bool status);
	void resetColor();
};


