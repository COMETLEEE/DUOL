#include "pch.h"
#include "DirectionalLight.h"

Muscle::DirectionalLight::DirectionalLight(std::shared_ptr<GameObject> _GameObject) :Light(_GameObject)
{
	_directionalLightInfo = std::make_shared<DirectionalLightInfo>();
	_lightInfo = _directionalLightInfo; // ĳ���ؼ� ����.. �Ź� ���̳��� ĳ��Ʈ�� ������ ��δ�.
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
