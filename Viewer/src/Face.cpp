#include "Face.h"

Face::Face(std::istream& issLine)
{
	vertexIndices =  { 0, 0, 0 };
	normalIndices =  { 0, 0, 0 };
	textureIndices = { 0, 0, 0 };

	char c;
	for (int i = 0; i < 3; i++)	
	{
		issLine >> std::ws >> vertexIndices.at(i) >> std::ws;

		if (issLine.peek() != '/')
		{
			continue;
		}

		issLine >> c >> std::ws;

		if (issLine.peek() == '/')
		{
			issLine >> c >> std::ws >> normalIndices.at(i);
			continue;
		}
		else
		{
			issLine >> textureIndices.at(i);
		}

		if (issLine.peek() != '/')
		{
			continue;
		}

		issLine >> c >> normalIndices.at(i);
	}
}

Face::Face(int x, int y, int z)
{
	vertexIndices.push_back(x);
	vertexIndices.push_back(y);
	vertexIndices.push_back(z);
}

Face::~Face()
{

}

bool Face::hasNormals() const
{
	return normalIndices.size() > 0;
}

const int Face::GetVertexIndex(int index) const
{
	return vertexIndices[index] - 1;
}

const int Face::GetNormalIndex(int index) const
{
	return normalIndices[index] - 1;
}

const int Face::GetTextureIndex(int index) const
{
	return textureIndices[index] - 1;
}

glm::vec3 Face::getFaceNormal() const
{
	return faceNormal;
}

void Face::setFaceNormal(glm::vec3 normal)
{
	faceNormal = normal;
}
