#include "pch.h"
#include "DirectionalLight.h"

Muscle::DirectionalLight::DirectionalLight(std::shared_ptr<GameObject> _GameObject) :Light(_GameObject)
{
	_directionalLightInfo = std::make_shared<DirectionalLightInfo>();
	_lightInfo = _directionalLightInfo; // 캐싱해서 쓰자.. 매번 다이나믹 캐스트는 연산이 비싸니.
}

Muscle::DirectionalLight::~DirectionalLight()
{
}

void Muscle::DirectionalLight::Start()
{
}

void Muscle::DirectionalLight::Update()
{
}

void Muscle::DirectionalLight::Render()
{
	MuscleEngine::GetInstance()->GetGraphicsManager()->PostDirectionalLightInfo(_directionalLightInfo);
}

void Muscle::DirectionalLight::SetDirectional(Vector3 directional)
{
	_directionalLightInfo->_direction = directional;
}
