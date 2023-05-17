#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

#include "rttr/registration_friend.h"

namespace DUOLGameEngine
{
	enum class DIRECTIONSTAT { LEFT_TO_RIGHT, RIGHT_TO_LEFT, TOP_TO_DOWN, DOWN_TO_TOP };

	class DUOL_GAMEENGINE_API Scrollbar final : public DUOLGameEngine::BehaviourBase
	{
	public:
		Scrollbar();

		Scrollbar(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("Scrollbar"));

		virtual ~Scrollbar();

		void OnUpdate(float deltaTime) override;

	private:
		void Initialize(DUOLGameEngine::GameObject* owner);

	private:
		DUOLGameEngine::GameObject* _scrollImage;

		DIRECTIONSTAT _directionState;

		float _maxGauge;

		float _nowGauge;

	public:
		float& GetMaxGauge() { return _maxGauge; }

		float& GetNowGauge() { return _nowGauge; }

		DIRECTIONSTAT& GetDirectionState() { return _directionState; }

		void SetDirectionState(DIRECTIONSTAT& direction) { _directionState = direction; }

		void SetMaxGauge(float& maxgauge) { _maxGauge = maxgauge; }

		void SetNowGauge(float& nowgauge) { _nowGauge = nowgauge; }

		DUOLGraphicsLibrary::Rect& CalculateRect(const DUOLGraphicsLibrary::Rect& parentRect);

#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion
	};

}