#include "SpotLight.h"
#include "Transform.h"
#include "MuscleEngine.h"
#include "GraphicsManager.h"

Muscle::SpotLight::SpotLight(std::shared_ptr<GameObject> _GameObject):Light(_GameObject), _SpotLightInfo()
{
	
	_lightInfo = _SpotLightInfo; // 캐싱해서 쓰자.. 매번 다이나믹 캐스트는 연산이 비싸니.
}

Muscle::SpotLight::~SpotLight()
{

}

void Muscle::SpotLight::Start()
{



}

void Muscle::SpotLight::LateUpdate()
{
	_SpotLightInfo._position = GetGameObject()->GetTransform()->GetPosition();
	_SpotLightInfo._direction = GetGameObject()->GetTransform()->GetLook();
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
	_SpotLightInfo._range;
}

void Muscle::SpotLight::SethalfAngle(float angle)
{
	_SpotLightInfo._halfAngle;
}

