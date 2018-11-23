#pragma once
#include <glm\glm.hpp>

/*
***** IMPORTANT: THIS IS NOT IN ANY WAY TESTED! Make sure it works! (matrix gotten from lectures + wikipedia)
*/
/*
* Documentation: This class is meant to make creation of rotate transformations more accessible.
*/
class RotationMatrix
{
private:
	float xAngle, yAngle, zAngle; //These variables are only to help during debug (we can check the expected angles, and they do not serve any other purpose at least not yet).
	glm::mat4x4 xRotation;
	glm::mat4x4 yRotation;
	glm::mat4x4 zRotation;
	glm::mat4x4 rotationTransformation;
	bool isTransformUpdated;

	void updateRotateTransformation();
public:
	/*constructor with default parameters of 0 (no rotation at all)*/
	RotationMatrix(float xAngle = 0.0f, float yAngle = 0.0f, float zAngle = 0.0f);
	/* set the rotation around the xAxis with angle xAngle*/
	void setXRotation(const float xAngle);
	/* set the rotation around the yAxis with angle yAngle*/
	void setYRotation(const float yAngle);
	/* set the rotation around the zAxis with angle zAngle*/
	void setZRotation(const float zAngle);
	/* ignore rotation over xAxis*/
	void resetXRotation();
	/* ignore rotation over yAxis*/
	void resetYRotation();
	/* ignore rotation over zAxis*/
	void resetZRotation();
	/* remove all rotations on object.*/
	void resetToOrginalRotation();
	/* returns the rotationTransformation generated from the rotations over the X,Y,Z rotations specified.*/
	glm::mat4x4 getTransform();
	~RotationMatrix();
};

