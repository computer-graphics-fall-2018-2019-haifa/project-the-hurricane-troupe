#pragma once
#include <imgui/imgui.h>
#include "Scene.h"
#include "PrimMeshModel.h"
enum class Mode { Orthographic, Perspective };
enum class RotationType { MODEL, WORLD };
enum class ShadingType { FLAT, GOURAUD, PHONG };

class GUIStore 
{
private:
	const int SYNCERROR = -50;
	const float INITIALMODELSPEED = 1.0f; //pixels
	const float INITIALMODELNORMALLENGTH = 0.5f;
	const glm::vec3 INITIALCOLOR = glm::vec3(1.0f, 0.0f, 0.0f); //red color
	const ShadingType INITIALSHADING = ShadingType::FLAT;
	const glm::vec3 INITIALFOGCOLOR = glm::vec3(128.0f/256.0f,128.0f/256.0f,128.0f/256.0f);
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
	std::vector<glm::vec3> _modelColor;
	std::vector<RotationType> _modelRotationType;
	//Lightning/Color/Shading Variables:
	ShadingType _shading;
	bool fog;
	glm::vec3 fogColor;
	float fogDensity;
	bool _msaa;
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
	glm::vec3 getModelColor(int i) const;
	void setModelColor(int i, const glm::vec3& color);
	bool isModelRotationAroundModel(int i) const;
	bool isModelRotationAroundWorld(int i) const;
	void setRotationAround(int i, const RotationType& rotType);
	//Lightning/Color/Shading Management functions:
	ShadingType getShading() const;
	void setShading(const ShadingType& type);
	void setFog(bool _bool);
	bool getFog() const;
	glm::vec3 getFogColor() const;
	void setFogColor(glm::vec3 fogColor);
	float getFogDensity() const;
	void setFogDensity(float density);
	bool getMsaa() const;
	void setMsaa(bool msaa);
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
void openModelManipulationWindow(const char* const modelName, Scene& scene, GUIStore& store, int index, float* moveSpeed);
void showNormalGUI(Scene& scene, GUIStore& store, int index);
void showModelColoringGUI(const Scene& scene, GUIStore& store, int index);
void showLightningGUI(const Scene& scene, GUIStore& store);
const glm::vec4& GetClearColor();
void showFocusButton(const char* const modelName, Scene& scene, GUIStore& store);
