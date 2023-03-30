#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	enum class OnClickEventFunctionMode;


	class DUOL_GAMEENGINE_API OnClickCall final : public DUOLGameEngine::BehaviourBase
	{
	public:
		OnClickCall();

		OnClickCall(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("OnClickCall"));

		virtual ~OnClickCall();

		void OnUpdate(float deltaTime) override;

		void Initialize();

	private:
		// 이걸 가지는 게임 오브젝트(여기선 버튼을 가지는 UI) 아이디
		DUOLCommon::UUID _gameObjectUUID;

		// 어떤 Mode인지
		DUOLGameEngine::OnClickEventFunctionMode _methodID;


	};
}
