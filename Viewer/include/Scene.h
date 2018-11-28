#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Camera.h"

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */
class Scene {
private:
	std::vector<std::shared_ptr<MeshModel>> models;
	std::vector<Camera> cameras;

	int activeCameraIndex;
	int activeModelIndex;

public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& model);
	const int GetModelCount() const;

	void AddCamera(Camera& camera);
	const int GetCameraCount() const;

	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;
	std::vector<Camera> GetCameras() const;
	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;
	std::vector<std::shared_ptr<MeshModel>> getSceneModels() const;
	Camera getActiveCamera() const;
	// Add more methods as needed...
	void rotateActiveModel(const RotationRules& rotation);
	void moveActiveModel(const float const xAddition = 0.0f, const float const yAddition = 0.0f, const float const zAdddition = 0.0f);
	void symmetricMoveActiveModel(const float addition);
	void scaleActiveModel(const float const xScale = 1.0f, const float const yScale = 1.0f, const float const zScale = 1.0f);
	void symmetricScaleActiveModel(const float const scalingFactor);

	void getScalingFactorsActiveModel(float* const xFactor, float* const yFactor, float* const zFactor) const;
	void getTranslationDeltasActiveModel(float* const xDelta, float* const yDelta, float* const zDelta) const;
	void setCameraVectors(glm::vec4& eye, glm::vec4& at, glm::vec4& up, int index);
	void setOrthoProjStuff(float top, float bottom, float right, float left, float _near, float _far , int index);
	void setPresProjStuff(float _near, float _far, float fovy, float aspect ,int index);

	void resetPositionActiveModel();
	void resetRotationActiveModel();

};