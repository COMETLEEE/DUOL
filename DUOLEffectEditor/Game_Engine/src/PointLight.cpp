#include "PointLight.h"
#include "Transform.h"
#include "MuscleEngine.h"
#include "GraphicsManager.h"

Muscle::PointLight::PointLight(std::shared_ptr<GameObject> _GameObject) : Light(_GameObject), _PointLightInfo()
{
	
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
	_PointLightInfo._position = GetGameObject()->GetTransform()->GetPosition();
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
	_PointLightInfo._range = range;
}
