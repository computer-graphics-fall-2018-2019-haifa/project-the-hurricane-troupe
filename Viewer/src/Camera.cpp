#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//source: https://en.wikipedia.org/wiki/Graphics_pipeline#Rasterisation
Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	zoom(1.0),
	viewTransformation(Utils::IdentityMat()),
	viewTransformationInverse(Utils::IdentityMat()),
	projectionTransformation(Utils::IdentityMat())
{
	SetCameraLookAt(eye, at, up);
}


void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 z = normalize(eye - at);
	glm::vec3 x = normalize(glm::cross(up, z));
	glm::vec3 y = normalize(glm::cross(z, x));
	//////glm::vec4 x4 = glm::vec4(x[0], x[1], x[2], 0);
	//////glm::vec4 y4 = glm::vec4(y[0], y[1], y[2], 0);
	//////glm::vec4 z4 = glm::vec4(z[0], z[1], z[2], 0);
	//////glm::vec4 t = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//////glm::mat4 c = glm::mat4x4(x4, y4, z4, t);
	glm::mat4x4 c = glm::transpose(glm::mat4x4({
			x[0], x[1], x[2], 0.0f,
			y[0], y[1], y[2], 0.0f,
			z[0], z[1], z[2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		}));
	glm::mat4x4 translationMatrix = glm::transpose(glm::mat4x4(
	{	1	,	0	,	0	,	-eye[0],
		0	,	1	,	0	,	-eye[1],
		0	,	0	,	1	,	-eye[2],
		0	,	0	,	0	,	1 }));

	viewTransformation = c * translationMatrix;
	viewTransformationInverse = glm::inverse(viewTransformation);
}

void Camera::SetOrthographicProjection(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
	glm::mat4x4 c = glm::transpose(glm::mat4x4({
		near, 0, 0, 0,
		0, near, 0, 0,
		0, 0, near + far, -far * near,
		0, 0, 1, 0
		}));
	projectionTransformation = glm::transpose(glm::mat4x4(
		{ 2.0 / (right - left)		,	0.0						,		0.0					,	-1.0*((right + left) / (right - left)),
			0.0						,	2.0 / (top - bottom)	,		0.0					,	-1.0*((top + bottom) / (top - bottom)),
			0.0						,	0.0						,		2.0 / (near - far)	,	-1.0*((far + near) / (far - near)),
			0.0						,	0.0						,		0.0					,	1.0
		})) * c;
}

void Camera::SetPerspectiveProjection(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
	glm::mat4x4 c = glm::transpose(glm::mat4x4({
		near, 0, 0, 0,
		0, near, 0, 0,
		0, 0, near + far, -far * near,
		0, 0, 1, 0
		}));
	projectionTransformation = glm::transpose(glm::mat4x4(
	{ (2.0 * near) / (right - left)		,	0								,		(right + left) / (right - left)		,	0,
		0								,	(2.0 * near) / (top - bottom)	,		 (top + bottom) / (top - bottom)	,	0,
		0								,	0								,		-1 * (far + near) / (far - near)	,	-1 * ((2 * far * near) / (far - near)),
		0								,	0								,	    -1									,	0
	})) * c;
}

void Camera::setProjection(bool orthographic, const float left, const float right, const float bottom, const float top, const float near, const float far)
{

	if (orthographic) {
		SetOrthographicProjection(left, right, bottom, top, near, far);
	}
	else {
		SetPerspectiveProjection(left, right, bottom, top, near, far);
	}
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
	return projectionTransformation;
}


Camera::~Camera() { }
