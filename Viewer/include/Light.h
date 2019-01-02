#pragma once
#include "MeshModel.h"
#include "Utils.h"

enum class LightType { POINT, PARALLEL };

class Light: public MeshModel {
private:
	const glm::vec3 NODIRECTION = glm::vec3(-1.0f, -1.0f, -1.0f);
	const glm::vec3 BLACKCOLOR = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
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
	void setLightAmbientColor(glm::vec3 color);
	glm::vec3 getLightAmbientColor()const;
	void setLightDiffuseColor(glm::vec3 color);
	glm::vec3 getLightDiffuseColor()const;
	void setLightSpecularColor(glm::vec3 color);
	glm::vec3 getLightSpecularColor()const;
	virtual void setDirection(const glm::vec3& dir);
	virtual glm::vec3 getDirection() const;
};

class LightPointSource : public Light {
private:

public:
	LightPointSource();
	~LightPointSource();
};

class LightParallelSource : public Light {
private:
	const glm::vec3 SUNDIRECTION = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 _direction;
public:
	LightParallelSource();
	void resetDirectionDefault();
	glm::vec3 getDirection() const override;
	void setDirection(const glm::vec3& dir) override;
	~LightParallelSource();
};

