#pragma once
#include <glm\glm.hpp>

/*
***** IMPORTANT: THIS IS NOT IN ANY WAY TESTED! Make sure it works! (matrix gotten from lectures + wikipedia)
*/
class RotationMatrix
{
private:
	float xAngle, yAngle, zAngle;
	glm::mat4x4 xRotation;
	glm::mat4x4 yRotation;
	glm::mat4x4 zRotation;
	glm::mat4x4 rotationTransformation;

	void updateRotateTransformation();
public:
	RotationMatrix(float xAngle = 0.0f, float yAngle = 0.0f, float zAngle = 0.0f);
	void setXRotation(const float xAngle);
	void setYRotation(const float yAngle);
	void setZRotation(const float zAngle);
	void resetXRotation();
	void resetYRotation();
	void resetZRotation();
	void resetToOrginalRotation();
	glm::mat4x4 getRotationMatrix() const;
	~RotationMatrix();
};

