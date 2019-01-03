#pragma once
#define _USE_MATH_DEFINES

#include "ImguiMenus.h"
#include "MeshModel.h"
#include "Utils.h"
#include <cmath>
#include <memory>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <nfd.h>
#include <random>

bool showDemoWindow = false;
bool showAnotherWindow = false;

glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

const glm::vec4& GetClearColor()
{
	return clearColor;
}

char* const stringToCharSeq(const std::string& str)
{
	int len = (int)str.length() + 1;
	char** seq = new char*[1];
	seq[0] = new char[len];
	for (int i = 0; i < len; ++i)
	{
		seq[0][i] = str[i];
	}
	return seq[0];
}

char* stringIntConcatenate(char* str, int i) { return stringToCharSeq(std::string(str) + std::to_string(i)); }

void showModelScaleGUI(Scene& scene, GUIStore& store, bool isSymmetric, int index, bool isLight = false) {
	float* xFactor = new float(0.0f);
	float* yFactor = new float(0.0f);
	float* zFactor = new float(0.0f);
	ImGui::Text("Scale:");
	ImGui::SameLine();
	if (ImGui::RadioButton(stringToCharSeq(std::string("Altogether##") + std::to_string(index)), store.isModelSymmetricScaled(index))) store.setModelSymmetricScaled(index, true);
	ImGui::SameLine();
	if (ImGui::RadioButton(stringToCharSeq(std::string("Seperate##") + std::to_string(index)), !store.isModelSymmetricScaled(index))) store.setModelSymmetricScaled(index, false);
	ImGui::SameLine();
	char* modelsReset = stringIntConcatenate("Reset##AllAxisReset", index);
	if (ImGui::Button(modelsReset)) { *xFactor = 1.0f; *yFactor = 1.0f; *zFactor = 1.0f; scene.symmetricScaleActiveModel(1.0f); }

	scene.getScalingFactorsActiveModel(xFactor, yFactor, zFactor);
	if (isSymmetric) {
		char* allScale = stringIntConcatenate("Scale##AllAxisScale", index);
		ImGui::SliderFloat(allScale, xFactor, -4.0f, 4.0f);
		*zFactor = *xFactor; *yFactor = *xFactor;
	}
	else {
		char* xScaleText = stringIntConcatenate("X Scale##XScale", index);
		char* yScaleText = stringIntConcatenate("Y Scale##YScale", index);
		char* zScaleText = stringIntConcatenate("z Scale##ZScale", index);

		char* xReset = stringIntConcatenate("Reset##XResetScale", index);
		char* yReset = stringIntConcatenate("Reset##YResetScale", index);
		char* zReset = stringIntConcatenate("Reset##ZResetScale", index);

		ImGui::SliderFloat(xScaleText, xFactor, -4.0f, 4.0f); ImGui::SameLine(); if (ImGui::Button(xReset)) { *xFactor = 1.0f; }
		ImGui::SliderFloat(yScaleText, yFactor, -4.0f, 4.0f); ImGui::SameLine(); if (ImGui::Button(yReset)) { *yFactor = 1.0f; }
		ImGui::SliderFloat(zScaleText, zFactor, -4.0f, 4.0f); ImGui::SameLine(); if (ImGui::Button(zReset)) { *zFactor = 1.0f; }
	}

	scene.scaleActiveModel(*xFactor, *yFactor, *zFactor);
	return;
}

void showLightScaleGUI(Scene& scene, GUIStore& store, bool isSymmetric, int index) {
	float* xFactor = new float(0.0f);
	float* yFactor = new float(0.0f);
	float* zFactor = new float(0.0f);
	ImGui::Text("Scale:");
	ImGui::SameLine();
	if (ImGui::RadioButton(stringToCharSeq(std::string("Altogether##Light") + std::to_string(index)), store.isLightSymmetricScaled(index))) store.setLightSymmetricScaled(index, true);
	ImGui::SameLine();
	if (ImGui::RadioButton(stringToCharSeq(std::string("Seperate##Light") + std::to_string(index)), !store.isLightSymmetricScaled(index))) store.setLightSymmetricScaled(index, false);
	ImGui::SameLine();
	char* lightReset = stringIntConcatenate("Reset##AllAxisResetLight", index);
	if (ImGui::Button(lightReset)) { *xFactor = 1.0f; *yFactor = 1.0f; *zFactor = 1.0f; scene.symmetricScaleActiveLight(1.0f); }

	scene.getScalingFactorsActiveLight(xFactor, yFactor, zFactor);
	if (isSymmetric) {
		char* allScale = stringIntConcatenate("Scale##AllAxisScaleLight", index);
		ImGui::SliderFloat(allScale, xFactor, -4.0f, 4.0f);
		*zFactor = *xFactor; *yFactor = *xFactor;
	}
	else {
		char* xScaleText = stringIntConcatenate("X Scale##XScaleLight", index);
		char* yScaleText = stringIntConcatenate("Y Scale##YScaleLight", index);
		char* zScaleText = stringIntConcatenate("z Scale##ZScaleLight", index);

		char* xReset = stringIntConcatenate("Reset##XResetScaleLight", index);
		char* yReset = stringIntConcatenate("Reset##YResetScaleLight", index);
		char* zReset = stringIntConcatenate("Reset##ZResetScaleLight", index);

		ImGui::SliderFloat(xScaleText, xFactor, -4.0f, 4.0f); ImGui::SameLine(); if (ImGui::Button(xReset)) { *xFactor = 1.0f; }
		ImGui::SliderFloat(yScaleText, yFactor, -4.0f, 4.0f); ImGui::SameLine(); if (ImGui::Button(yReset)) { *yFactor = 1.0f; }
		ImGui::SliderFloat(zScaleText, zFactor, -4.0f, 4.0f); ImGui::SameLine(); if (ImGui::Button(zReset)) { *zFactor = 1.0f; }
	}

	scene.scaleActiveLight(*xFactor, *yFactor, *zFactor);
	return;
}

