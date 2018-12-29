#pragma once
#include <imgui/imgui.h>
#include "Scene.h"
#include "PrimMeshModel.h"
#include "Light.h"
enum class Mode { Orthographic, Perspective };
enum class RotationType { MODEL, WORLD };
enum class ShadingType { FLAT, GOURAUD, PHONG };

class GUIStore 
{
private:
	const int SYNCERROR = -50;
	const float INITIALMODELSPEED = 1.0f; //pixels
	const float INITIALMODELNORMALLENGTH = 0.5f;
	const glm::vec3 INITIALMESHMODELCOLOR = glm::vec3(1.0f, 0.0f, 0.0f); //red color
	const ShadingType INITIALSHADING = ShadingType::FLAT;
	const int ERRORINDEX = -1;
	const float ERRORSPEED = -1.0f;
	const glm::vec3 INITIALLIGHTCOLOR = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 INITIALAMBIENTLIGHTCOLOR = glm::vec3(1.0f, 1.0f, 1.0f); // white
	const float INITIALAMBIENTLIGHTINTENSITY = 0.0f;
	const glm::vec3 ERRORDIRECTION = glm::vec3(-2.0f, -2.0f, -2.0f);
	Scene _scene;

	//Model variables
	std::vector<std::shared_ptr<MeshModel>> _models;
	int _modelCount;
	std::vector<bool> _isModelBeingManipulated;
	std::vector<bool> _isCameraBeingManipulated;
	std::vector<bool> _isModelSymmetricScaled;
	std::vector<float> _modelSpeed;
	std::vector<Utils::Normals> _whichNormals;
	std::vector<float> _modelNormalLength;
	std::vector<bool> _isModelBoundingBoxOn;
	std::vector<glm::vec3> _modelColor;
	std::vector<RotationType> _modelRotationType;
	std::vector<float> _lightSpeeds;
	//Lightning/Color/Shading Variables:
	ShadingType _shading;
	bool fog;
	glm::vec3 fogColor;
	float fogDensity;
	bool _antiAliased;
	std::vector<bool> _isLightActive;
	std::vector<glm::vec3> _lightsColors;
	int _activeLightIndex;
	int _lightsCount;
	std::vector<RotationType> _lightRotationType;
	std::vector<bool> _isLightSymmetricScaled;
	glm::vec3 _ambientLightColor;
	float _ambientLightIntensity;
	std::vector<std::pair<glm::vec3, glm::vec3>> _Lights_actualDirection_UserDirection;
	float ambientReflectionIntinsety;
	float defuseReflectionIntinsety;
	float shine;
	float aLightReflect;
	float dLightReflect;
	float sLightReflect;
	//Camera variables
	int cameraCount;
	std::vector<Mode> projModeForCams;

public:
	GUIStore(const Scene& scene);
	void sync(const Scene& scene);

	// Model Management functions:
	void setModelManipulated(int i, bool isManipulated);
	bool isModelSelected(int i) const;
	void setModelSymmetricScaled(int i, bool isSymmetric);
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
	void setModelRotationAround(int i, const RotationType& rotType);
	void setModelNormal(int i, Utils::Normals newNormal);
	Utils::Normals getModelNormalStatus(int i) const;
	//Lightning/Color/Shading Management functions:
	ShadingType getShading() const;
	void setShading(const ShadingType& type);
	void setFog(bool _bool);
	bool getFog() const;
	glm::vec3 getFogColor() const;
	void setFogColor(glm::vec3 fogColor);
	float getFogDensity() const;
	void setFogDensity(float density);
	bool isAntiAliased() const;
	void setAntiAlias(bool isAliased);
	void setActiveLight(int index);
	bool isLightActive(int index) const;
	glm::vec3 getLightColor(int index) const;
	void setLightColor(int index, const glm::vec3& color);
	float getLightSpeed(int index) const;
	void setLightSpeed(int index, const float speed);
	bool isLightRotationAroundModel(int index) const;
	bool isLightRotationAroundWorld(int index) const;
	void setLightRotationAround(int index, const RotationType& type);
	bool isLightSymmetricScaled(int index) const;
	void setLightSymmetricScaled(int index, bool isSymmetric);
	glm::vec3 getAmbientLightColor() const;
	void setAmbientLightColor(const glm::vec3& color);
	float getAmbientLightIntensity() const;
	void setAmbientLightIntensity(const float intensity);
	void updateUserSettingLightDirection(int index, float x, float y, float z);
	glm::vec3 getUserSettingLightDirection(int index) const;
	void updateActualLightDirection(int index, float x, float y, float z);
	glm::vec3 getActualLightDirection(int index) const;
	//Camera Management functions:
	void setCamsProjMode(int i, Mode mode);
	Mode getProjModeForCam(int i) const;
	void setCameraManipulated(int i, bool isManipulated);
	bool isCameraManipulated(int i) const;
	float getAmbientReflectionIntinsety() const;
	void setAmbientReflectionIntinsety(float number);
	float getDefuseReflectionIntinsety() const;
	void setDefuseReflectionIntinsety(float number);
	float getShine() const;
	void setShine(float number);
	float getALightReflection() const;
	void setAlightReflection(float number);
	float getDLightReflection() const;
	void setDlightReflection(float number);
	float getSLightReflection() const;
	void setSlightReflection(float number);
};

void DrawImguiMenus(ImGuiIO& io, Scene& scene);
void showModelsListed(std::vector<std::shared_ptr<MeshModel>> models, Scene& scene, GUIStore& store, ImGuiIO& io);
void showCamerasListed(std::vector<Camera>& cameras, Scene& scene, GUIStore& store);
void showLightningGUI(Scene& scene, GUIStore& store, ImGuiIO& io);
void showBoundingBoxGUI(Scene& scene, GUIStore& store, int index);
void GenerateGUI(ImGuiIO& io, Scene& scene, GUIStore& store);
void openModelManipulationWindow(const char* const modelName, Scene& scene, GUIStore& store, int index, float* moveSpeed);
void showNormalGUI(Scene& scene, GUIStore& store, int index);
void showModelColoringGUI(const Scene& scene, GUIStore& store, int index);
void showShadingGUI(const Scene& scene, GUIStore& store);
const glm::vec4& GetClearColor();
void showFocusButton(const char* const modelName, Scene& scene, GUIStore& store);
void showLightManipulationGUI(Scene& scene, GUIStore& store, ImGuiIO& io);
void showAddLightGUI(Scene& scene);
std::string generateLightName(const Light& light, int index);
