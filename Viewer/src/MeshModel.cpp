#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

void MeshModel::updateWorldTransformation() {
	glm::mat4x4 translateToOrigin = Utils::IdentityMat() - translateTransform + Utils::IdentityMat();
	worldTransform = translateToOrigin* rotateTransform.getRotationMatrix()*scaleTransform*translateTransform;
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

void MeshModel::scale(float xAxis = 1.0f, float yAxis = 1.0f, float zAxis = 1.0f) {
	scaleTransform = glm::mat4x4({
		xAxis,		0.0f,		0.0f,		0.0f,
		0.0f,		yAxis,		0.0f,		0.0f,
		0.0f,		0.0f,		zAxis,		0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	});
	updateWorldTransformation();
}

void MeshModel::translate(const float * const newX, const float * const newY, const float * const newZ)
{
	//TODO: Continue to implement translate... the initialization is incorrect.
	float x = *newX, y = *newY, z = *newZ;
	if (newX != nullptr) {
		x = *newX;
	}
	if (newY != nullptr) {
		y = *newY;
	}
	if (newZ != nullptr) {
		z = *newZ;
	}

	translateTransform = ;
	updateWorldTransformation();
}

void MeshModel::rotate(std::set<AxisAngleRotation> axisSet)
{
	for each (AxisAngleRotation info in axisSet)
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
