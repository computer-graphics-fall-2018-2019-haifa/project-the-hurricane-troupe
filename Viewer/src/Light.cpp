#include "Light.h"

int Light::lightObjectsAmount = 0;
Light::Light(const LightType& type): 
	MeshModel(Utils::LoadMeshModel(".\\..\\..\\project-the-hurricane-troupe\\Data\\obj_examples\\demo.obj")), 
	_lightType(type),
	_lightIndex(lightObjectsAmount++),
	_lightColor(BLACKCOLOR)
{
}

Light::~Light()
{
}

int Light::getLightIndex() const
{
	return _lightIndex;
}

glm::vec3 Light::getLightColor() const
{
	return _lightColor;
}

void Light::setLightColor(const glm::vec3 & color)
{
	if (Utils::isColorLegal(color) == false) return;
	_lightColor = color;
}

float Light::getLightIntensity() const
{
	return _intensity;
}

void Light::setLightIntensity(const float intensity)
{
	if (intensity < 0.0f || intensity > 1.0f) return;
	_intensity = intensity;
}

LightType Light::getLightType() const
{
	return _lightType;
}

LightPointSource::LightPointSource(): Light(LightType::POINT)
{
}

LightPointSource::~LightPointSource()
{
}

LightParallelSource::LightParallelSource(): Light(LightType::PARALLEL)
{
}

LightParallelSource::~LightParallelSource()
{
}
