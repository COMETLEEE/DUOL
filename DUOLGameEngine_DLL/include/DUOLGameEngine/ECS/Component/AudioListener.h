#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLSound/SoundSystem.h"

namespace DUOLGameEngine
{
	/**
	 * \brief ������� ��� ��ü
	 */
	class DUOL_GAMEENGINE_API AudioListener : public DUOLGameEngine::BehaviourBase
	{
	public:
		AudioListener(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("AudioListener"));

		virtual ~AudioListener() override;

	private:
		DUOLSound::ListenerAttribute _listenerAttribute;

	public:
		virtual void OnEnable() override;

		virtual void OnDisable() override;

		virtual void OnLateUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)
	};
}