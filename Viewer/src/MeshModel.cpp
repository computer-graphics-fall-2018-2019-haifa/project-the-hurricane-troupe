#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


void MeshModel::updateWorldTransformation() {
	if (isTransformUpdated == true) return;
	worldTransform = worldRotationTransform.getTransform() * translateTransform * rotateTransform.getTransform() * scaleTransform;
	isTransformUpdated = true;
}

MeshModel::MeshModel(std::map<int, glm::vec3>& _finalNormalPerVertex, const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	vertices(vertices),
	faces(faces),
	//textures(textures),
	normals(normals),
	_xScale(1.0f), _yScale(1.0f), _zScale(1.0f),
	_xDelta(0.0f), _yDelta(0.0f), _zDelta(0.0f),
	finalNormalPerVertex(_finalNormalPerVertex),
	minBoundingBoxVec(0.0f), 
	maxBoundingBoxVec(0.0f)
{
	//initialization code only
	_translate(nullptr, nullptr, nullptr); //no translation
	_scale(1.0, 1.0, 1.0); //no scale
	rotateTransform = RotationMatrix(); //no rotation
	worldRotationTransform = RotationMatrix(); //no rotation
	isTransformUpdated = false;
	updateWorldTransformation(); //set the transformation.
	openGLInitializations();
}

void MeshModel::openGLInitializations()
{

	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; ++j)
		{
			int vertexIndex = currentFace.GetVertexIndex(j);
			int normalIndex = currentFace.GetNormalIndex(j);

			Vertex vertex;
			vertex.position = vertices[vertexIndex];
			vertex.normal = normals[normalIndex];

			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j);
				vertex.tex = textureCoords[textureCoordsIndex];
			}

			modelVertices.push_back(vertex);
		}
	}
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

std::vector<Vertex> MeshModel::getModelVertices()
{
	return modelVertices;
}

MeshModel::MeshModel(): minBoundingBoxVec(0.0f), maxBoundingBoxVec(0.0f)
{
	openGLInitializations();
}

MeshModel::~MeshModel() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}


const glm::mat4x4& MeshModel::GetWorldTransformation() {
	updateWorldTransformation();
	return worldTransform;
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
	_xScale = xFactor; _yScale = yFactor; _zScale = zFactor;
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
	float newX = *xAddition + _xDelta;
	_xDelta = newX;
	float newY = *yAddition + _yDelta;
	_yDelta = newY;
	float newZ = *zAddition + _zDelta;
	_zDelta = newZ;
	_translate(&newX, &newY, &newZ);
}

void MeshModel::symmetricMove(const float * const addition)
{
	move(addition, addition, addition);
}

void MeshModel::setPosition(float newX, float newY, float newZ)
{
	_xDelta = 0.0f;
	_yDelta = 0.0f;
	_zDelta = 0.0f;
	_translate(&newX, &newY, &newZ);
}

void MeshModel::resetPosition()
{
	setPosition(0.0f, 0.0f, 0.0f);
}

void MeshModel::resetRotation()
{
	rotateTransform.resetToOrginalRotation();
	isTransformUpdated = false;
}

void MeshModel::resetRotationAroundWorld()
{
	worldRotationTransform.resetToOrginalRotation();
	isTransformUpdated = false;
}

void MeshModel::rotate(const RotationRules& rotation) {
	_rotate(rotation);
}

void MeshModel::rotateAroundWorld(const RotationRules& rotation) {
	_rotateAroundWorld(rotation);
}

void MeshModel::getScalingFactors(float * xFactor, float * yFactor, float * zFactor) const
{
	*xFactor = _xScale;
	*yFactor = _yScale;
	*zFactor = _zScale;
}

void MeshModel::getCoordinateDifference(float* const xDelta, float* const yDelta, float* const zDelta) const
{
	*xDelta = _xDelta;
	*yDelta = _yDelta;
	*zDelta = _zDelta;
}

glm::vec4 MeshModel::getMinBoundingBoxVec() const
{
	return minBoundingBoxVec;
}

glm::vec4 MeshModel::getMaxBoundingBoxVec() const 
{
	return maxBoundingBoxVec;
}

void MeshModel::setMinBoundingBoxVec(glm::vec4 min)
{
	minBoundingBoxVec = min;
}

void MeshModel::setMaxBoundingBoxVec(glm::vec4 max)
{
	maxBoundingBoxVec = max;
}

std::map<int, glm::vec3> MeshModel::getNormalForVertices() const
{
	return finalNormalPerVertex;
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

void MeshModel::_rotate(const RotationRules& rotation)
{
	rotateTransform.setXRotation(rotation.getAngleX(AngleUnits::RADIANS));
	rotateTransform.setYRotation(rotation.getAngleY(AngleUnits::RADIANS));
	rotateTransform.setZRotation(rotation.getAngleZ(AngleUnits::RADIANS));
	isTransformUpdated = false;
}

void MeshModel::_rotateAroundWorld(const RotationRules& rotation) {
	worldRotationTransform.setXRotation(rotation.getAngleX(AngleUnits::RADIANS));
	worldRotationTransform.setYRotation(rotation.getAngleY(AngleUnits::RADIANS));
	worldRotationTransform.setZRotation(rotation.getAngleZ(AngleUnits::RADIANS));
	isTransformUpdated = false;
}

GLuint MeshModel::GetVAO() const
{
	return vao;
}
