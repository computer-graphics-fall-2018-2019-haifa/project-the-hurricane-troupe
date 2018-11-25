#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"
#include "Utils.h"

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
	glm::mat4x4 viewTransformationInverse;
	glm::mat4x4 projectionTransformation;
	float zoom;

		void SetOrthographicProjection(
		const float left,
		const float right,
		const float bottom,
		const float top,
		const float near,
		const float far);

	void SetPerspectiveProjection(const float left, const float right, const float bottom, const float top, const float near, const float far);

public:
	Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);
	~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	void setProjection(bool orthographic, const float left = -1.0, const float right = 1.0, const float bottom = -1.0, const float top = 1.0, const float near = 1.0, const float far = -1.0);

	void SetZoom(const float zoom);

	glm::mat4x4 getViewTransformationInverse() const;
	glm::mat4x4 getProjectionTransformation() const;
	// Add more methods/functionality as needed...
};