void handleTranslationFromKeyboardInput(const char* const modelName, Scene& scene, GUIStore& store, ImGuiIO& io, float moveStrength, bool isLight = false) {
	float xAddition = 0.0f, yAddition = 0.0f, zAddition = 0.0f;
	bool changed = false;
	int left = io.KeyMap[ImGuiKey_::ImGuiKey_LeftArrow];
	int right = io.KeyMap[ImGuiKey_::ImGuiKey_RightArrow];
	int up = io.KeyMap[ImGuiKey_::ImGuiKey_UpArrow];
	int down = io.KeyMap[ImGuiKey_::ImGuiKey_DownArrow];
	int shift = io.KeyShift;
	if (ImGui::IsKeyDown(up)) {
		if (shift) { zAddition -= moveStrength; }
		else { yAddition += moveStrength; }
		changed = true;
	}
	else if (ImGui::IsKeyDown(down)) {
		if (shift) { zAddition += moveStrength; }
		else { yAddition -= moveStrength; }
		changed = true;
	}
	else if (ImGui::IsKeyDown(left)) { xAddition -= moveStrength; changed = true; }
	else if (ImGui::IsKeyDown(right)) {
		xAddition += moveStrength;
		changed = true;
	}
	if (changed) {
		if (isLight == true) {
			scene.moveActiveLight(xAddition, yAddition, zAddition);
		}
		else {
			scene.moveActiveModel(xAddition, yAddition, zAddition);
		}
	}
}

void showTranslationGUI(Scene& scene, GUIStore& store, int index, float* moveSpeed) {
	if (ImGui::SliderFloat(stringIntConcatenate("move speed##Speed", index), moveSpeed, 0.0f, 30.0f)) {
		store.setModelSpeed(index, *moveSpeed);
	}
	ImGui::SameLine();
	if (ImGui::Button(stringIntConcatenate("Reset Position##ResetPosition", index))) {
		scene.resetPositionActiveModel();
	}
}


void showLightRotationGUI(Scene& scene, GUIStore& store, int index)
{
	float xAngle = 0.0f;
	float yAngle = 0.0f;
	float zAngle = 0.0f;
	bool modifiedRotation = false;
	bool isRotationAroundSelf = store.isLightRotationAroundModel(index);
	bool isWorldRotation = store.isLightRotationAroundWorld(index);

	ImGui::Text("Rotate Controls:");
	if (ImGui::RadioButton(stringIntConcatenate("Model##ModelRotation", index), isRotationAroundSelf)) {
		store.setLightRotationAround(index, RotationType::MODEL);
	}
	ImGui::SameLine();
	if (ImGui::RadioButton(stringIntConcatenate("World##WorldRotation", index), isWorldRotation)) {
		store.setLightRotationAround(index, RotationType::WORLD);
	}
	ImGui::PushButtonRepeat(true);
	if (ImGui::ArrowButton(stringIntConcatenate("Z##RotateZMinus", index), ImGuiDir_Up)) { zAngle = -15.0f; modifiedRotation = true; }
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("X##RotateXPlus", index), ImGuiDir_Up)) {
		xAngle = 15.0f; modifiedRotation = true;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("Z##RotateZPlus", index), ImGuiDir_Up)) { zAngle = 15.0f; modifiedRotation = true; }

	if (ImGui::ArrowButton(stringIntConcatenate("Y##RotateYMinus", index), ImGuiDir_Left)) { yAngle = -15.0f; modifiedRotation = true; }
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("X##RotateXMinus", index), ImGuiDir_Down)) { xAngle = -15.0f; modifiedRotation = true; }
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("Y##RotateYPlus", index), ImGuiDir_Right)) { yAngle = 15.0f; modifiedRotation = true; }

	if (ImGui::Button(stringIntConcatenate("Clear Rotation##ClearRotation", index))) {
		if (isRotationAroundSelf == true) {
			scene.resetRotationActiveLight();
		}
		else {
			scene.resetRotationAroundWorldActiveLight();
		}
	}
	ImGui::PopButtonRepeat();

	if (modifiedRotation) {
		if (isRotationAroundSelf) {
			scene.rotateActiveLight(RotationRules(xAngle, yAngle, zAngle, AngleUnits::DEGREES));
		}
		else {
			scene.rotateActiveLightAroundWorld(RotationRules(xAngle, yAngle, zAngle, AngleUnits::DEGREES));
		}
	}
}


void showModelRotationGUI(Scene& scene, GUIStore& store, int index)
{
	float xAngle = 0.0f;
	float yAngle = 0.0f;
	float zAngle = 0.0f;
	bool modifiedRotation = false;
	bool isRotationAroundSelf = store.isModelRotationAroundModel(index);
	bool isRotationAroundWorld = store.isModelRotationAroundWorld(index);

	ImGui::Text("Rotate Controls:");
	if (ImGui::RadioButton(stringIntConcatenate("Model##ModelRotation", index), isRotationAroundSelf)) {
		store.setModelRotationAround(index, RotationType::MODEL);
	}
	ImGui::SameLine();
	if (ImGui::RadioButton(stringIntConcatenate("World##WorldRotation", index), isRotationAroundWorld)) {
		store.setModelRotationAround(index, RotationType::WORLD);
	}
	ImGui::PushButtonRepeat(true);
	if (ImGui::ArrowButton(stringIntConcatenate("Z##RotateZMinus", index), ImGuiDir_Up)) { zAngle = -15.0f; modifiedRotation = true; }
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("X##RotateXPlus", index), ImGuiDir_Up)) {
		xAngle = 15.0f; modifiedRotation = true;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("Z##RotateZPlus", index), ImGuiDir_Up)) { zAngle = 15.0f; modifiedRotation = true; }

	if (ImGui::ArrowButton(stringIntConcatenate("Y##RotateYMinus", index), ImGuiDir_Left)) { yAngle = -15.0f; modifiedRotation = true; }
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("X##RotateXMinus", index), ImGuiDir_Down)) { xAngle = -15.0f; modifiedRotation = true; }
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("Y##RotateYPlus", index), ImGuiDir_Right)) { yAngle = 15.0f; modifiedRotation = true; }

	if (ImGui::Button(stringIntConcatenate("Clear Rotation##ClearRotation", index))) {
		if (isRotationAroundSelf == true) {
			scene.resetRotationActiveModel();
		}
		else {
			scene.resetRotationAroundWorldActiveModel();
		}
	}
	ImGui::PopButtonRepeat();

	if (modifiedRotation) {
		if (isRotationAroundSelf) {
			scene.rotateActiveModel(RotationRules(xAngle, yAngle, zAngle, AngleUnits::DEGREES));
		}
		else {
			scene.rotateActiveModelAroundWorld(RotationRules(xAngle, yAngle, zAngle, AngleUnits::DEGREES));
		}
	}
}

