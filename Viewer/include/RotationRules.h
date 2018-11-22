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

	/*converts radians to degrees*/
	float radiansToDegree(float radian);
	/*converts degrees to radians*/
	float degreeToRadians(float degree);
	/*updates the Axis axis to rotate with angle degree */
	void updateRotation(Axis axis, float angle, AngleUnits unit);
	/*updates angle value, assumes that it is passed a degree value */
	void updateDegreeCoordinates(Axis axis, float degree);
public:
	RotationRules(float xAngle = 0.0f, float yAngle = 0.0f, float zAngle = 0.0f, AngleUnits unit = AngleUnits::DEGREES);
	void setRotation(Axis axis, float angle, AngleUnits unit = AngleUnits::DEGREES);
	/*sets rotation for each axis, when an angle is set to nullptr, it keeps it unchanged. */
	void setRotationAll(float* xAngle, float* yAngle, float* zAngle, AngleUnits unit);
	/*removes model's rotation on the axis axis.*/
	void resetRotation(Axis axis);
	/*removes model's rotation completely.*/
	void resetRotation();
	float getAngleX(AngleUnits unit) const;
	float getAngleY(AngleUnits unit) const;
	float getAngleZ(AngleUnits unit) const;
	~RotationRules();
};

