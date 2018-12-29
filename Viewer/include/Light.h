#pragma once
#include "MeshModel.h"
#include "Utils.h"

enum class LightType { POINT, PARALLEL };

class Light: public MeshModel {
private:
	const glm::vec3 BLACKCOLOR = glm::vec3(0.0f, 0.0f, 0.0f);
	float _intensity; //between 0.0f and 1.0f
	glm::vec3 _lightColor;
	const LightType _lightType;
	static int lightObjectsAmount;
	int _lightIndex;
public:
	Light(const LightType& type);
	~Light();

	int getLightIndex() const;
	glm::vec3 getLightColor() const;
	void setLightColor(const glm::vec3& color);
	float getLightIntensity() const;
	void setLightIntensity(const float intensity);
	LightType getLightType() const;
};

class LightPointSource : public Light {
private:

public:
	LightPointSource();
	~LightPointSource();
};

class LightParallelSource : public Light {
private:

public:
	LightParallelSource();
	~LightParallelSource();
};

