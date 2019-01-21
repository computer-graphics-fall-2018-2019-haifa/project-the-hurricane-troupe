#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//source: https://en.wikipedia.org/wiki/Graphics_pipeline#Rasterisation
Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	eye(eye),
	at(at),
	up(up),
	OrigAt(at),
	OrigEye(eye),
	OrigUp(up),
	zoom(1.0),
	viewTransformation(Utils::IdentityMat()),
	viewTransformationInverse(Utils::IdentityMat()),
	perspectionProjectionTransformation(Utils::IdentityMat()),
	orthographicProjectionTransformation(Utils::IdentityMat()),
	_projLeft(-10.0f), _projRight(10.0f), _projBottom(-10.0f), _projTop(10.0f), _projNear(1.0), _projFar(10.0), _projFovy(90.0f), _projAspectRatio(1.5f)
{
	setPerspectiveProjection(_projFovy, _projAspectRatio, _projNear, _projFar, AngleUnits::DEGREES);
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

void Camera::setIndex(int index)
{
	this->index = index;
}

int Camera::getIndex()
{
	return index;
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


void Camera::_SetPerspectiveProjection(const float left, const float right, const float bottom, const float top, const float _near, const float _far)
{
	this->_projLeft = left;
	this->_projRight = right;
	this->_projBottom = bottom;
	this->_projTop = top;
	this->_projNear = _near;
	this->_projFar = _far;
	perspectionProjectionTransformation = glm::transpose(glm::mat4x4(
	{ (2.0 * _near) / (right - left)		,	0								,		(right + left) / (right - left)		,	0,
		0								,	(2.0 * _near) / (top - bottom)	,		 (top + bottom) / (top - bottom)	,	0,
		0								,	0								,		-1 * (_far + _near) / (_far - _near)	,	-1 * ((2 * _far * _near) / (_far - _near)),
		0								,	0								,	    -1									,	0
	}));
	setActiveProjection(ProjectionType::PERSPECTIVE);
}

void Camera::setPerspectiveProjection(const float fovy, const float aspectRatio, const float _near, const float _far, AngleUnits unit)
{
	float actualFovy = 0.0f;
	switch (unit) {
	case AngleUnits::RADIANS:
		actualFovy = fovy;
		break;
	case AngleUnits::DEGREES:
		actualFovy = ((fovy * ((float)(3.14159265359)))) / 180.0f; //to radians
		break;
	default:
		break;
	}
	actualFovy /= zoom;

	if (actualFovy <= MINVIEWABLEANGLE || actualFovy >= MAXVIEWABLEANGLE) return;

	float height = glm::abs(glm::tan(actualFovy / 2.0f)*_near);
	float width = aspectRatio * height;
	_SetPerspectiveProjection(-width, width, -height, height, _near, _far);
	this->_projFovy = fovy;
	this->_projAspectRatio = aspectRatio;
}

void Camera::setOrthographicProjection(const float left, const float right, const float bottom, const float top, const float _near, const float _far)
{
	this->_projLeft = left;
	this->_projRight = right;
	this->_projBottom = bottom;
	this->_projTop = top;
	this->_projNear = _near;
	this->_projFar = _far;
	orthographicProjectionTransformation = glm::transpose(glm::mat4x4(
		{ 2.0 / (right - left)		,	0.0						,		0.0					,	-1.0*((right + left) / (right - left)),
		0.0						,	2.0 / (top - bottom)	,		0.0					,	-1.0*((top + bottom) / (top - bottom)),
		0.0						,	0.0						,		2.0 / (_near - _far)	,	-1.0*((_far + _near) / (_far - _near)),
		0.0						,	0.0						,		0.0					,	1.0
		}));
	setActiveProjection(ProjectionType::ORTHOGRAPHIC);
}

void Camera::SetZoom(const float zoom)
{
	if (zoom <= 0.0) return;

	float expectedFovy = this->_projFovy / zoom;
	if (expectedFovy >= MAXVIEWABLEANGLE || expectedFovy <= MINVIEWABLEANGLE) return;
	
	ProjectionType a = whichProjection();
	this->zoom = zoom;
	
	//updates near, far, left, right, etc.. (required for orthographic updates)
	setPerspectiveProjection(this->_projFovy, this->_projAspectRatio, this->_projNear, this->_projFar, AngleUnits::DEGREES);

	//returns back the view to what kind it was
	if (a == ProjectionType::PERSPECTIVE) return;
	if (a == ProjectionType::ORTHOGRAPHIC) setOrthographicProjection(this->_projLeft, this->_projRight, this->_projBottom, this->_projTop, this->_projNear, this->_projFar);

}

float Camera::getZoom() const
{
	return zoom;
}

float Camera::getMaxZoomAllowed()
{
	return MAXZOOM;
}

float Camera::getMinZoomAllowed()
{
	return this->_projFovy/MAXVIEWABLEANGLE;
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


glm::vec4 Camera::getOrigEye()
{
	return OrigEye;
}

glm::vec4 Camera::getOrigUp()
{
	return OrigUp;
}

glm::vec4 Camera::getOrigAt()
{
	return OrigAt;
}


glm::vec4 Camera::getEyeVector() const
{
	return eye;
}

void Camera::setEyeVector(glm::vec4& vec)
{
	eye = vec;
}

glm::vec4 Camera::getAtVector()
{
	return at;
}

void Camera::setAtVector(glm::vec4& vec)
{
	at = vec;
}

glm::vec4 Camera::getUpVector()
{
	return up;
}

void Camera::setUpVector(glm::vec4& vec)
{
	up = vec;
}


void Camera::getPerspectiveProjParameters(float* const projNear, float* const projFar, float* const projFovy, float* const projAspectRatio) const
{
	*projNear = _projNear;
	*projFar = _projFar;
	*projFovy = _projFovy;
	*projAspectRatio = _projAspectRatio;
}

void Camera::getOrthographicProjParameters(float* const projNear, float* const projFar, float* const projTop, float* const projBottom, float* const projLeft, float* const projRight) const
{
	*projNear = _projNear;
	*projFar = _projFar;
	*projTop = _projTop;
	*projBottom = _projBottom;
	*projLeft = _projLeft;
	*projRight = _projRight;
}



Camera::~Camera() { }
