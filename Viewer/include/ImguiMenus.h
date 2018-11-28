#pragma once
#include <imgui/imgui.h>
#include "Scene.h"

class GUIStore 
{
private:
	const int SYNCERROR = -50;
	Scene _scene;
	std::vector<std::shared_ptr<MeshModel>> _models;
	std::vector<bool> _isModelBeingManipulated;
	std::vector<bool> _isModelSymmetricScaled;
	int _modelCount;

public:
	GUIStore(const Scene& scene);
	void sync(const Scene& scene);
	void setModelManipulated(int i, bool isManipulated);
	bool isModelManipulated(int i) const;
	void setModelSymmetricScaled(int i, bool isManipulated);
	bool isModelSymmetricScaled(int i) const;

};

void DrawImguiMenus(ImGuiIO& io, Scene& scene);
void showModelsListed(std::vector<std::shared_ptr<MeshModel>> models, GUIStore& store);
void ObjectManipulationMenus(ImGuiIO& io, Scene& scene, GUIStore& store);
const glm::vec4& GetClearColor();
