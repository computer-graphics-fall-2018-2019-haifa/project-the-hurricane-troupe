#pragma once
#include <map>
#include <functional>
#include <glm/glm.hpp>

class ZColor {
private:
	float _z;
	glm::vec3 _color;
	bool isColorLegal(const glm::vec3& color) const;
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
	
	const float INVALID_Z = -100000000.0f;
public:
	const glm::vec3 ERROR_COLOR = glm::vec3(-1.0f, -1.0f, -1.0f);
	ClosestPixel();

	void tryToSetColor(const glm::vec3& point, const glm::vec3& color);
	void tryToSetColor(int x, int y, float z, const glm::vec3& color);
	glm::vec3 getPixelColor(const glm::vec2& point);
	glm::vec3 getPixelColor(int x, int y);
	void resetColor();
};


