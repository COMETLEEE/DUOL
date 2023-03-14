#include "DUOLClient/Util/TransformController.h"

namespace DUOLClient
{
	TransformController::TransformController(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		MonoBehaviourBase(owner, name)
	{
	}

	TransformController::~TransformController()
	{
	}

	void TransformController::OnLateUpdate(float deltaTime)
	{
		MonoBehaviourBase::OnLateUpdate(deltaTime);
	}
}
