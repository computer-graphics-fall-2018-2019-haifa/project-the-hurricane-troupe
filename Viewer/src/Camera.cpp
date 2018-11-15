#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	zoom(1.0)
{
	SetCameraLookAt(eye, at, up);
}

Camera::~Camera()
{
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 z = normalize(eye - at);
	glm::vec3 x = normalize(glm::cross(up, z));
	glm::vec3 y = normalize(glm::cross(z, x));
	glm::vec4 z4(z[0], z[1], z[2], 0);
	glm::vec4 x4(x[0], x[1], x[2], 0);
	glm::vec4 y4(y[0], y[1], y[2], 0);
	glm::vec4 t(0, 0, 0, 1);
	glm::mat4 c = glm::mat4(x4, y4, z4, t);
	glm::mat4x4 translationMatrix
	{ 1	,	0	,	0	,	    -eye[0],
		0	,	1	,	0	,	-eye[1],
		0	,	0	,	1	,	-eye[2],
		0	,	0	,	0	,	1 };

	viewTransformation = c * translationMatrix;
}

void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float near,
	const float far)
{

}

void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{
	 
}

void Camera::SetZoom(const float zoom)
{

}