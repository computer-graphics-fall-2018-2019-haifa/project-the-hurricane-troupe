#include "AxisRotation.h"

float PairOfAxisAngle::degreeToRadians(float degree)
{
	return (degree * ((float)(PI))) / 180.0f;
}

PairOfAxisAngle::PairOfAxisAngle(AxisType _axis, float _angleInDegrees)
{
	axis = _axis;
	angle = degreeToRadians(_angleInDegrees);
}

float PairOfAxisAngle::getAngle() const { return angle; }

AxisType PairOfAxisAngle::getAxis() const { return axis; }
