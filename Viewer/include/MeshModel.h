#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <map>
#include "Face.h"
#include "RotationMatrix.h"
#include "RotationRules.h"
#include "AxisTypes.h"
#include <glad/glad.h>

/*
 * MeshModel class.
 * This class represents a mesh model (with faces and normals informations).
 * You can use Utils::LoadMeshModel to create instances of this class from .obj files.
 */
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex;
};

class MeshModel
{
protected:
	std::map<int, glm::vec3> finalNormalPerVertex;
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<Vertex> modelVertices;
	std::vector<glm::vec3> textureCoords;

	glm::vec4 color;
	glm::vec4 minBoundingBoxVec;
	glm::vec4 maxBoundingBoxVec;
	std::string modelName;
	RotationMatrix rotateTransform;
	RotationMatrix worldRotationTransform;
	glm::mat4x4 translateTransform;
	glm::mat4x4 scaleTransform;
	glm::mat4x4 worldTransform;
	GLuint vao;
	GLuint vbo;
	void updateWorldTransformation();
	bool isTransformUpdated;
	float _xScale, _yScale, _zScale;
	float _xDelta, _yDelta, _zDelta;
	void _translate(const float* const newX, const float* const newY, const float* const newZ );
	void _scale(float xFactor, float yFactor, float zFactor);
	void _rotate(const RotationRules& rotation);
	void _rotateAroundWorld(const RotationRules & rotation);
public:
	
	MeshModel(std::map<int, glm::vec3>& _finalNormalPerVertex, const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	MeshModel();
	virtual ~MeshModel();


	const std::string& GetModelName();
	std::vector<glm::vec2> getTextures();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getVertices();
	std::vector<Face> getFaces();
	// Add more methods/functionality as needed...
	/* returns the updated transformation as a result of rotations, scalings and translations */
	const glm::mat4x4& GetWorldTransformation();
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

	/* sets the position *exactly* to (newX, newY, newZ)*/
	void setPosition(float newX, float newY, float newZ);

	/*sets the position back to the original model*/
	void resetPosition();
	
	/*sets the rotation back to original*/
	void resetRotation();

	/*sets the rotation for the world back to original*/
	void resetRotationAroundWorld();

	/* rotates the objects in a specific order, additional attention is needed to ensure correctness of rotations.*/
	void rotate(const RotationRules& rotation);

	/* rotates the object around the world */
	void rotateAroundWorld(const RotationRules & rotation);

	void getScalingFactors(float* xFactor, float* yFactor, float* zFactor) const;
	void getCoordinateDifference(float* const xDelta, float* const yDelta, float* const zDelta) const;
	glm::vec4 getMinBoundingBoxVec() const;
	glm::vec4 getMaxBoundingBoxVec() const;
	void setMinBoundingBoxVec(glm::vec4 min);
	void setMaxBoundingBoxVec(glm::vec4 max);
	std::map<int, glm::vec3> getNormalForVertices() const;
	void openGLInitializations();
	std::vector<Vertex> getModelVertices();
	GLuint GetVAO() const;

};