void showNormalGUI(Scene& scene, GUIStore& store, int index) {
	Utils::Normals whichNormal = store.getModelNormalStatus(index);
	float length = store.getModelNormalLength(index);
	bool isPerVertex = whichNormal == Utils::Normals::PerVERTEX;
	bool isPerFace = whichNormal == Utils::Normals::PerFACE;
	bool isNone = whichNormal == Utils::Normals::NONE;
	if (ImGui::Checkbox(stringIntConcatenate("Show Normals Per Vertex##VertexNormals", index), &isPerVertex)) {
		store.setModelNormal(index, Utils::Normals::PerVERTEX);
	}
	if (ImGui::Checkbox(stringIntConcatenate("Show Normals Per Face##FaceNormals", index), &isPerFace)) { store.setModelNormal(index, Utils::Normals::PerFACE); }
	if (ImGui::Checkbox(stringIntConcatenate("No Normals##FaceNormals", index), &isNone)) { store.setModelNormal(index, Utils::Normals::NONE); }
	if (isNone == false) {
		if (ImGui::SliderFloat(stringIntConcatenate("Length##NormalsLength", index), &length, 0.01f, 20.0f)) { store.setModelNormalLength(index, length); }
	}
}

void showModelColoringGUI(const Scene & scene, GUIStore & store, int index) {
	glm::vec3 newColor = store.getModelColor(index);
	if (ImGui::ColorEdit3(stringIntConcatenate("Model Color##AllTrianglesColors", index), (float*)&newColor)) {
		store.setModelColor(index, newColor);
	}
}

void showShadingGUI(const Scene & scene, GUIStore & store)
{
	ShadingType shade = store.getShading();
	bool isFlatShading = shade == ShadingType::FLAT;
	bool isGouraudShaing = shade == ShadingType::GOURAUD;
	bool isPhongShading = shade == ShadingType::PHONG;
	ImGui::Text("Shading: ");
	ImGui::SameLine();
	if (ImGui::RadioButton("Flat##Shading", isFlatShading)) {
		store.setShading(ShadingType::FLAT);
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Gouraud##Shading", isGouraudShaing)) {
		store.setShading(ShadingType::GOURAUD);
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Phong##Shading", isPhongShading)) {
		store.setShading(ShadingType::PHONG);
	}
}

void openModelManipulationWindow(const char* const modelName, Scene& scene, GUIStore& store, int index, float* moveSpeed) {
	ImGui::Text("What would you like to do to %s?", modelName);
	showFocusButton(modelName, scene, store);
	showModelColoringGUI(scene, store, index);
	showModelScaleGUI(scene, store, store.isModelSymmetricScaled(index), index);
	showTranslationGUI(scene, store, index, moveSpeed);
	ImGui::Columns(3, "##Manipulation");
	showModelRotationGUI(scene, store, index);
	ImGui::NextColumn();
	showNormalGUI(scene, store, index);
	ImGui::NextColumn();
	showBoundingBoxGUI(scene, store, index);
	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::Columns(3, "##LightsSettings");
	//Ambient settings
	{
		ImGui::Text("Ambient Light:");
		glm::vec3 ambientReflection = store.getALightReflection(index);
		if (ImGui::ColorEdit3(stringIntConcatenate("Ambient##", index), (float*)&ambientReflection)) {
			store.setAlightReflection(index, ambientReflection);
		}
		//float ambientReflected = store.getALightReflection(index);
		//if (ImGui::SliderFloat(stringIntConcatenate("Reflected##AmbientReflection", index), &ambientReflected, 0.0f, 1.0f)) {
		//	store.setAlightReflection(index, ambientReflected);
		//}
	}
	ImGui::NextColumn();
	//Defuse settings
	{
		ImGui::Text("Diffuse Light:");
		//float diffuseIntensity = store.getDefuseReflectionIntinsety(index);
		//if (ImGui::SliderFloat(stringIntConcatenate("Intensity##DiffuseIntensity", index), &diffuseIntensity, 0.0f, 1.0f)) {
		//	store.setDefuseReflectionIntinsety(index, diffuseIntensity);
		//}
		//float diffuseLightReflection = store.getDLightReflection(index);
		//if (ImGui::SliderFloat(stringIntConcatenate("Reflected##DiffuseReflection", index), &diffuseLightReflection, 0.0f, 1.0f)) {
		//	store.setDlightReflection(index, diffuseLightReflection);
		//}
		glm::vec3 diffuseLightReflection = store.getDiffuseLightReflection(index);
		if (ImGui::ColorEdit3(stringIntConcatenate("Diffuse##", index), (float*)&diffuseLightReflection)) {
			store.setDlightReflection(index, diffuseLightReflection);
		}
	}
	ImGui::NextColumn();
	//Specular settings
	{
		ImGui::Text("Specular:");
		//float defuse = store.getDefuseReflectionIntinsety(index);
		//if (ImGui::SliderFloat(stringIntConcatenate("Intensity##SpecularIntensity", index), &defuse, 0.0f, 1.0f)) {
		//	store.setDefuseReflectionIntinsety(index, defuse);
		//}
		float shine = store.getShine(index);
		if (ImGui::SliderFloat(stringIntConcatenate("Shine##SpecularShine", index), &shine, 0.0f, 1.0f)) {
			store.setShine(index, shine);
		}
		glm::vec3 specularLightReflection = store.getSLightReflection(index);
		if (ImGui::ColorEdit3(stringIntConcatenate("Specular##", index), (float*)&specularLightReflection)) {
			store.setSlightReflection(index, specularLightReflection);
		}/*
		float specularLightReflection = store.getSLightReflection(index);
		if (ImGui::SliderFloat(stringIntConcatenate("Reflected##LightReflection", index), &specularLightReflection, 0.0f, 1.0f)) {
			store.setSlightReflection(index, specularLightReflection);
		}*/
	}
	ImGui::Columns(1);
}

void showFocusButton(const char* const modelName, Scene& scene, GUIStore& store) {
	if (ImGui::Button("Focus on me ;)")) {
		std::vector<std::shared_ptr<MeshModel>> models = scene.getSceneModels();
		MeshModel model = *models[scene.GetActiveModelIndex()];
		glm::vec3 min = model.getMinBoundingBoxVec();
		glm::vec3 max = model.getMaxBoundingBoxVec();
		float xAvg = (min.x + max.x) / 2.0f;
		float yAvg = (min.y + max.y) / 2.0f;
		float zAvg = (min.z + max.z) / 2.0f;
		scene.setCameraVectors(glm::vec4(xAvg, yAvg, 5.0, 1), glm::vec4(xAvg, yAvg, zAvg, 1.0f), scene.getActiveCamera().getUpVector(), scene.GetActiveCameraIndex());
	}
}

