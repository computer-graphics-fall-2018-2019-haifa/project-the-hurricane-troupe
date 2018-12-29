#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Camera.h"
#include "Light.h"

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */
class Scene {
private:
	std::vector<std::shared_ptr<MeshModel>> models;
	std::vector<Camera> cameras;
	std::vector<std::shared_ptr<Light>> lights;

	int activeCameraIndex;
	int activeModelIndex;
	int activeLightIndex;

public:
	Scene();

	// Model controls:
	void AddModel(const std::shared_ptr<MeshModel>& model);
	const int GetModelCount() const;
	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;
	std::vector<std::shared_ptr<MeshModel>> getSceneModels() const;

	void rotateActiveModel(const RotationRules& rotation);
	void rotateActiveModelAroundWorld(const RotationRules & rotation);
	void moveActiveModel(const float const xAddition = 0.0f, const float const yAddition = 0.0f, const float const zAdddition = 0.0f);
	void moveActiveLight(const float const xAddition = 0.0f, const float const yAddition = 0.0f, const float const zAdddition = 0.0f);
	void symmetricMoveActiveModel(const float addition);
	void scaleActiveModel(const float const xScale = 1.0f, const float const yScale = 1.0f, const float const zScale = 1.0f);
	void symmetricScaleActiveModel(const float const scalingFactor);
	void getScalingFactorsActiveModel(float* const xFactor, float* const yFactor, float* const zFactor) const;

	void getTranslationDeltasActiveModel(float* const xDelta, float* const yDelta, float* const zDelta) const;
	void setCameraVectors(glm::vec4& eye, glm::vec4& at, glm::vec4& up, int index);
	void setOrthographicParameters(float top, float bottom, float right, float left, float _near, float _far , int index);
	void setPerspectiveParameters(float _near, float _far, float fovy, float aspect ,int index, AngleUnits unit = AngleUnits::DEGREES);
	void setZoomForCam(float zoom, int index);
	void resetPositionActiveModel();
	void resetRotationActiveModel();
	void resetRotationAroundWorldActiveModel();


	//Camera controls:
	void AddCamera(Camera& camera);
	const int GetCameraCount() const;
	Camera getActiveCamera() const;
	std::vector<Camera> GetCameras() const;
	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;
	void setActiveCameraAspectRatio(int oldWidth, int oldHeight, int newWidth, int newHeight);

	////Light controls:
	int getLightCount() const;
	void SetActiveLight(int index);
	std::vector<std::shared_ptr<Light>> getLights() const;
	void symmetricScaleActiveLight(const float scalingFactor);
	void getScalingFactorsActiveLight(float* const xFactor, float* const yFactor, float* const zFactor) const;
	void scaleActiveLight(const float const xScale = 1.0f, const float const yScale = 1.0f, const float const zScale = 1.0f);
	void AddLight(const std::shared_ptr<Light>& light);
	void resetRotationActiveLight();
	void resetRotationAroundWorldActiveLight();
	void rotateActiveLight(const RotationRules& rotation);
	void rotateActiveLightAroundWorld(const RotationRules & rotation);
	void updateLightDirection(int index, float x, float y, float z);
	Light getActiveLight() const;
};
