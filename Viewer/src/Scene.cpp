#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	activeCameraIndex(-1),
	activeModelIndex(0),
	activeLightIndex(0)
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
void Scene::rotateActiveLight(const RotationRules & rotation)
{
	lights[activeLightIndex]->rotate(rotation);
}

void Scene::rotateActiveModel(const RotationRules & rotation)
{
	models[GetActiveModelIndex()]->rotate(rotation);
}


void Scene::rotateActiveLightAroundWorld(const RotationRules & rotation)
{
	lights[activeLightIndex]->rotateAroundWorld(rotation);
}

void Scene::updateLightDirection(int index, float x, float y, float z)
{
	if (index < 0 || index > lights.size()) return;
	lights[index]->setDirection(glm::vec3(x, y, z));
}


void Scene::rotateActiveModelAroundWorld(const RotationRules & rotation)
{
	models[GetActiveModelIndex()]->rotateAroundWorld(rotation);
}

void Scene::moveActiveModel(const float const xAddition, const float const yAddition, const float const zAdddition)
{
	models[GetActiveModelIndex()]->move(&xAddition, &yAddition, &zAdddition);
}

void Scene::moveActiveLight(const float const xAddition, const float const yAddition, const float const zAdddition) 
{
	lights[activeLightIndex]->move(&xAddition, &yAddition, &zAdddition);
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

void Scene::resetRotationAroundWorldActiveModel()
{
	models[GetActiveModelIndex()]->resetRotationAroundWorld();
}


void Scene::setCameraVectors(glm::vec4& eye, glm::vec4& at, glm::vec4& up, int index)
{
	cameras[index].setEyeVector(eye);
	cameras[index].setAtVector(at);
	cameras[index].setUpVector(up);
	cameras[index].SetCameraLookAt(eye, at, up);
}

void Scene::setOrthographicParameters(float top, float bottom, float right, float left, float _near, float _far, int index)
{
	cameras[index].setOrthographicProjection(left, right, bottom, top, _near, _far);
	cameras[index].setActiveProjection(ProjectionType::ORTHOGRAPHIC);
}

void Scene::setPerspectiveParameters(float _near, float _far, float fovy, float aspect,int index, AngleUnits unit)
{
	cameras[index].setPerspectiveProjection(fovy, aspect, _near, _far, unit);
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
	if (index < 0 || index >= cameras.size()) return;
	activeCameraIndex = index;
}

const int Scene::GetActiveCameraIndex() const
{
	return activeCameraIndex;
}

void Scene::setActiveCameraAspectRatio(int oldWidth, int oldHeight, int newWidth, int newHeight)
{
	Camera activeCam = getActiveCamera();
	ProjectionType which = activeCam.whichProjection();
	float pNear = -1.0f, pFar = -1.0f, pFovy = -1.0f, pAspectRatio = -1.0f;
	activeCam.getPerspectiveProjParameters(&pNear, &pFar, &pFovy, &pAspectRatio);
	pAspectRatio = (float)(newWidth) / (float)(newHeight);

	setPerspectiveParameters(pNear, pFar, pFovy, pAspectRatio, activeCameraIndex, AngleUnits::RADIANS);
	cameras[activeCameraIndex].setActiveProjection(which);
}

Light Scene::getActiveLight() const
{
	return *lights[activeLightIndex];
}

int Scene::getLightCount() const
{
	return lights.size();
}

void Scene::SetActiveLight(int index)
{
	if (index < 0 || index >= lights.size()) return;
	activeLightIndex = index;
}

std::vector<std::shared_ptr<Light>> Scene::getLights() const
{
	return lights;
}

void Scene::symmetricScaleActiveLight(const float scalingFactor)
{
	lights[activeLightIndex]->symmetricScale(scalingFactor);
}

void Scene::getScalingFactorsActiveLight(float * const xFactor, float * const yFactor, float * const zFactor) const
{
	lights[activeLightIndex]->getScalingFactors(xFactor, yFactor, zFactor);
}

void Scene::scaleActiveLight(const float const xScale, const float const yScale, const float const zScale)
{
	lights[activeLightIndex]->scale(xScale, yScale, zScale);
}

void Scene::AddLight(const std::shared_ptr<Light>& light)
{
	lights.push_back(light);
}

void Scene::resetRotationActiveLight()
{
	lights[activeLightIndex]->resetRotation();
}

void Scene::resetRotationAroundWorldActiveLight()
{
	lights[activeLightIndex]->resetRotationAroundWorld();
}

std::vector<Camera> Scene::GetCameras() const
{
	return cameras;
}

void Scene::SetActiveModelIndex(int index)
{
	if (index < 0 || index >= models.size()) return;
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