void showCamerasListed(std::vector<Camera>& cameras, Scene& scene, GUIStore& store) {
	ImGui::Text("Please select the camera you wish to manipulate:");
	Camera acCamera = scene.getActiveCamera();
	for each (Camera cam in cameras)
	{
		char* name = stringToCharSeq("camera" + std::to_string(cam.getIndex()));
		int camIndex = cam.getIndex();
		bool isSelected = store.isCameraManipulated(camIndex);
		const char* nameInString = stringToCharSeq("settings of camera" + std::to_string(cam.getIndex()));
		if (ImGui::Checkbox(name, &isSelected)) {
			int oldActiveCam = scene.GetActiveCameraIndex();
			store.setCameraManipulated(oldActiveCam, false);
			store.setCameraManipulated(camIndex, true);
			scene.SetActiveCameraIndex(camIndex);
		}
		if (scene.getActiveCamera().getIndex() == camIndex) {
			ImGui::SameLine();
			if (ImGui::TreeNode(nameInString)) {
				glm::vec4 eye = cam.getEyeVector();
				glm::vec4 at = cam.getAtVector();
				glm::vec4 up = cam.getUpVector();
				//eye of the cams
				{
					ImGui::Columns(3, "mixed");
					ImGui::Text("position");
					ImGui::SameLine();
					static float x = 0;
					static float y = 0;
					static float z = 0;
					ImGui::InputFloat(stringToCharSeq("x##eye" + std::to_string(camIndex)), &eye.x, 1.0f, 0, "%.3f");
					ImGui::NextColumn();
					ImGui::InputFloat(stringToCharSeq("y##eye" + std::to_string(camIndex)), &eye.y, 1.0f, 0, "%.3f");
					ImGui::NextColumn();
					ImGui::InputFloat(stringToCharSeq("z##eye" + std::to_string(camIndex)), &eye.z, 1.0f, 0, "%.3f");
					ImGui::Columns(1);
				}
				// at of the cam
				{
					ImGui::Columns(3, "mixed");
					ImGui::Text("LookingAt");
					ImGui::SameLine();
					ImGui::InputFloat(stringToCharSeq("x##at" + std::to_string(camIndex)), &at.x, 1.0f, 0, "%.3f");
					ImGui::NextColumn();
					ImGui::InputFloat(stringToCharSeq("y##at" + std::to_string(camIndex)), &at.y, 1.0f, 0, "%.3f");
					ImGui::NextColumn();
					ImGui::InputFloat(stringToCharSeq("z##at" + std::to_string(camIndex)), &at.z, 1.0f, 0, "%.3f");
					ImGui::Columns(1);
				}

				//up of the cam
				{
					ImGui::Columns(3, "mixed");

					ImGui::Text("UpOfCam");
					ImGui::SameLine();
					ImGui::InputFloat(stringToCharSeq("x##up" + std::to_string(camIndex)), &up.x, 1.0f, 0, "%.3f");
					ImGui::NextColumn();
					ImGui::InputFloat(stringToCharSeq("y##up" + std::to_string(camIndex)), &up.y, 1.0f, 0, "%.3f");
					ImGui::NextColumn();
					ImGui::InputFloat(stringToCharSeq("z##up" + std::to_string(camIndex)), &up.z, 1.0f, 0, "%.3f");
					ImGui::Columns(1);
					ImGui::Separator();
				}

				scene.setCameraVectors(eye, at, up, camIndex);
				ImGui::TreePop();
				ImGui::SameLine();


				//projection stuff
				{
					Mode mode = store.getProjModeForCam(camIndex);
					ImGui::Text("Please select a projection type...");
					if (ImGui::RadioButton("Orthographic", mode == Mode::Orthographic)) {
						store.setCamsProjMode(camIndex, Mode::Orthographic);
					}
					ImGui::SameLine();
					if (ImGui::RadioButton("Perspective", mode == Mode::Perspective)) {
						store.setCamsProjMode(camIndex, Mode::Perspective);
					}
					if (mode == Mode::Orthographic) {
						float projNear;
						float projFar;
						float projTop;
						float projBottom;
						float projLeft;
						float projRight;
						cam.getOrthographicProjParameters(&projNear, &projFar, &projTop, &projBottom, &projLeft, &projRight);
						ImGui::Columns(6, "mixed");
						ImGui::InputFloat(stringToCharSeq("Near##" + std::to_string(camIndex)), &projNear, 1.0f, 0, "%.1f");
						ImGui::NextColumn();
						ImGui::InputFloat(stringToCharSeq("Far##" + std::to_string(camIndex)), &projFar, 1.0f, 0, "%.1f");
						ImGui::NextColumn();
						ImGui::InputFloat(stringToCharSeq("Top##" + std::to_string(camIndex)), &projTop, 1.0f, 0, "%.1f");
						ImGui::NextColumn();
						ImGui::InputFloat(stringToCharSeq("Bottom##" + std::to_string(camIndex)), &projBottom, 1.0f, 0, "%.1f");
						ImGui::NextColumn();
						ImGui::InputFloat(stringToCharSeq("Left##" + std::to_string(camIndex)), &projLeft, 1.0f, 0, "%.1f");
						ImGui::NextColumn();
						ImGui::InputFloat(stringToCharSeq("Right##" + std::to_string(camIndex)), &projRight, 1.0f, 0, "%.1f");
						ImGui::Columns(1);
						ImGui::Separator();
						scene.setOrthographicParameters(projTop, projBottom, projRight, projLeft, projNear, projFar, camIndex);
					}
					else {
						float projNear;
						float projFar;
						float projFovy;
						float projAspectRatio;
						cam.getPerspectiveProjParameters(&projNear, &projFar, &projFovy, &projAspectRatio);
						ImGui::Columns(4, "mixed");
						ImGui::InputFloat(stringToCharSeq("Near##" + std::to_string(camIndex)), &projNear, 1.0f, 0, "%.1f");
						ImGui::NextColumn();
						ImGui::InputFloat(stringToCharSeq("Far##" + std::to_string(camIndex)), &projFar, 1.0f, 0, "%.1f");
						ImGui::NextColumn();
						ImGui::InputFloat(stringToCharSeq("Angel##" + std::to_string(camIndex)), &projFovy, 1.0f, 0, "%.1f");
						ImGui::NextColumn();
						ImGui::InputFloat(stringToCharSeq("width##" + std::to_string(camIndex)), &projAspectRatio, 1.0f, 0, "%.1f");
						ImGui::Columns(1);
						ImGui::Separator();
						scene.setPerspectiveParameters(projNear, projFar, projFovy, projAspectRatio, camIndex);
					}
					float zoom = cam.getZoom();
					ImGui::Columns(2, "mixed");
					if (ImGui::InputFloat(stringToCharSeq("Zoom##" + std::to_string(camIndex)), &zoom, 0.5f, 0, "%.2f")) {
						scene.setZoomForCam(zoom, camIndex);
					}
					ImGui::Columns(1);
					ImGui::Separator();

					if (ImGui::Button("Look at ORIGIN")) {

						glm::vec4 eye = cam.getOrigEye();
						glm::vec4 at = cam.getOrigAt();
						glm::vec4 up = cam.getOrigUp();
						scene.setCameraVectors(eye, at, up, camIndex);
						continue;
					}
					ImGui::SameLine();
					if (ImGui::Button("Reset projection")) {
						if (mode == Mode::Orthographic) {
							scene.setOrthographicParameters(1.0f, -1.0f, 1.5f, -1.5, 1.0f, 10.0f, camIndex);
						}
						else
						{
							scene.setPerspectiveParameters(1.0f, 10.0f, 90.0f, 1.5f, camIndex);
						}
					}
				}
			}

		}
	}
}




