#pragma once
#include "AxisTypes.h"
#include <glm\glm.hpp>

enum class AngleUnits
{
	DEGREES,
	RADIANS
};

//TODO: Should add order virtue to Rotation (z then y then x, or y then x then z, etc..)
class RotationRules {
private:
	const double PI = 3.14159265359;
	float _xAngleDegrees, _xAngleRadians;
	float _yAngleDegrees, _yAngleRadians;
	float _zAngleDegrees, _zAngleRadians;
	//TODO: array that describes the order 

	float radiansToDegree(float radian);
	float degreeToRadians(float degree);
	void updateRotation(Axis axis, float angle, AngleUnits unit);
	void updateDegreeCoordinates(Axis axis, float angle);
public:
	RotationRules(float xAngle = 0.0f, float yAngle = 0.0f, float zAngle = 0.0f, AngleUnits unit = AngleUnits::DEGREES);
	void setRotation(Axis axis, float angle, AngleUnits unit = AngleUnits::DEGREES);
	void setRotationAll(float* xAngle, float* yAngle, float* zAngle, AngleUnits unit);
	void resetRotation(Axis axis);
	void resetRotation();
	float getAngleX(AngleUnits unit) const;
	float getAngleY(AngleUnits unit) const;
	float getAngleZ(AngleUnits unit) const;
	~RotationRules();
};

