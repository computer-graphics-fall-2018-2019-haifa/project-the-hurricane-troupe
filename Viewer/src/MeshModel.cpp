#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	vertices(vertices),
	faces(faces),
	//textures(textures),
	normals(normals),
	worldTransform(Utils::IdentityMat())
{

}

MeshModel::~MeshModel()
{

}

void MeshModel::SetWorldTransformation(const glm::mat4x4& worldTransform)
{
	this->worldTransform = worldTransform;
}

const glm::mat4x4& MeshModel::GetWorldTransformation() const
{
	return worldTransform;
}

void MeshModel::SetColor(const glm::vec4& color)
{
	this->color = color;
}

const glm::vec4& MeshModel::GetColor() const
{
	return color;
}

const std::string& MeshModel::GetModelName()
{
	return modelName;
}

std::vector<glm::vec2> MeshModel::getTextures()
{
	return textures;
}

std::vector<glm::vec3> MeshModel::getNormals()
{
	return normals;
}

std::vector<glm::vec3> MeshModel::getVertices()
{
	return vertices;
}

std::vector<Face> MeshModel::getFaces()
{
	return faces;
}
