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

void showScaleGUI(Scene& scene, GUIStore& store, bool isSymmetric, int index) {
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

void showTranslationGUI(Scene& scene, GUIStore& store, int index, float* moveSpeed) {
	if (ImGui::SliderFloat(stringIntConcatenate("move speed##Speed", index), moveSpeed, 0.0f, 30.0f)) {
		store.setModelSpeed(index, *moveSpeed);
	}
	ImGui::SameLine();
	if (ImGui::Button(stringIntConcatenate("Reset Position##ResetPosition", index))) {
		scene.resetPositionActiveModel();
	}
}

void showRotationGUI(Scene& scene, GUIStore& store, int index) {
	float xAngle = 0.0f;
	float yAngle = 0.0f;
	float zAngle = 0.0f;
	bool modifiedRotation = false;

	ImGui::Text("Rotate Controls:");
	ImGui::PushButtonRepeat(true);
	if (ImGui::ArrowButton(stringIntConcatenate("Z##RotateZMinus", index), ImGuiDir_Up)) { zAngle = -15.0f; modifiedRotation = true; }
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("X##RotateXPlus", index), ImGuiDir_Up)) { xAngle = 15.0f; modifiedRotation = true; }
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("Z##RotateZPlus", index), ImGuiDir_Up)) { zAngle = 15.0f; modifiedRotation = true; }

	if (ImGui::ArrowButton(stringIntConcatenate("Y##RotateYMinus", index), ImGuiDir_Left)) { yAngle = -15.0f; modifiedRotation = true; }
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("X##RotateXMinus", index), ImGuiDir_Down)) { xAngle = -15.0f; modifiedRotation = true; }
	ImGui::SameLine();
	if (ImGui::ArrowButton(stringIntConcatenate("Y##RotateYPlus", index), ImGuiDir_Right)) { yAngle = 15.0f; modifiedRotation = true; }
	if (ImGui::Button(stringIntConcatenate("Clear Rotation##ClearRotation", index))) { scene.resetRotationActiveModel(); }
	ImGui::PopButtonRepeat();
	
	if (modifiedRotation) {
		scene.rotateActiveModel(RotationRules(xAngle, yAngle, zAngle, AngleUnits::DEGREES));
	}
}

void showNormalGUI(Scene& scene, GUIStore& store, int index) {
	Utils::Normals whichNormal = store.getModelNormalStatus(index);
	float length = store.getModelNormalLength(index);
	bool isPerVertex = whichNormal == Utils::Normals::PerVERTEX;
	bool isPerFace = whichNormal == Utils::Normals::PerFACE;
	bool isNone = whichNormal == Utils::Normals::NONE;
	if (ImGui::Checkbox(stringIntConcatenate("Show Normals Per Vertex##VertexNormals", index), &isPerVertex)) {
		store.setModelNormal(index, Utils::Normals::PerVERTEX); }
	if (ImGui::Checkbox(stringIntConcatenate("Show Normals Per Face##FaceNormals", index), &isPerFace)) { store.setModelNormal(index, Utils::Normals::PerFACE); }
	if (ImGui::Checkbox(stringIntConcatenate("No Normals##FaceNormals", index), &isNone)) { store.setModelNormal(index, Utils::Normals::NONE); }
	if (isNone == false) {
		if (ImGui::SliderFloat(stringIntConcatenate("Length##NormalsLength", index), &length, 0.01f, 100.0f)) { store.setModelNormalLength(index, length); }
	}
}

void openModelManipulationWindow(const char* const modelName, Scene& scene, GUIStore& store, int index, float* moveSpeed) {
	ImGui::Text("What would you like to do to %s?", modelName);
	showScaleGUI(scene, store, store.isModelSymmetricScaled(index), index);
	showTranslationGUI(scene, store, index, moveSpeed);
	ImGui::Columns(3, "##Manipulation");
	showRotationGUI(scene, store, index);
	ImGui::NextColumn();
	showNormalGUI(scene, store, index);
	ImGui::NextColumn();
	showBoundingBoxGUI(scene, store, index);
	ImGui::Columns(1);
	ImGui::Separator();
}

void showCamerasListed(std::vector<Camera>& cameras, Scene& scene, GUIStore& store) {
	ImGui::Text("Please select the camera you wish to manipulate:");
	Camera acCamera = scene.getActiveCamera();
	for each (Camera cam in cameras)
	{
		//if (cam.getIndex() == acCamera.getIndex())
		//{
		//	isSelected = true;
		//}
		char* name = stringToCharSeq("camera" + std::to_string(cam.getIndex()));
		int camIndex = cam.getIndex();
		bool isSelected = store.isCameraManipulated(camIndex);
		//ImGui::SameLine();
		const char* nameInString = stringToCharSeq("settings of camera" + std::to_string(cam.getIndex()));
		if (ImGui::Checkbox(name, &isSelected)) {
			int oldActiveCam = scene.GetActiveCameraIndex();
			store.setCameraManipulated(oldActiveCam, false);
			store.setCameraManipulated(camIndex, true);
			scene.SetActiveCameraIndex(camIndex);
		}
		if(scene.getActiveCamera().getIndex()==camIndex){
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
					//ImGui::Separator();
					ImGui::Columns(3, "mixed");
					ImGui::Text("LookingAt");
					ImGui::SameLine();
					ImGui::InputFloat(stringToCharSeq("x##at" + std::to_string(camIndex)), &at.x, 1.0f, 0, "%.3f");
					ImGui::NextColumn();
					ImGui::InputFloat(stringToCharSeq("y##at" + std::to_string(camIndex)), &at.y, 1.0f, 0, "%.3f");
					ImGui::NextColumn();
					ImGui::InputFloat(stringToCharSeq("z##at" + std::to_string(camIndex)), &at.z, 1.0f, 0, "%.3f");
					ImGui::Columns(1);
					//ImGui::Separator();
				}

				//up of the cam
				{
					//ImGui::Separator();
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
					if (ImGui::RadioButton("Orthographic",  mode == Mode::Orthographic)) {
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
						cam.getOrthographicProjStuff(&projNear, &projFar, &projTop, &projBottom, &projLeft, &projRight);
						//ImGui::Separator();
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
						cam.getPerspectiveProjStuff(&projNear, &projFar, &projFovy, &projAspectRatio);
						//ImGui::Separator();
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

void showBoundingBoxGUI(Scene& scene, GUIStore& store, int index)
{
	bool isBoundingBoxDisplayed = store.isModelBoundingBoxOn(index);
	if (ImGui::Checkbox(stringIntConcatenate("Bounding Box##ON", index), &isBoundingBoxDisplayed)) {
		store.setModelBoundingBox(index, isBoundingBoxDisplayed);
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
		
		if (ImGui::CollapsingHeader("Models")) {
			// List All Loaded Mesh Models
			showModelsListed(models, scene, store, io);
		}
		if (ImGui::CollapsingHeader("Cameras")) {
			// List All Loaded Mesh Models
			showCamerasListed(cameras, scene, store);
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
	projModeForCams(scene.GetCameraCount(),Mode::Perspective),
	cameraCount(scene.GetCameraCount()),
	_isCameraBeingManipulated(scene.GetCameraCount(),false)
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
	}
	_modelCount = newSize;
	
	
	int camCount = scene.GetCameras().size();
	if (camCount > cameraCount) {
		_isCameraBeingManipulated.push_back(false);
		projModeForCams.push_back(Mode::Perspective);
	}
	cameraCount = camCount;
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
	if (i < 0 || i >= _modelCount) return false;
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
