#include "pch.h"
#include "PointLight.h"

Muscle::PointLight::PointLight(std::shared_ptr<GameObject> _GameObject) : Light(_GameObject)
{
	_PointLightInfo = std::make_shared<PointLightInfo>();
	_lightInfo = _PointLightInfo; // ĳ���ؼ� ����.. �Ź� ���̳��� ĳ��Ʈ�� ������ ��δ�.
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
