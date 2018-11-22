#include "RotationRules.h"

float RotationRules::radiansToDegree(float radian)
{
	return (radian*((float)PI))/180.0f;
}

float RotationRules::degreeToRadians(float degree)
{
	return (degree * ((float)(PI))) / 180.0f;
}

void RotationRules::updateDegreeCoordinates(Axis axis, float degree) {
	float radian = degreeToRadians(degree);
	switch (axis) {
	case Axis::XAXIS: 
		_xAngleDegrees = degree;
		_xAngleRadians = radian;
		break;
	case Axis::YAXIS:
		_yAngleDegrees = degree;
		_yAngleRadians = radian;
		break;
	case Axis::ZAXIS:
		_zAngleDegrees = degree;
		_zAngleRadians = radian;
		break;
	}
}

void RotationRules::updateRotation(Axis axis, float angle, AngleUnits unit)
{
	switch (unit) {
	case AngleUnits::RADIANS:
		updateDegreeCoordinates(axis, radiansToDegree(angle));
		break;
	case AngleUnits::DEGREES:
		updateDegreeCoordinates(axis, angle);
		break;
	default:
		// Notice: This should never happen!
		// TODO: Should throw exception! 
		(_xAngleDegrees = (_yAngleDegrees = (_zAngleDegrees = (_xAngleRadians = (_yAngleRadians = (_zAngleRadians = 0.0f))))));
		break;
	}
}

RotationRules::RotationRules(float xAngle, float yAngle, float zAngle, AngleUnits unit)
{
	updateRotation(Axis::XAXIS, xAngle, unit);
	updateRotation(Axis::YAXIS, yAngle, unit);
	updateRotation(Axis::ZAXIS, zAngle, unit);
}

void RotationRules::setRotation(Axis axis, float angle, AngleUnits unit)
{
	updateRotation(axis, angle, unit);
}

void RotationRules::setRotationAll(float * xAngle, float * yAngle, float * zAngle, AngleUnits unit)
{
	if (xAngle != nullptr) {
		updateRotation(Axis::XAXIS, *xAngle, unit);
	}
	if (yAngle != nullptr) {
		updateRotation(Axis::YAXIS, *yAngle, unit);
	}
	if (zAngle != nullptr) {
		updateRotation(Axis::ZAXIS, *zAngle, unit);
	}

}

void RotationRules::resetRotation(Axis axis)
{
	updateRotation(axis, 0.0f, AngleUnits::DEGREES);
}

void RotationRules::resetRotation()
{
	//gives all 0.0f value
	(_xAngleDegrees = (_yAngleDegrees = (_zAngleDegrees = (_xAngleRadians = (_yAngleRadians = (_zAngleRadians = 0.0f))))));
}

float RotationRules::getAngleX(AngleUnits unit) const
{
	if (unit == AngleUnits::DEGREES) return _xAngleDegrees;
	return _xAngleRadians;
}

float RotationRules::getAngleY(AngleUnits unit) const
{
	if (unit == AngleUnits::DEGREES) return _yAngleDegrees;
	return _yAngleRadians;
}

float RotationRules::getAngleZ(AngleUnits unit) const
{
	if (unit == AngleUnits::DEGREES) return _zAngleDegrees;
	return _zAngleRadians;
}


RotationRules::~RotationRules() { }
