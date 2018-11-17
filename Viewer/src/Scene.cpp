#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	activeCameraIndex(0),
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

const int Scene::GetCameraCount() const
{
	return cameras.size();
}

void Scene::SetActiveCameraIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < cameras.size())
	{
		activeCameraIndex = index;
	}
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
	cameras[index].SetPerspectiveProjection(0.0, 700.0, 0.0, 1000.0, 5.0, 1000.0);
}

const int Scene::GetActiveModelIndex() const
{
	return activeModelIndex;
}

std::vector<std::shared_ptr<MeshModel>> Scene::getSceneModels() const
{
	return models;
}


