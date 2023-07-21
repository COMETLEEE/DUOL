#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"

namespace DUOLGameEngine
{
	class Animator;
}

/**
 * \brief 이벤트 바인딩용 더미 comp
 */
namespace DUOLClient
{
class DummyMonoBehavior : public DUOLGameEngine::MonoBehaviourBase
{
public:
	DummyMonoBehavior(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("DummyMonoBehavior"));

	virtual ~DummyMonoBehavior() override;

	RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	RTTR_REGISTRATION_FRIEND
};
}