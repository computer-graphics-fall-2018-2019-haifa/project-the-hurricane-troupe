#pragma once
#include <imgui/imgui.h>
#include "Scene.h"
#include "PrimMeshModel.h"
enum class Mode { Orthographic, Perspective };

class GUIStore 
{
private:
	const int SYNCERROR = -50;
	const float INITIALMODELSPEED = 1.0f; //pixels
	const float INITIALMODELNORMALLENGTH = 0.5f;
	Scene _scene;

	//Model variables
	std::vector<std::shared_ptr<MeshModel>> _models;
	std::vector<bool> _isModelBeingManipulated;
	std::vector<bool> _isCameraBeingManipulated;
	std::vector<bool> _isModelSymmetricScaled;
	std::vector<float> _modelSpeed;
	std::vector<Utils::Normals> _whichNormals;
	std::vector<float> _modelNormalLength;
	std::vector<bool> _isModelBoundingBoxOn;

	//Camera variables
	int _modelCount;
	int cameraCount;
	std::vector<Mode> projModeForCams;

public:
	GUIStore(const Scene& scene);
	void sync(const Scene& scene);

	// Model Management functions:
	void setModelManipulated(int i, bool isManipulated);
	bool isModelSelected(int i) const;
	void setModelSymmetricScaled(int i, bool isManipulated);
	bool isModelSymmetricScaled(int i) const;
	void setModelSpeed(int i, float newSpeed);
	float getModelSpeed(int i) const;
	void setModelBoundingBox(int i, bool on);
	bool isModelBoundingBoxOn(int i) const;
	void setModelNormalLength(int i, float length);
	float getModelNormalLength(int i) const;

	//Camera Management functions:
	void setCamsProjMode(int i, Mode mode);
	Mode getProjModeForCam(int i) const;
	void setCameraManipulated(int i, bool isManipulated);
	bool isCameraManipulated(int i) const;
	void setModelNormal(int i, Utils::Normals newNormal);
	Utils::Normals getModelNormalStatus(int i) const;
};

void DrawImguiMenus(ImGuiIO& io, Scene& scene);
void showModelsListed(std::vector<std::shared_ptr<MeshModel>> models, Scene& scene, GUIStore& store, ImGuiIO& io);
void showBoundingBoxGUI(Scene& scene, GUIStore& store, int index);
void GenerateGUI(ImGuiIO& io, Scene& scene, GUIStore& store);
const glm::vec4& GetClearColor();

void showNormalGUI(Scene & scene, GUIStore & store, int index);