void showModelsListed(std::vector<std::shared_ptr<MeshModel>> models, Scene& scene, GUIStore& store, ImGuiIO& io)
{
	int i = 0;
	if (models.size() == 0) {
		ImGui::Text("** Here You should see the list of models you loaded. **");
		ImGui::Text("\t It appears you have not yet loaded any models.");
		ImGui::Text("\t -> To load a model, please use the menu at the top of the window.");
		return;
	}
	ImGui::Text("Please select the model(s) you wish to manipulate:");
	ImGui::Text("To move your models around, firstly select them then use the keyboard arrows:\n UP,DOWN,LEFT,RIGHT, SHIFT+UP, SHIFT+DOWN.");
	for each (std::shared_ptr<MeshModel> model in models)
	{
		char* name = stringToCharSeq(model->GetModelName());
		bool isSelected = store.isModelSelected(i);
		if (ImGui::Checkbox(name, &isSelected)) {
			store.setModelManipulated(i, isSelected);
		}
		if (isSelected) {
			scene.SetActiveModelIndex(i);
			float moveSpeed = store.getModelSpeed(i);
			openModelManipulationWindow(name, scene, store, i, &moveSpeed);
			handleTranslationFromKeyboardInput(name, scene, store, io, moveSpeed);
		}
		++i;
	}
}

void showAddLightGUI(Scene& scene) {
	if (ImGui::Button("+Add Point Light Source##AddLightningPoint")) {
		LightPointSource light;
		scene.AddLight(std::make_shared<LightPointSource>(light));
	}
	ImGui::SameLine();
	if (ImGui::Button("+Add Parallel Light Source##AddLightningParallel")) {
		LightParallelSource light;
		scene.AddLight(std::make_shared<LightParallelSource>(light));
	}
}

std::string generateLightName(const Light& light, int index) {
	switch (light.getLightType()) {
	case LightType::PARALLEL: {
		return stringIntConcatenate("Parallel", index);
	}
							  break;
	case LightType::POINT: {
		return stringIntConcatenate("Point", index);
	}
						   break;
	default: {
		return "";
	}
	}
}


void showLightManipulationGUI(Scene& scene, GUIStore& store, ImGuiIO& io) {
	std::vector<std::shared_ptr<Light>> lights = scene.getLights();
	for each (std::shared_ptr<Light> light in lights)
	{
		int index = light->getLightIndex();
		std::string lightName = generateLightName(*light, index);
		char* name = stringToCharSeq(lightName);
		bool isSelected = store.isLightActive(index);
		if (ImGui::Checkbox(name, &isSelected)) {
			store.setActiveLight(index);
		}
		if (isSelected) {
			scene.SetActiveLight(index);
			//glm::vec3 newColor = light->getLightColor();
			//if (ImGui::ColorEdit3(stringIntConcatenate("Color##", index), (float*)&newColor)) {
			//	store.setLightColor(index, newColor);
			//	light->setLightColor(newColor);
			//}
			ImGui::Columns(3, "##LightsSettings");
			//Ambient settings
			{
				ImGui::Text("Ambient Light:");
				glm::vec3 ambientReflection = light->getLightAmbientColor();
				if (ImGui::ColorEdit3(stringIntConcatenate("Ambient##", index), (float*)&ambientReflection)) {
					light->setLightAmbientColor(ambientReflection);
					store.setAmbientReflectionIntinsety(index, ambientReflection);
				}
			}
			ImGui::NextColumn();
			//Defuse settings
			{
				ImGui::Text("Diffuse Light:");
				glm::vec3 diffuseLightReflection = light->getLightDiffuseColor();
				if (ImGui::ColorEdit3(stringIntConcatenate("Diffuse##", index), (float*)&diffuseLightReflection)) {
					light->setLightDiffuseColor(diffuseLightReflection);
					store.setDefuseReflectionIntinsety(index, diffuseLightReflection);
				}
			}
			ImGui::NextColumn();
			//Specular settings
			{
				ImGui::Text("Specular:");
				glm::vec3 specularLightReflection = light->getLightSpecularColor();
				if (ImGui::ColorEdit3(stringIntConcatenate("Specular##", index), (float*)&specularLightReflection)) {
					light->setLightSpecularColor(specularLightReflection);
					store.setSpecularReflectionIntinsety(index, specularLightReflection);
				}
			}
			ImGui::Columns(1);

			if (light->getLightType() == LightType::POINT) {
				float moveSpeed = store.getLightSpeed(index);
				showLightScaleGUI(scene, store, store.isLightSymmetricScaled(index), index);
				showLightRotationGUI(scene, store, index);
				handleTranslationFromKeyboardInput(name, scene, store, io, moveSpeed, true);
			}
			else if (light->getLightType() == LightType::PARALLEL) {
				glm::vec3 actualDirection = store.getActualLightDirection(index);
				glm::vec3 userDirection = store.getUserSettingLightDirection(index);
				float x = userDirection.x, y = userDirection.y, z = userDirection.z;
				float actualX = actualDirection.x, actualY = actualDirection.y, actualZ = actualDirection.z;
				ImGui::Text("Current Direction (Normalized): ");
				ImGui::Text(stringToCharSeq("X: " + std::to_string(actualX)));
				ImGui::SameLine();
				ImGui::Text(stringToCharSeq("Y: " + std::to_string(actualY)));
				ImGui::SameLine();
				ImGui::Text(stringToCharSeq("Z: " + std::to_string(actualZ)));

				ImGui::InputFloat(stringIntConcatenate("X##", index), &x);
				ImGui::InputFloat(stringIntConcatenate("Y##", index), &y);
				ImGui::InputFloat(stringIntConcatenate("Z##", index), &z);

				store.updateUserSettingLightDirection(index, x, y, z);

				if (ImGui::Button(stringIntConcatenate("Set New Direction##", index))) {
					glm::vec3 normalizedDirection = glm::normalize(glm::vec3(x, y, z));
					float a = normalizedDirection.x;
					float b = normalizedDirection.y;
					float c = normalizedDirection.z;
					store.updateActualLightDirection(index, a, b, c);
					scene.updateLightDirection(index, a, b, c);
				}
			}
		}
	}
}

