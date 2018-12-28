#include "Utils.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

MeshModel Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::ifstream ifile(filePath.c_str());
	std::map<int, std::vector<glm::vec3>> forCalculate;
	std::map<int,glm::vec3> finalNormalPerVertex;

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			textures.push_back(Utils::Vec2fFromStream(issLine));
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	for (int i = 0; i < faces.size(); i++)
	{
		int v1 = faces[i].GetVertexIndex(0);
		float x1 = vertices[v1].x, y1 = vertices[v1].y, z1 = vertices[v1].z;
		int v2 = faces[i].GetVertexIndex(1);
		float x2 = vertices[v2].x, y2 = vertices[v2].y, z2 = vertices[v2].z;
		int v3 = faces[i].GetVertexIndex(2);
		float x3 = vertices[v3].x, y3 = vertices[v3].y, z3 = vertices[v3].z;

		glm::vec4 originalPoint1 = glm::vec4(x1, y1, z1, 1.0f);
		glm::vec4 originalPoint2 = glm::vec4(x2, y2, z2, 1.0f);
		glm::vec4 originalPoint3 = glm::vec4(x3, y3, z3, 1.0f);

		glm::vec4 vector1 = (originalPoint2 - originalPoint1);
		glm::vec4 vector2 = (originalPoint3 - originalPoint1);
		glm::vec4 vector3 = (originalPoint3 - originalPoint2);

		glm::vec3 a = glm::vec3(vector1.x, vector1.y, vector1.z);
		glm::vec3 b = glm::vec3(vector2.x, vector2.y, vector2.z);
		glm::vec3 c = glm::vec3(vector3.x, vector3.y, vector3.z);

		glm::vec3 normalHelper1 = glm::cross(a, b);
		glm::vec3 normalHelper2 = glm::cross(-a, c);
		glm::vec3 normalHelper3 = glm::cross(-b, -c);
		glm::vec3 _normalPoint = (normalHelper1 + normalHelper2 + normalHelper3) / 3.0f;

		faces[i].setFaceNormal(_normalPoint);

		forCalculate[v1].push_back(_normalPoint);
		forCalculate[v2].push_back(_normalPoint);
		forCalculate[v3].push_back(_normalPoint);
	}

	std::map<int, std::vector<glm::vec3>>::iterator p;
	for (p = forCalculate.begin(); p != forCalculate.end(); p++) {
		glm::vec3 sumVectors(0, 0, 0);
		int size = 0;
		for (int j = 0; j < p->second.size(); j++)
		{
			sumVectors += p->second[j];
			size++;
		}
		glm::vec3 finalNormalPerCurr(sumVectors.x / (float)size, sumVectors.y / (float)size, sumVectors.z / (float)size);
		finalNormalPerVertex[p->first] = finalNormalPerCurr;
	}
	/*TODO: Add textures to return*/
	return MeshModel(finalNormalPerVertex,faces, vertices, normals, Utils::GetFileName(filePath));
}

glm::mat4x4 Utils::IdentityMat()
{
	return glm::mat4x4({
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		});
}

bool Utils::isColorLegal(const glm::vec3 & color)
{
	return (0.0f <= color.x && color.x <= 1.0f) && (0.0f <= color.y && color.y <= 1.0f) && (0.0f <= color.z && color.z <= 1.0f);
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}