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
	int len = (int)str.length()+1;
	char** seq = new char*[1];
	seq[0] = new char[len];
	for (int i = 0; i < len; ++i)
	{
		seq[0][i] = str[i];
	}
	return seq[0];
}

char* stringIntConcatenate(char* str, int i) { return stringToCharSeq(std::string(str) + std::to_string(i)); }

void showScaleSliders(Scene& scene, bool isSymmetric, int index) {
	float* xFactor = new float(0.0f);
	float* yFactor = new float(0.0f);
	float* zFactor = new float(0.0f);
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

void handleTranslationFromKeyboardInput(const char* const modelName, Scene& scene, GUIStore& store, ImGuiIO& io, float moveStrength) {
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
	else if (ImGui::IsKeyDown(left)) { xAddition -= moveStrength; changed = true;}
	else if (ImGui::IsKeyDown(right)) {
		xAddition += moveStrength; 
		changed = true;
	}
	if (changed) {
		scene.moveActiveModel(xAddition, yAddition, zAddition);
	}
}


void openModelManipulationWindow(const char* const modelName, Scene& scene, GUIStore& store, int index, float* moveSpeed) {
	ImGui::Text("What would you like to do to %s?", modelName);
	ImGui::Text("Scale:");
	ImGui::SameLine();
	char* tmp = stringToCharSeq(std::string("Altogether##") + std::to_string(index));
	if (ImGui::RadioButton(tmp, store.isModelSymmetricScaled(index))) store.setModelSymmetricScaled(index, true);
	ImGui::SameLine();
	if (ImGui::RadioButton(stringToCharSeq(std::string("Seperate##") + std::to_string(index)), !store.isModelSymmetricScaled(index))) store.setModelSymmetricScaled(index, false);
	showScaleSliders(scene, store.isModelSymmetricScaled(index), index);
	if (ImGui::SliderFloat(stringIntConcatenate("moveSpeed##Speed", index), moveSpeed, 0.0f, 30.0f)) { 
		store.setModelSpeed(index, *moveSpeed);
	}
	ImGui::Separator();
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
		bool isSelected = store.isModelManipulated(i);
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

void ObjectManipulationMenus(ImGuiIO& io, Scene& scene, GUIStore& store)
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (showDemoWindow)
	//{
		ImGui::ShowDemoWindow(&showDemoWindow);
	//}

	std::vector<std::shared_ptr<MeshModel>> models = scene.getSceneModels();
	{
		ImGui::Begin("All Models");
		
		if (ImGui::CollapsingHeader("Models")) {
			// List All Loaded Mesh Models
			showModelsListed(models, scene, store, io);
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
	_modelCount(scene.GetModelCount()),
	_modelSpeed(scene.GetModelCount(), INITIALMODELSPEED)
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
	}
	_modelCount = newSize;
	}

void GUIStore::setModelManipulated(int i, bool isManipulated)
{
	if (i < 0 || i > _modelCount) return;
	_isModelBeingManipulated[i] = isManipulated;
}

bool GUIStore::isModelManipulated(int i) const
{
	if (i < 0 || i > _modelCount) return false;
	return _isModelBeingManipulated[i];
}

void GUIStore::setModelSymmetricScaled(int i, bool isSymmetric)
{
	if (i < 0 || i > _modelCount) return;
	_isModelSymmetricScaled[i] = isSymmetric;
}

bool GUIStore::isModelSymmetricScaled(int i) const
{
	if (i < 0 || i > _modelCount) return false;
	return _isModelSymmetricScaled[i];
}

void GUIStore::setModelSpeed(int i, float newSpeed)
{
	if (i < 0 || i > _modelCount) return;
	_modelSpeed[i] = newSpeed;
}

float GUIStore::getModelSpeed(int i) const
{
	if (i < 0 || i > _modelCount) return false;
	return _modelSpeed[i];
}