void showLightningGUI(Scene& scene, GUIStore& store, ImGuiIO& io)
{
	showAddLightGUI(scene);
	showLightManipulationGUI(scene, store, io);
}

void showBoundingBoxGUI(Scene& scene, GUIStore& store, int index)
{
	bool isBoundingBoxDisplayed = store.isModelBoundingBoxOn(index);
	if (ImGui::Checkbox(stringIntConcatenate("Bounding Box##ON", index), &isBoundingBoxDisplayed)) {
		store.setModelBoundingBox(index, isBoundingBoxDisplayed);
	}
}

void showAmbientLightGUI(GUIStore& store)
{
	glm::vec3 newColor = store.getAmbientLightColor();
	float intensity = store.getAmbientLightIntensity();
	ImGui::Text("Ambient Light:");
	ImGui::SameLine();
	if (ImGui::ColorEdit3("##AmbientLight", (float*)&newColor)) {
		store.setAmbientLightColor(newColor);
	}

	ImGui::Text("Intensity:");
	ImGui::SameLine();
	if (ImGui::SliderFloat("##AmbientIntensity", &intensity, 0.0f, 1.0f)) {
		store.setAmbientLightIntensity(intensity);
	}
}

void GenerateGUI(ImGuiIO& io, Scene& scene, GUIStore& store)
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (showDemoWindow)
	//{
	//ImGui::ShowDemoWindow(&showDemoWindow);
	//}

	std::vector<std::shared_ptr<MeshModel>> models = scene.getSceneModels();
	std::vector<Camera> cameras = scene.GetCameras();
	{
		ImGui::Begin("All Models");
		bool shouldFog = store.getFog();
		if (ImGui::Checkbox("Let's add some fog :O", &shouldFog)) {
			store.setFog(shouldFog);
		}

		if (shouldFog) {
			glm::vec3 newColor = store.getFogColor();
			float fogDensity = store.getFogDensity();
			if (ImGui::ColorEdit3("FogColor##FogColor", (float*)&newColor)) {
				store.setFogColor(newColor);
			}
			if (ImGui::SliderFloat("Fog Density", &fogDensity, 0.0f, 3.0f)) {
				store.setFogDensity(fogDensity);
			}
		}
		bool shouldAntiAlias = store.isAntiAliased();
		if (ImGui::Checkbox("Check me for supersampling anti-aliasing", &shouldAntiAlias)) {
			store.setAntiAlias(shouldAntiAlias);
		}
		bool nonUniform = store.isNonUniformMaterial();
		if (ImGui::Checkbox("Wanna add a non-uniform material XD? CLICK ME", &nonUniform)) {
			store.setNonUniformMaterial(nonUniform);
		}
		showShadingGUI(scene, store);
		if (ImGui::CollapsingHeader("Models")) {
			// List All Loaded Mesh Models
			showModelsListed(models, scene, store, io);
		}

		if (ImGui::CollapsingHeader("Cameras")) {
			// List All Loaded Cameras
			showCamerasListed(cameras, scene, store);
		}
		ImGui::Begin("Lights Menu");
		//showAmbientLightGUI(store);
		if (ImGui::CollapsingHeader("Lights")) {
			// List All Lights
			showLightningGUI(scene, store, io);
		}
		ImGui::End();

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::End();
	}


	// 4. Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load Model...", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						scene.AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel(outPath)));
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
	// 5. Demonstrate creating a new camera
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					glm::vec4 newEye = scene.getActiveCamera().getEyeVector();
					newEye.z = -newEye.z;
					glm::vec4 newAt = scene.getActiveCamera().getAtVector();
					Camera mainCam = Camera(
						newEye,
						newAt,
						glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)
					);
					scene.AddCamera(mainCam);
					//scene.AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel("D:\\Git\\project-the-hurricane-troupe\\Data\\camera.obj")));

				}
				if (ImGui::BeginMenu("Basic Models"))
				{
					if (ImGui::MenuItem("Pyramid"))
					{
						scene.AddModel(std::make_shared<MeshModel>(PrimMeshModel()));
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
	//// 6. Demonstrate help
	//{
	//	if (ImGui::BeginMainMenuBar())
	//	{
	//		if (ImGui::BeginMenu("Help"))
	//		{
	//			//ImGui::ShowDemoWindow(&showDemoWindow);
	//			ImGui::EndMenu();
	//		}
	//		ImGui::EndMainMenuBar();
	//	}
	//}
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &showAnotherWindow);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (showAnotherWindow)
	{
		ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
		{
			showAnotherWindow = false;
		}
		ImGui::End();
	}

	// 4. Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load Model...", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						scene.AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel(outPath)));
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}

GUIStore::GUIStore(const Scene & scene) :
	_scene(scene),
	_models(scene.getSceneModels()),
	_isModelBeingManipulated(scene.GetModelCount(), false),
	_isModelSymmetricScaled(scene.GetModelCount(), true),
	_whichNormals(scene.GetModelCount(), Utils::Normals::NONE),
	_modelNormalLength(scene.GetModelCount(), INITIALMODELNORMALLENGTH),
	_modelCount(scene.GetModelCount()),
	_modelSpeed(scene.GetModelCount(), INITIALMODELSPEED),
	_isModelBoundingBoxOn(scene.GetModelCount(), false),
	_modelColor(scene.GetModelCount(), INITIALMESHMODELCOLOR),
	_modelRotationType(scene.GetModelCount(), RotationType::MODEL),
	projModeForCams(scene.GetCameraCount(), Mode::Perspective),
	cameraCount(scene.GetCameraCount()),
	_isCameraBeingManipulated(scene.GetCameraCount(), false),
	_shading(INITIALSHADING),
	fog(false),
	fogColor(GetClearColor()),
	fogDensity(1.0f),
	_antiAliased(false),
	_activeLightIndex(ERRORINDEX),
	_lightsCount(scene.getLightCount()),
	_isLightActive(scene.getLightCount(), false),
	_lightsColors(scene.getLightCount(), INITIALLIGHTCOLOR),
	_lightSpeeds(scene.getLightCount(), INITIALMODELSPEED),
	_lightRotationType(scene.getLightCount(), RotationType::MODEL),
	_isLightSymmetricScaled(scene.getLightCount(), true),
	_ambientLightColor(INITIALAMBIENTLIGHTCOLOR),
	_ambientLightIntensity(INITIALAMBIENTLIGHTINTENSITY),
	_Lights_actualDirection_UserDirection(scene.getLightCount()),
	ambientReflectionIntinsety(scene.GetModelCount(), glm::vec3(1.0f,1.0f,1.0f)),
	defuseReflectionIntinsety(scene.GetModelCount(), glm::vec3(1.0f, 1.0f, 1.0f)),
	specularReflectionIntinsety(scene.GetModelCount(), glm::vec3(1.0f, 1.0f, 1.0f)),
	shine(scene.GetModelCount(), 0.5f),
	aLightReflect(scene.GetModelCount(), glm::vec3(1.0f, 1.0f, 1.0f)),
	dLightReflect(scene.GetModelCount(), glm::vec3(1.0f, 1.0f, 1.0f)),
	sLightReflect(scene.GetModelCount(), glm::vec3(1.0f, 1.0f, 1.0f)),
	nonUniformMaterial(false)
{
}

