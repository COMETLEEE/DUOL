#include "pch.h"
#include "SpotLight.h"

Muscle::SpotLight::SpotLight(std::shared_ptr<GameObject> _GameObject):Light(_GameObject)
{
	_SpotLightInfo = std::make_shared<SpotLightInfo>();
	_lightInfo = _SpotLightInfo; // ĳ���ؼ� ����.. �Ź� ���̳��� ĳ��Ʈ�� ������ ��δ�.
}

Muscle::SpotLight::~SpotLight()
{

}

void Muscle::SpotLight::Start()
{



}

void Muscle::SpotLight::LateUpdate()
{
	_SpotLightInfo->_position = GetGameObject()->GetTransform()->GetPosition();
	_SpotLightInfo->_direction = GetGameObject()->GetTransform()->GetLook();
}

void Muscle::SpotLight::Update()
{

}

void Muscle::SpotLight::Render()
{
	MuscleEngine::GetInstance()->GetGraphicsManager()->PostSpotLightInfo(_SpotLightInfo);
}

void Muscle::SpotLight::SetRange(float range)
{
	_SpotLightInfo->_range;
}

void Muscle::SpotLight::SethalfAngle(float angle)
{
	_SpotLightInfo->_halfAngle;
}

