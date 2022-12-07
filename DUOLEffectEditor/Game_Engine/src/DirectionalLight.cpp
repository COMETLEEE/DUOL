#include "DirectionalLight.h"
#include "MuscleEngine.h"
#include "GraphicsManager.h"

Muscle::DirectionalLight::DirectionalLight(std::shared_ptr<GameObject> _GameObject) :Light(_GameObject)
{
	_directionalLightInfo = std::make_shared<MuscleGrapics::DirectionalLightInfo>();
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

void Muscle::DirectionalLight::SetDirectional(DUOLMath::Vector3 directional)
{
	_directionalLightInfo->_direction = directional;
}
