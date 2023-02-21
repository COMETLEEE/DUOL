#include "Light.h"

#include "GraphicsManager.h"
#include "MuscleEngine.h"
#include "Transform.h"

namespace Muscle
{

	Light::Light(std::shared_ptr<GameObject> _GameObject) :IComponents(_GameObject)
	{
	}

	Light::~Light()
	{
	}

	void Light::Start()
	{
	}

	void Light::Update()
	{
	}

	void Light::Render()
	{
		_lightInfo.Position = GetGameObject()->GetTransform()->GetPosition();

		MuscleEngine::GetInstance()->GetGraphicsManager()->PostLightingData(_lightInfo);
	}

}
