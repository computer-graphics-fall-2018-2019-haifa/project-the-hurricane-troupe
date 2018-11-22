#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <set>
#include "Face.h"
#include "RotationMatrix.h"
#include "AxisRotation.h"
#include "AxisTypes.h"

/*
 * MeshModel class.
 * This class represents a mesh model (with faces and normals informations).
 * You can use Utils::LoadMeshModel to create instances of this class from .obj files.
 */

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
	bool isTransformUpdated;
	void _translate(const float* const newX, const float* const newY, const float* const newZ );
	void _scale(float xFactor, float yFactor, float zFactor);
	void _rotate(const std::set<PairOfAxisAngle>& axisAngleSet);
public:

	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	virtual ~MeshModel();

	/* returns the updated transformation as a result of rotations, scalings and translations */
	const glm::mat4x4& GetWorldTransformation();

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
	/* rotates the objects in a specific order, additional attention is needed to ensure correctness of rotations.*/
	void rotate(const std::set<PairOfAxisAngle>& axisAngleSet);

};
