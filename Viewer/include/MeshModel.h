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
} PairOfAxisAngle;

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
	void translate(const float* const newX = nullptr, const float* const newY = nullptr, const float* const newZ = nullptr);
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
	//void setPosition(const float* const newX = nullptr, const float* const newY = nullptr, const float* const newZ = nullptr);

	/* scales model in the following way:
		-> xFactor times its size on the x-axis,
		-> yFactor times its size on the y-axis,
		-> zFactor times its size on the z-axis. */
	void scale(float xFactor = 1.0f, float yFactor = 1.0f, float zFactor = 1.0f);
	/* scales model in the following way:
		-> factor times its size on the x-axis, y-axis and z-axis.*/
	void symmetricScale(float factor = 1.0f);
	/* moves the model relative to its location in the following way:
		-> assume the model's current location is (x,y,z), 
			then its new location will become (x + newX, y + newY, z + newZ). */
	void move(const float* const newX = nullptr, const float* const newY = nullptr, const float* const newZ = nullptr);
	/* moves the model relative to its location in the following way:
	-> assume the model's current location is (x,y,z),
	then its new location will become (x + addition, y + addition, z + addition). */
	void symmetricMove(const float * const addition = nullptr);
	void rotate(const std::set<PairOfAxisAngle>& axisAngleSet);

};
