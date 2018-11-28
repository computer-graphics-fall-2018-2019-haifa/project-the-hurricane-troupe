#include "RotationMatrix.h"



void RotationMatrix::updateRotateTransformation()
{
	if (isTransformUpdated == true) return;
	rotationTransformation *= zRotation * yRotation * xRotation;
	isTransformUpdated = true;
}

RotationMatrix::RotationMatrix(float xAngle, float yAngle, float zAngle) :
	rotationTransformation(glm::mat4x4({
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
		}))
{
	this->xAngle = xAngle; setXRotation(xAngle);
	this->yAngle = yAngle; setYRotation(yAngle);
	this->zAngle = zAngle; setZRotation(zAngle);
	isTransformUpdated = false;
	updateRotateTransformation();
}

void RotationMatrix::setXRotation(const float xAngle)
{
	this->xAngle = xAngle;
	float cos = glm::cos(this->xAngle);
	float sin = glm::sin(this->xAngle);
	xRotation = glm::transpose(glm::mat4x4({
		1,		0,		0,		0,
		0,		cos,	-sin,	0,
		0,		sin,	cos,	0,
		0,		0,		0,		1
		}));
	isTransformUpdated = false;
}

void RotationMatrix::setYRotation(const float yAngle)
{
	this->yAngle = yAngle;
	float cos = glm::cos(this->yAngle);
	float sin = glm::sin(this->yAngle);
	yRotation = glm::transpose(glm::mat4x4({
		cos,	0,		sin,	0,
		0,		1,		0,		0,
		-sin,	0,		cos,	0,
		0,		0,		0,		1
		}));
	isTransformUpdated = false;
}

void RotationMatrix::setZRotation(const float zAngle)
{
	this->zAngle = zAngle;
	float cos = glm::cos(this->zAngle);
	float sin = glm::sin(this->zAngle);
	zRotation = glm::transpose(glm::mat4x4({
		cos,	-sin,	0,		0,
		sin,	cos,	0,		0,
		0,		0,		1,		0,
		0,		0,		0,		1
		}));
	isTransformUpdated = false;
}

void RotationMatrix::resetXRotation() {
	setXRotation(0.0f);
}

void RotationMatrix::resetYRotation() {
	setYRotation(0.0f);
}

void RotationMatrix::resetZRotation() {
	setZRotation(0.0f);
}


void RotationMatrix::resetToOrginalRotation() {
	rotationTransformation = glm::mat4x4({
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
		});
}

glm::mat4x4 RotationMatrix::getTransform() {
	if (isTransformUpdated == false) updateRotateTransformation();
	return rotationTransformation;
}


RotationMatrix::~RotationMatrix() { }
