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

void showModelsListed(std::vector<std::shared_ptr<MeshModel>> models, GUIStore& store)
{
	int i = 0;
	
	ImGui::Text("Please select the model you wish to manipulate");
	for each (std::shared_ptr<MeshModel> model in models)
	{
		char* name = stringToCharSeq(model->GetModelName());
		bool curr = store.isModelManipulated(i);
		if (ImGui::Checkbox(name, &curr)) {
			store.setModelManipulated(i, curr);
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
			showModelsListed(models, store);
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
	_modelCount(scene.GetModelCount())
{
}

void GUIStore::sync(const Scene& scene)
{
	_scene = Scene(scene);
	_models = scene.getSceneModels(); //Models can only be added so far... if they are removed more work is needed.
	int newSize = _models.size();
	if (newSize > _modelCount) {
		_isModelBeingManipulated.push_back(false);
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
