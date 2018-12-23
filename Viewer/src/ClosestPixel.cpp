#include "..\include\ClosestPixel.h"

PixelPoint::PixelPoint(int x, int y) : _x(x), _y(y)
{
}

bool ZColor::isColorLegal(const glm::vec3 & color) const
{
	return (0.0f <= color.x && color.x <= 1.0f) && (0.0f <= color.y && color.y <= 1.0f) && (0.0f <= color.z && color.z <= 1.0f);
}

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
	if (isColorLegal(color) == false) return;
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

ClosestPixel::ClosestPixel()
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
	if (closestPixelColor.find(key) == closestPixelColor.end()) { //if it doesnt exist...
		closestPixelColor[key] = ZColor(z, color);
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

glm::vec3 ClosestPixel::getPixelColor(int x, int y)
{
	size_t xHashKey = xHashes._Do_hash(x);
	size_t yHashKey = yHashes._Do_hash(y);
	std::pair<size_t, size_t> key = std::make_pair(xHashKey, yHashKey);
	if (closestPixelColor.find(key) == closestPixelColor.end()) return ERROR_COLOR;
	return closestPixelColor.at(key).getColor();
}

void ClosestPixel::resetColor()
{
	closestPixelColor.clear();
}
