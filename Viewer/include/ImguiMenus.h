#pragma once
#include <imgui/imgui.h>
#include "Scene.h"

enum class Mode { Orthographic, Perspective };

class GUIStore 
{
private:
	const int SYNCERROR = -50;
	const int INITIALMODELSPEED = 10.0f; //pixels
	Scene _scene;
	std::vector<std::shared_ptr<MeshModel>> _models;
	std::vector<bool> _isModelBeingManipulated;
	std::vector<bool> _isCameraBeingManipulated;
	std::vector<bool> _isModelSymmetricScaled;
	std::vector<float> _modelSpeed;
	int _modelCount;
	int cameraCount;
	std::vector<Mode> projModeForCams;

public:
	GUIStore(const Scene& scene);
	void sync(const Scene& scene);
	void setModelManipulated(int i, bool isManipulated);
	bool isModelManipulated(int i) const;
	void setModelSymmetricScaled(int i, bool isManipulated);
	bool isModelSymmetricScaled(int i) const;
	void setModelSpeed(int i, float newSpeed);
	float getModelSpeed(int i) const;

	void setCamsProjMode(int i, Mode mode);
	Mode getProjModeForCam(int i) const;
	void setCameraManipulated(int i, bool isManipulated);
	bool isCameraManipulated(int i) const;
};

void DrawImguiMenus(ImGuiIO& io, Scene& scene);
void showModelsListed(std::vector<std::shared_ptr<MeshModel>> models, Scene& scene, GUIStore& store, ImGuiIO& io);
void ObjectManipulationMenus(ImGuiIO& io, Scene& scene, GUIStore& store);
const glm::vec4& GetClearColor();
