#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//source: https://en.wikipedia.org/wiki/Graphics_pipeline#Rasterisation
Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	zoom(1.0),
	viewTransformation(Utils::IdentityMat()),
	viewTransformationInverse(Utils::IdentityMat()),
	perspectionProjectionTransformation(Utils::IdentityMat()),
	orthographicProjectionTransformation(Utils::IdentityMat()),
	_projLeft(-1.0f), _projRight(1.0f), _projBottom(-1.0f), _projTop(0.1f), _projNear(1.0), _projFar(1.0), _projFovy(0.0f), _projAspectRatio(0.0f)
{
	setOrthographicProjection(_projLeft, _projRight, _projBottom, _projTop, _projNear, _projFar);
	SetCameraLookAt(eye, at, up);
}


void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 z = normalize(eye - at);
	glm::vec3 x = normalize(glm::cross(up, z));
	glm::vec3 y = normalize(glm::cross(z, x));

	glm::mat4x4 c = glm::transpose(glm::mat4x4({
		x[0], x[1], x[2], 0,
		y[0], y[1], y[2], 0,
		z[0], z[1], z[2], 0,
		0, 0, 0, 1
		}
	));

	glm::mat4x4 translationMatrix = glm::transpose(glm::mat4x4(
	{	1	,	0	,	0	,	-eye[0],
		0	,	1	,	0	,	-eye[1],
		0	,	0	,	1	,	-eye[2],
		0	,	0	,	0	,	1 }));

	viewTransformation = c * translationMatrix;
	viewTransformationInverse = glm::transpose(c) * translationMatrix;
}

bool Camera::isOrthographicProjection() const
{
	return currentProjectionType == ProjectionType::ORTHOGRAPHIC;
}

bool Camera::isPerspectiveProjection() const
{
	return currentProjectionType == ProjectionType::PERSPECTIVE;
}

ProjectionType Camera::whichProjection() const
{
	return currentProjectionType;
}

void Camera::setActiveProjection(const ProjectionType& type)
{
	currentProjectionType = type;
}


void Camera::_SetPerspectiveProjection(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
	this->_projLeft = left;
	this->_projRight = right;
	this->_projBottom = bottom;
	this->_projTop = top;
	this->_projNear = near;
	this->_projFar = far;
	perspectionProjectionTransformation = glm::transpose(glm::mat4x4(
	{ (2.0 * near) / (right - left)		,	0								,		(right + left) / (right - left)		,	0,
		0								,	(2.0 * near) / (top - bottom)	,		 (top + bottom) / (top - bottom)	,	0,
		0								,	0								,		-1 * (far + near) / (far - near)	,	-1 * ((2 * far * near) / (far - near)),
		0								,	0								,	    -1									,	0
	}));
	setActiveProjection(ProjectionType::PERSPECTIVE);
}

void Camera::setPerspectiveProjection(const float fovy, const float aspectRatio, const float near, const float far, AngleUnits unit)
{
	float actualFovy = 0.0f;
	switch (unit) {
	case AngleUnits::RADIANS:
		actualFovy = fovy;
		break;
	case AngleUnits::DEGREES:
		actualFovy = (fovy * ((float)(3.14159265359))) / 180.0f; //to radians
		break;
	default:
		break;
	}
	double height = glm::abs(glm::tan(actualFovy / 2)*near);
	double width = aspectRatio * height;
	_SetPerspectiveProjection(-width, width, -height, height, near, far);
	this->_projFovy = fovy;
	this->_projAspectRatio = aspectRatio;
}

void Camera::setOrthographicProjection(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
	orthographicProjectionTransformation = glm::transpose(glm::mat4x4(
		{ 2.0 / (right - left)		,	0.0						,		0.0					,	-1.0*((right + left) / (right - left)),
		0.0						,	2.0 / (top - bottom)	,		0.0					,	-1.0*((top + bottom) / (top - bottom)),
		0.0						,	0.0						,		2.0 / (near - far)	,	-1.0*((far + near) / (far - near)),
		0.0						,	0.0						,		0.0					,	1.0
		}));
	setActiveProjection(ProjectionType::ORTHOGRAPHIC);
}

void Camera::SetZoom(const float zoom)
{
}

glm::mat4x4 Camera::getViewTransformationInverse() const
{
	return viewTransformationInverse;
}

glm::mat4x4 Camera::getProjectionTransformation() const
{
	if (currentProjectionType == ProjectionType::ORTHOGRAPHIC) {
		return orthographicProjectionTransformation;
	}
	return perspectionProjectionTransformation;
}


Camera::~Camera() { }
