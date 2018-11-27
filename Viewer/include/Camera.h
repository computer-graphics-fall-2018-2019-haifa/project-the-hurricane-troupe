#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"
#include "Utils.h"


enum class ProjectionType {
	PERSPECTIVE,
	ORTHOGRAPHIC
};

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
	const float MAXVIEWABLEANGLE = 180.0f;
	const float MINVIEWABLEANGLE = 0.0f;
	const float MAXZOOM = 10.0f;
	glm::mat4x4 viewTransformation;
	glm::mat4x4 viewTransformationInverse;
	glm::mat4x4 orthographicProjectionTransformation;
	glm::mat4x4 perspectionProjectionTransformation;
	ProjectionType currentProjectionType;
	float zoom;

	float _projLeft, _projRight, _projTop, _projBottom, _projNear, _projFar, _projFovy, _projAspectRatio;

	void _SetPerspectiveProjection(const float left, const float right, const float bottom, const float top, const float near, const float far);

public:
	Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);
	~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	bool isOrthographicProjection() const;
	bool isPerspectiveProjection() const;
	ProjectionType whichProjection() const;
	void setActiveProjection(const ProjectionType& type);
	void setOrthographicProjection(const float left = -1.0, const float right = 1.0, const float bottom = -1.0, const float top = 1.0, const float near = 1.0, const float far = -1.0);
	void setPerspectiveProjection(const float fovy, const float aspectRatio, const float near, const float far, AngleUnits unit = AngleUnits::DEGREES);

	void SetZoom(const float zoom);
	// returns the max zoom which allows up to 180 degrees. (not inclusive)
	float getMaxZoomAllowed();
	// returns the min zoom which allows minimum 0 degrees. (not inclusive)
	float getMinZoomAllowed();

	glm::mat4x4 getViewTransformationInverse() const;
	glm::mat4x4 getProjectionTransformation() const;
	// Add more methods/functionality as needed...
};