void GUIStore::sync(const Scene& scene)
{
	_scene = Scene(scene);
	_models = scene.getSceneModels(); //Models can only be added so far... if they are removed more work is needed.
	int newSize = _models.size();
	if (newSize > _modelCount) {
		_isModelBeingManipulated.push_back(false);
		_isModelSymmetricScaled.push_back(true);
		_modelSpeed.push_back(INITIALMODELSPEED);
		_whichNormals.push_back(Utils::Normals::NONE);
		_modelNormalLength.push_back(INITIALMODELNORMALLENGTH);
		_isModelBoundingBoxOn.push_back(false);
		_modelColor.push_back(INITIALMESHMODELCOLOR);
		_modelRotationType.push_back(RotationType::MODEL);
		ambientReflectionIntinsety.push_back(glm::vec3(1.0f,1.0f,1.0f));
		defuseReflectionIntinsety.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		specularReflectionIntinsety.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		shine.push_back(0.5f);
		aLightReflect.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		dLightReflect.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		sLightReflect.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	}
	_modelCount = newSize;


	int camCount = scene.GetCameras().size();
	if (camCount > cameraCount) {
		_isCameraBeingManipulated.push_back(false);
		projModeForCams.push_back(Mode::Perspective);
	}
	cameraCount = camCount;

	int lightCount = scene.getLightCount();
	if (lightCount > _lightsCount) {
		if (_activeLightIndex == ERRORINDEX)
			_activeLightIndex = 0;
		_isLightActive.push_back(false);
		_lightsColors.push_back(INITIALLIGHTCOLOR);
		_lightSpeeds.push_back(INITIALMODELSPEED);
		_lightRotationType.push_back(RotationType::MODEL);
		_isLightSymmetricScaled.push_back(true);
		_Lights_actualDirection_UserDirection.push_back(std::make_pair(scene.getLights().back()->getDirection(), scene.getLights().back()->getDirection()));
	}
	_lightsCount = lightCount;
}

void GUIStore::setModelManipulated(int i, bool isManipulated)
{
	if (i < 0 || i >= _modelCount) return;
	_isModelBeingManipulated[i] = isManipulated;
}

bool GUIStore::isModelSelected(int i) const
{
	if (i < 0 || i >= _modelCount) return false;
	return _isModelBeingManipulated[i];
}

void GUIStore::setModelSymmetricScaled(int i, bool isSymmetric)
{
	if (i < 0 || i >= _modelCount) return;
	_isModelSymmetricScaled[i] = isSymmetric;
}

bool GUIStore::isModelSymmetricScaled(int i) const
{
	if (i < 0 || i >= _modelCount) return false;
	return _isModelSymmetricScaled[i];
}

void GUIStore::setModelSpeed(int i, float newSpeed)
{
	if (i < 0 || i >= _modelCount) return;
	_modelSpeed[i] = newSpeed;
}

float GUIStore::getModelSpeed(int i) const
{
	if (i < 0 || i >= _modelCount) return ERRORSPEED;
	return _modelSpeed[i];
}

void GUIStore::setModelBoundingBox(int i, bool on)
{
	if (i < 0 || i >= _modelCount) return;
	_isModelBoundingBoxOn[i] = on;
}

bool GUIStore::isModelBoundingBoxOn(int i) const
{
	if (i < 0 || i >= _modelCount) return false;
	return _isModelBoundingBoxOn[i];
}

void GUIStore::setModelNormalLength(int i, float length)
{
	if (i < 0 || i >= _modelCount) return;
	_modelNormalLength[i] = length;
}

float GUIStore::getModelNormalLength(int i) const
{
	if (i < 0 || i >= _modelCount) return -1.0f;
	return _modelNormalLength[i];
}

glm::vec3 GUIStore::getModelColor(int i) const
{
	if (i < 0 || i >= _modelCount) return INITIALMESHMODELCOLOR;
	return _modelColor[i];
}

void GUIStore::setModelColor(int i, const glm::vec3& color)
{
	if (i < 0 || i >= _modelCount) return;
	_modelColor[i] = color;
}

bool GUIStore::isModelRotationAroundModel(int i) const
{
	if (i < 0 || i >= _modelCount) return false;
	return _modelRotationType[i] == RotationType::MODEL;
}

bool GUIStore::isModelRotationAroundWorld(int i) const
{
	if (i < 0 || i >= _modelCount) return false;
	return _modelRotationType[i] == RotationType::WORLD;
}

void GUIStore::setModelRotationAround(int i, const RotationType& rotType)
{
	if (i < 0 || i >= _modelCount) return;
	_modelRotationType[i] = rotType;
}

ShadingType GUIStore::getShading() const
{
	return _shading;
}

void GUIStore::setShading(const ShadingType& type)
{
	_shading = type;
}

void GUIStore::setFog(bool _bool)
{
	fog = _bool;
}

bool GUIStore::getFog() const
{
	return fog;
}

glm::vec3 GUIStore::getFogColor() const
{
	return fogColor;
}

void GUIStore::setFogColor(glm::vec3 _fogColor)
{
	fogColor = _fogColor;
}

float GUIStore::getFogDensity() const
{
	return fogDensity;
}

void GUIStore::setFogDensity(float density)
{
	fogDensity = density;
}

bool GUIStore::isAntiAliased() const
{
	return _antiAliased;
}

void GUIStore::setAntiAlias(bool isAliased)
{
	_antiAliased = isAliased;
}

void GUIStore::setActiveLight(int index)
{
	if (index < 0 || index >= _lightsCount) return;
	_isLightActive[_activeLightIndex] = false;
	_isLightActive[index] = true;
	_activeLightIndex = index;
}

bool GUIStore::isLightActive(int index) const
{
	if (index < 0 || index > _lightsCount) return false;
	return _activeLightIndex == index;
}

