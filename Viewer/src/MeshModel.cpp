#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

void MeshModel::updateWorldTransformation() {
	if (isTransformUpdated == true) return;
	worldTransform = translateTransform * rotateTransform.getTransform() * scaleTransform;
	isTransformUpdated = true;
}

MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	vertices(vertices),
	faces(faces),
	//textures(textures),
	normals(normals)
{
	//initialization code only
	_translate(nullptr, nullptr, nullptr); //no translation
	_scale(1.0, 1.0, 1.0); //no scale
	rotateTransform = RotationMatrix(); //no rotation
	isTransformUpdated = false;
	updateWorldTransformation(); //set the transformation.
}

MeshModel::~MeshModel() {}


const glm::mat4x4& MeshModel::GetWorldTransformation() {
	updateWorldTransformation();
	return worldTransform;
}

void MeshModel::SetColor(const glm::vec4& color) {
	this->color = color;
}

const glm::vec4& MeshModel::GetColor() const {
	return color;
}

const std::string& MeshModel::GetModelName() {
	return modelName;
}

std::vector<glm::vec2> MeshModel::getTextures() {
	return textures;
}

std::vector<glm::vec3> MeshModel::getNormals() {
	return normals;
}

std::vector<glm::vec3> MeshModel::getVertices() {
	return vertices;
}

std::vector<Face> MeshModel::getFaces() {
	return faces;
}

void MeshModel::_scale(float xFactor, float yFactor, float zFactor) {
	scaleTransform = glm::mat4x4({
		xFactor,		0.0f,		0.0f,		0.0f,
		0.0f,		yFactor,		0.0f,		0.0f,
		0.0f,		0.0f,		zFactor,		0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	});
	isTransformUpdated = false;
}

void MeshModel::scale(float xFactor, float yFactor, float zFactor) {
	_scale(xFactor, yFactor, zFactor);
}

void MeshModel::symmetricScale(float factor) {
	_scale(factor, factor, factor);
}

//void MeshModel::setPosition(const float * const newX, const float * const newY, const float * const newZ)
//{
//	 move to origin
//	 set to newX,newY,newZ
//}

void MeshModel::move(const float * const xAddition, const float * const yAddition, const float * const zAddition)
{
	_translate(xAddition, yAddition, zAddition);
}

void MeshModel::symmetricMove(const float * const addition)
{
	move(addition, addition, addition);
}

void MeshModel::rotate(const std::set<PairOfAxisAngle>& axisAngleSet) {
	_rotate(axisAngleSet);
}



void MeshModel::_translate(const float * const xAddition, const float * const yAddition, const float * const zAddition)
{
	float x = 0, y = 0, z = 0;
	if (xAddition != nullptr) {
		x = *xAddition;
	}
	if (yAddition != nullptr) {
		y = *yAddition;
	}
	if (zAddition != nullptr) {
		z = *zAddition;
	}

	translateTransform = glm::transpose(glm::mat4x4({
		1,		0,		0,		x,
		0,		1,		0,		y,
		0,		0,		1,		z,
		0,		0,		0,		1
		}));
	isTransformUpdated = false;
}

void MeshModel::_rotate(const std::set<PairOfAxisAngle>& axisAngleSet)
{
	for each (PairOfAxisAngle info in axisAngleSet)
	{
		float angle = info.getAngle();
		switch (info.getAxis()) {
		case AxisType::XAXIS:
			rotateTransform.setXRotation(angle);
			break;
		case AxisType::YAXIS:
			rotateTransform.setYRotation(angle);
			break;
		case AxisType::ZAXIS:
			rotateTransform.setZRotation(angle);
			break;
		}
	}
	isTransformUpdated = false;
}
