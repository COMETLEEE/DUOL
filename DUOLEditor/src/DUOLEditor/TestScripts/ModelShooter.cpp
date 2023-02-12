#include "DUOLEditor/TestScripts/ModelShooter.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"

namespace DUOLEditor
{
	ModelShooter::ModelShooter(DUOLGameEngine::GameObject* owner) :
		MonoBehaviourBase(owner, TEXT("ModelShooter"))
		, _isShooted(false)
	{
	}

	ModelShooter::~ModelShooter()
	{
		
	}

	void ModelShooter::OnStart()
	{
		
	}

	void ModelShooter::OnUpdate(float deltaTime)
	{
		if ((!_isShooted) && (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Space)))
		{
			_isShooted = true;

			for (int i = 0; i < _rigids.size(); i++)
				_rigids[i]->SetUseGravity(true);
		}
		else if (_isShooted)
		{
			GetTransform()->Translate(deltaTime * DUOLMath::Vector3::Right * 30.f);
		}
	}
}
