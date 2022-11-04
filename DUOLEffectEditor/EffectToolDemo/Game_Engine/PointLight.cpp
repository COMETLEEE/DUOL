#include "pch.h"
#include "PointLight.h"

Muscle::PointLight::PointLight(std::shared_ptr<GameObject> _GameObject) : Light(_GameObject)
{
	_PointLightInfo = std::make_shared<PointLightInfo>();
	_lightInfo = _PointLightInfo; // 캐싱해서 쓰자.. 매번 다이나믹 캐스트는 연산이 비싸니.
}

Muscle::PointLight::~PointLight()
{

}

void Muscle::PointLight::Start()
{

}

void Muscle::PointLight::LateUpdate()
{
	_PointLightInfo->_position = GetGameObject()->GetTransform()->GetPosition();
}

void Muscle::PointLight::Update()
{

}

void Muscle::PointLight::Render()
{
	MuscleEngine::GetInstance()->GetGraphicsManager()->PostPointLightInfo(_PointLightInfo);
}

void Muscle::PointLight::SetRange(float range)
{
	_PointLightInfo->_range = range;
}
