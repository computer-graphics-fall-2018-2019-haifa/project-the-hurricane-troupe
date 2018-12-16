#include "PrimMeshModel.h"



PrimMeshModel::PrimMeshModel()
{
	vertices.push_back(glm::vec3(0, 0, 0));
	vertices.push_back(glm::vec3(1, 0, 0));
	vertices.push_back(glm::vec3(1, 1, 0));
	vertices.push_back(glm::vec3(0, 1, 0));
	vertices.push_back(glm::vec3(0.50, 0.50, 1.6));

	faces.push_back(Face(4, 1, 2));
	faces.push_back(Face(3, 4, 2));
	faces.push_back(Face(5, 2, 1));
	faces.push_back(Face(4, 5, 1));
	faces.push_back(Face(3, 5, 4));
	faces.push_back(Face(5, 3, 2));


	modelName = "Pyramid";

		_xScale=(1.0f);
		_yScale=(1.0f);
		_zScale=(1.0f);
		_xDelta=(0.0f);
		_yDelta=(0.0f);
		_zDelta=(0.0f);
		//initialization code only
		_translate(nullptr, nullptr, nullptr); //no translation
		_scale(1.0, 1.0, 1.0); //no scale
		rotateTransform = RotationMatrix(); //no rotation
		isTransformUpdated = false;
		updateWorldTransformation(); //set the transformation.
}
PrimMeshModel::~PrimMeshModel()
{
}