glm::vec3 GUIStore::getLightColor(int index) const
{
	if (index < 0 || index > _lightsCount) return INITIALLIGHTCOLOR;
	return _lightsColors[index];
}

void GUIStore::setLightColor(int index, const glm::vec3& color)
{

	if (index < 0 || index > _lightsCount) return;
	_lightsColors[index] = color;
}

float GUIStore::getLightSpeed(int index) const
{
	if (index < 0 || index > _lightsCount) return ERRORSPEED;
	return _lightSpeeds[index];
}

void GUIStore::setLightSpeed(int index, const float speed)
{
	if (index < 0 || index > _lightsCount) return;
	_lightSpeeds[index] = speed;
}

bool GUIStore::isLightRotationAroundModel(int index) const
{
	if (index < 0 || index >= _lightsCount) return false;
	return _lightRotationType[index] == RotationType::MODEL;
}

bool GUIStore::isLightRotationAroundWorld(int index) const
{
	if (index < 0 || index >= _lightsCount) return false;
	return _lightRotationType[index] == RotationType::WORLD;
}

void GUIStore::setLightRotationAround(int index, const RotationType & type)
{
	if (index < 0 || index >= _lightsCount) return;
	_lightRotationType[index] = type;
}

bool GUIStore::isLightSymmetricScaled(int index) const
{
	if (index < 0 || index >= _lightsCount) return false;
	return _isLightSymmetricScaled[index];
}

void GUIStore::setLightSymmetricScaled(int index, bool isSymmetric)
{
	if (index < 0 || index >= _lightsCount) return;
	_isLightSymmetricScaled[index] = isSymmetric;
}

glm::vec3 GUIStore::getAmbientLightColor() const
{
	return _ambientLightColor;
}

void GUIStore::setAmbientLightColor(const glm::vec3& color)
{
	if (Utils::isColorLegal(color) == false) return;
	_ambientLightColor = color;
}

float GUIStore::getAmbientLightIntensity() const
{
	return _ambientLightIntensity;
}

void GUIStore::setAmbientLightIntensity(const float intensity)
{
	if (intensity < 0.0f || intensity > 1.0f) return;
	_ambientLightIntensity = intensity;
}

void GUIStore::updateUserSettingLightDirection(int index, float x, float y, float z)
{
	if (index < 0 || index >= _lightsCount) return;
	_Lights_actualDirection_UserDirection[index].second = glm::vec3(x, y, z);
}

glm::vec3 GUIStore::getUserSettingLightDirection(int index) const
{
	if (index < 0 || index >= _lightsCount) return ERRORDIRECTION;
	return _Lights_actualDirection_UserDirection[index].second;
}

void GUIStore::updateActualLightDirection(int index, float x, float y, float z)
{
	if (index < 0 || index >= _lightsCount) return;
	_Lights_actualDirection_UserDirection[index].first = glm::vec3(x, y, z);
}

glm::vec3 GUIStore::getActualLightDirection(int index) const
{
	if (index < 0 || index >= _lightsCount) return ERRORDIRECTION;
	return _Lights_actualDirection_UserDirection[index].first;
}


void GUIStore::setCamsProjMode(int i, Mode mode)
{
	projModeForCams[i] = mode;
}

Mode GUIStore::getProjModeForCam(int i) const
{
	return projModeForCams[i];
}

void GUIStore::setCameraManipulated(int i, bool isManipulated)
{
	_isCameraBeingManipulated[i] = isManipulated;
}

bool GUIStore::isCameraManipulated(int i) const
{
	return _isCameraBeingManipulated[i];
}

glm::vec3 GUIStore::getAmbientReflectionIntinsety(int i) const
{
	if (i < 0 || i >= _modelCount) return ERRORREFLECTION;
	return ambientReflectionIntinsety[i];
}

void GUIStore::setAmbientReflectionIntinsety(int i, glm::vec3 color)
{
	if (i < 0 || i >= _modelCount) return;
	ambientReflectionIntinsety[i] = color;
}

glm::vec3 GUIStore::getDiffuseReflectionIntensity(int i) const
{
	if (i < 0 || i >= _modelCount) return ERRORREFLECTION;
	return defuseReflectionIntinsety[i];
}

void GUIStore::setDefuseReflectionIntinsety(int i, glm::vec3 color)
{
	if (i < 0 || i >= _modelCount) return;
	defuseReflectionIntinsety[i] = color;
}

glm::vec3 GUIStore::getSpecularReflectionIntensity(int i) const
{
	if (i < 0 || i >= _modelCount) return ERRORREFLECTION;
	return specularReflectionIntinsety[i];
}

void GUIStore::setSpecularReflectionIntinsety(int i, glm::vec3 color)
{
	if (i < 0 || i >= _modelCount) return;
	specularReflectionIntinsety[i] = color;
}

float GUIStore::getShine(int i) const
{
	if (i < 0 || i >= _modelCount) return -1.0f;
	return shine[i];
}

void GUIStore::setShine(int i, float number)
{
	if (i < 0 || i >= _modelCount) return;
	shine[i] = number;
}

glm::vec3 GUIStore::getALightReflection(int i) const
{
	if (i < 0 || i >= _modelCount) return ERRORREFLECTION;
	return aLightReflect[i];
}

void GUIStore::setAlightReflection(int i, glm::vec3 color)
{
	if (i < 0 || i >= _modelCount) return;
	aLightReflect[i] = color;
}

glm::vec3 GUIStore::getDiffuseLightReflection(int i) const
{
	if (i < 0 || i >= _modelCount) return ERRORREFLECTION;
	return dLightReflect[i];
}

void GUIStore::setDlightReflection(int i, glm::vec3 color)
{
	if (i < 0 || i >= _modelCount) return;
	dLightReflect[i] = color;
}

glm::vec3 GUIStore::getSLightReflection(int i) const
{
	if (i < 0 || i >= _modelCount) return ERRORREFLECTION;
	return sLightReflect[i];
}

void GUIStore::setSlightReflection(int i, glm::vec3 color)
{
	if (i < 0 || i >= _modelCount) return;
	sLightReflect[i] = color;
}

bool GUIStore::isNonUniformMaterial() const
{
	return nonUniformMaterial;
}

void GUIStore::setNonUniformMaterial(bool _bool)
{
	nonUniformMaterial = _bool;
}

void GUIStore::setModelNormal(int i, Utils::Normals newNormal)
{
	if (i < 0 || i >= _modelCount) return;
	_whichNormals[i] = newNormal;
}

Utils::Normals GUIStore::getModelNormalStatus(int i) const
{
	if (i < 0 || i >= _modelCount) return Utils::Normals::NONE;
	return _whichNormals[i];
}
