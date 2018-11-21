#include "RotationMatrix.h"



void RotationMatrix::updateRotateTransformation()
{
	setXRotation(this->xAngle);
	setYRotation(this->yAngle);
	setZRotation(this->zAngle);
	rotationTransformation = zRotation * yRotation * xRotation;
}

RotationMatrix::RotationMatrix(float xAngle, float yAngle, float zAngle)
{
	this->xAngle = xAngle;
	this->yAngle = yAngle;
	this->zAngle = zAngle;
	updateRotateTransformation();
}

void RotationMatrix::setXRotation(const float xAngle)
{
	this->xAngle = xAngle;
	float cos = glm::cos(this->xAngle);
	float sin = glm::sin(this->xAngle);
	xRotation = glm::mat4x4({
		1,		0,		0,		0,
		0,		cos,	-sin,	0,
		0,		sin,	cos,	0,
		0,		0,		0,		1
		});
}

void RotationMatrix::setYRotation(const float yAngle)
{
	this->yAngle = yAngle;
	float cos = glm::cos(this->yAngle);
	float sin = glm::sin(this->yAngle);
	yRotation = glm::mat4x4({
		cos,	0,		sin,	0,
		0,		1,		0,		0,
		-sin,	0,		cos,	0,
		0,		0,		0,		1
		});
}

void RotationMatrix::setZRotation(const float zAngle)
{
	this->zAngle = zAngle;
	float cos = glm::cos(this->zAngle);
	float sin = glm::sin(this->zAngle);
	zRotation = glm::mat4x4({
		cos,	-sin,	0,		0,
		sin,	cos,	0,		0,
		0,		0,		1,		0,
		0,		0,		0,		1
		});
}

void RotationMatrix::resetXRotation() {
	this->xAngle = 0.0f;
	updateRotateTransformation();
}

void RotationMatrix::resetYRotation() {
	this->yAngle = 0.0f;
	updateRotateTransformation();
}

void RotationMatrix::resetZRotation() {
	this->zAngle = 0.0f;
	updateRotateTransformation();
}


void RotationMatrix::resetToOrginalRotation() {
	resetZRotation();
	resetYRotation();
	resetXRotation();
}

glm::mat4x4 RotationMatrix::getRotatateTransform() const {
	return rotationTransformation;
}


RotationMatrix::~RotationMatrix() { }
