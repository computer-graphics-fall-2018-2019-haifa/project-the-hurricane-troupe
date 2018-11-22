#pragma once
#include "AxisTypes.h"

class PairOfAxisAngle {
private:
	const double PI = 3.14159265359;
	AxisType axis;
	float angle;

	float degreeToRadians(float degree);
public:
	PairOfAxisAngle(AxisType _axis, float _angleInDegrees);
	float getAngle() const;
	AxisType getAxis() const;
};

