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

void Scene::AddCamera(Camera& camera)
{
	camera.setIndex(cameras.size());
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

void Scene::setCameraVectors(glm::vec4& eye, glm::vec4& at, glm::vec4& up, int index)
{
	cameras[index].setEye(eye);
	cameras[index].setAt(at);
	cameras[index].setUp(up);
	cameras[index].SetCameraLookAt(eye, at, up);
}

void Scene::setOrthoProjStuff(float top, float bottom, float right, float left, float _near, float _far, int index)
{
	cameras[index].setOrthographicProjection(left, right, bottom, top, _near, _far);
	cameras[index].setActiveProjection(ProjectionType::ORTHOGRAPHIC);
}

void Scene::setPresProjStuff(float _near, float _far, float fovy, float aspect,int index)
{
	cameras[index].setPerspectiveProjection(fovy, aspect, _near, _far);
	cameras[index].setActiveProjection(ProjectionType::PERSPECTIVE);
}

void Scene::setZoomForCam(float zoom, int index)
{
	cameras[index].SetZoom(zoom);
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

std::vector<Camera> Scene::GetCameras() const
{
	return cameras;
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


