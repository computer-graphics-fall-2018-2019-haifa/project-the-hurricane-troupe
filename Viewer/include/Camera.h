#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"

/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 *
 * Implementation suggestion:
 * --------------------------
 * Make the Camera class be a subclass of MeshModel, so you can easily and elegantly render 
 * the cameras you have added to the scene.
 */
class Camera
{
private:
	glm::mat4x4 viewTransformation;
	glm::mat4x4 projectionTransformation;
	float zoom;

public:
	Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);
	~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);


	void SetOrthographicProjection(
		const float left,
		const float right,
		const float bottom,
		const float top,
		const float near,
		const float far);

	void SetPerspectiveProjection(const float left, const float right, const float bottom, const float top, const float near, const float far);

	void setProjection(bool orthographic, const float left = 0.0, const float right = 700.0, const float bottom = 0.0, const float top = 1000.0, const float near = 5.0, const float far = 450.0);

	void SetZoom(const float zoom);

	glm::mat4x4 getViewTransformationInverse() const;
	// Add more methods/functionality as needed...
};
