#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

void MeshModel::updateWorldTransformation() {
	worldTransform = (rotateTransform.getTransform() * scaleTransform * translateTransform);
}

MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	vertices(vertices),
	faces(faces),
	//textures(textures),
	normals(normals)
{
	//initialization code only
	translate(); //no translation
	scale(); //no scale
	rotateTransform = RotationMatrix(); //no rotation
	updateWorldTransformation();
}

MeshModel::~MeshModel() {}


const glm::mat4x4& MeshModel::GetWorldTransformation() const {
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

void MeshModel::scale(float xAxis, float yAxis, float zAxis) {
	scaleTransform = glm::mat4x4({
		xAxis,		0.0f,		0.0f,		0.0f,
		0.0f,		yAxis,		0.0f,		0.0f,
		0.0f,		0.0f,		zAxis,		0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	});
	updateWorldTransformation();
}

//void MeshModel::setPosition(const float * const newX, const float * const newY, const float * const newZ)
//{
//	 move to origin
//	 set to newX,newY,newZ
//}

void MeshModel::move(const float * const xAddition, const float * const yAddition, const float * const zAddition)
{
	translate(xAddition, yAddition, zAddition);
}

void MeshModel::symmetricMove(const float * const addition)
{
	move(addition, addition, addition);
}



void MeshModel::translate(const float * const xAddition, const float * const yAddition, const float * const zAddition)
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
	updateWorldTransformation();
}

void MeshModel::rotate(const std::set<PairOfAxisAngle>& axisAngleSet)
{
	for each (PairOfAxisAngle info in axisAngleSet)
	{
		float angle = info.angle;
		switch (info.axis) {
		case Axis::XAXIS:
			rotateTransform.setXRotation(angle);
			break;
		case Axis::YAXIS:
			rotateTransform.setYRotation(angle);
			break;
		case Axis::ZAXIS:
			rotateTransform.setZRotation(angle);
			break;
		}
	}
	updateWorldTransformation();
}
