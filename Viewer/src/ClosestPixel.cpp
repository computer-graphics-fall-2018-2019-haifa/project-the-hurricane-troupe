#include "..\include\ClosestPixel.h"



ZColor::ZColor(): _z(0.0f), _color()
{
}

ZColor::ZColor(float z, const glm::vec3 & color) : _z(z), _color(color)
{
}

void ZColor::setZAndColor(float z, const glm::vec3 & color)
{
	_z = z;
	_color = color;
}

void ZColor::setColor(const glm::vec3 & color)
{
	if (Utils::isColorLegal(color) == false) return;
	_color = color;
}

float ZColor::getZ() const
{
	return _z;
}

glm::vec3 ZColor::getColor() const
{
	return _color;
}

ClosestPixel::ClosestPixel(const glm::vec3& backgroundColor): bgColor(backgroundColor), antiAliasing(false)
{
}

void ClosestPixel::tryToSetColor(const glm::vec3 & point, const glm::vec3 & color)
{
	int x = point.x;
	int y = point.y;
	float z = point.z;
	tryToSetColor(x, y, z, color);
}

void ClosestPixel::tryToSetColor(int x, int y, float z, const glm::vec3& color)
{
	size_t xHashKey = xHashes._Do_hash(x);
	size_t yHashKey = yHashes._Do_hash(y);
	std::pair<size_t, size_t> key = std::make_pair(xHashKey, yHashKey);
	std::pair<int, int> xyCoordinates = std::make_pair(x, y);
	if (closestPixelColor.find(key) == closestPixelColor.end()) { //if it doesnt exist...
		closestPixelColor[key] = ZColor(z, color);
		xyPixelCoordinatesAffected.push_back(xyCoordinates);
		return;
	}
	if (z < closestPixelColor.at(key).getZ()) {
		closestPixelColor[key].setZAndColor(z, color);
	}
}

glm::vec3 ClosestPixel::getPixelColor(const glm::vec2 & point)
{
	return getPixelColor(point.x, point.y);
}

glm::vec3 ClosestPixel::getExactPixelColor(int x, int y) const {
	size_t xHashKey = xHashes._Do_hash(x);
	size_t yHashKey = yHashes._Do_hash(y);
	std::pair<size_t, size_t> key = std::make_pair(xHashKey, yHashKey);
	if (closestPixelColor.find(key) == closestPixelColor.end()) return bgColor;
	return closestPixelColor.at(key).getColor();
}

glm::vec3 ClosestPixel::getPixelColor(int x, int y)
{
	if (antiAliasing == false) { //no antiAliasing is needed
		return getExactPixelColor(x, y);
	}
	else { //antiAliasing is active
		int x1 = x, x2 = -1000, y1 = y, y2 = -1000;
		
		if (x1 % 2 == 0) x2 = x1 + 1;
		else x2 = x1 - 1;

		if (y1 % 2 == 0) y2 = y1 + 1;
		else y2 = y1 - 1;
		
		glm::vec3 finalColor = glm::vec3(0.0f, 0.0f, 0.0f);
		finalColor += getExactPixelColor(x1, y1);
		finalColor += getExactPixelColor(x1, y2);
		finalColor += getExactPixelColor(x2, y1);
		finalColor += getExactPixelColor(x2, y2);
		finalColor /= 4.0f;
		return finalColor;
	}

}

std::vector<std::pair<int, int>> ClosestPixel::getCoordinates() const
{
	return xyPixelCoordinatesAffected;
}

void ClosestPixel::setAntiAliasing(bool status)
{
	antiAliasing = status;
}

void ClosestPixel::resetColor()
{
	closestPixelColor.clear();
	xyPixelCoordinatesAffected.clear();
}
