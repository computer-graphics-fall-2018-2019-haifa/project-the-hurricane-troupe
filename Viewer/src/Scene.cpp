#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	activeCameraIndex(-1),
	activeModelIndex(0)
{

}

void Scene::AddModel(const std::shared_ptr<MeshModel>& model)
{
	models.push_back(model);
}

const int Scene::GetModelCount() const
{
	return models.size();
}

void Scene::AddCamera(const Camera& camera)
{
	cameras.push_back(camera);
}

Camera Scene::getActiveCamera() const {
	return cameras[GetActiveCameraIndex()];
}

void Scene::rotateActiveModel(const RotationRules & rotation)
{
	models[GetActiveModelIndex()]->rotate(rotation);
}

void Scene::moveActiveModel(const float const xAddition, const float const yAddition, const float const zAdddition)
{
	models[GetActiveModelIndex()]->move(&xAddition, &yAddition, &zAdddition);
}

void Scene::symmetricMoveActiveModel(const float addition)
{
	models[GetActiveModelIndex()]->symmetricMove(&addition);
}

void Scene::scaleActiveModel(const float const xScale, const float const yScale, const float const zScale)
{
	models[GetActiveModelIndex()]->scale(xScale, yScale, zScale);
}

void Scene::symmetricScaleActiveModel(const float const scalingFactor)
{
	models[GetActiveModelIndex()]->symmetricScale(scalingFactor);
}

void Scene::getScalingFactorsActiveModel(float* const xFactor, float* const yFactor, float* const zFactor) const
{
	models[GetActiveModelIndex()]->getScalingFactors(xFactor, yFactor, zFactor);
}

void Scene::getTranslationDeltasActiveModel(float * const xDelta, float * const yDelta, float * const zDelta) const
{
	models[GetActiveModelIndex()]->getCoordinateDifference(xDelta, yDelta, zDelta);
}

void Scene::resetPositionActiveModel()
{
	models[GetActiveModelIndex()]->resetPosition();
}

void Scene::resetRotationActiveModel()
{
	models[GetActiveModelIndex()]->resetRotation();
}

const int Scene::GetCameraCount() const
{
	return cameras.size();
}

void Scene::SetActiveCameraIndex(int index)
{
	// implementation suggestion...
	if (index < 0 || index > cameras.size()) return;
	
	activeCameraIndex = index;
}

const int Scene::GetActiveCameraIndex() const
{
	return activeCameraIndex;
}

void Scene::SetActiveModelIndex(int index)
{
	if (index < 0 || index >= models.size()) return;
	// implementation suggestion...
	activeModelIndex = index;
}

const int Scene::GetActiveModelIndex() const
{
	return activeModelIndex;
}

std::vector<std::shared_ptr<MeshModel>> Scene::getSceneModels() const
{
	return models;
}


