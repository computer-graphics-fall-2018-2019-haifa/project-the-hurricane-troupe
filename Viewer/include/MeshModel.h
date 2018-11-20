#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <set>
#include "Face.h"
#include "RotationMatrix.h"

/*
 * MeshModel class.
 * This class represents a mesh model (with faces and normals informations).
 * You can use Utils::LoadMeshModel to create instances of this class from .obj files.
 */

enum class Axis {
	XAXIS,
	YAXIS,
	ZAXIS
};

typedef struct _AxisAngleRotation {
	Axis axis;
	float angle;
} AxisAngleRotation;

class MeshModel
{
private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	glm::vec4 color;
	std::string modelName;
	RotationMatrix rotateTransform;
	glm::mat4x4 translateTransform;
	glm::mat4x4 scaleTransform;
	glm::mat4x4 worldTransform;
	void updateWorldTransformation();
public:

	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	virtual ~MeshModel();

	const glm::mat4x4& GetWorldTransformation() const;

	const glm::vec4& GetColor() const;
	void SetColor(const glm::vec4& color);

	const std::string& GetModelName();
	std::vector<glm::vec2> getTextures();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getVertices();
	std::vector<Face> getFaces();
	// Add more methods/functionality as needed...
	void scale(float xAxis = 1.0f, float yAxis = 1.0f, float zAxis = 1.0f);
	void translate(const float* const newX = nullptr, const float* const newY = nullptr, const float* const newZ = nullptr);
	void rotate(std::set<AxisAngleRotation> axisSet);

};
