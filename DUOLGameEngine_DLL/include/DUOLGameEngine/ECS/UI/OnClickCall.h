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
		// �̰� ������ ���� ������Ʈ(���⼱ ��ư�� ������ UI) ���̵�
		DUOLCommon::UUID _gameObjectUUID;

		// � Mode����
		DUOLGameEngine::OnClickEventFunctionMode _methodID;


	};
}
