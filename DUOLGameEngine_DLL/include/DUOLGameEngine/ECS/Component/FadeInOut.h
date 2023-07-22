#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Image;
}

namespace DUOLGameEngine
{
	enum class FadeInOutMode
	{
		NOT
		, FADE_IN
		, FADE_OUT
		, DONE
	};

	class DUOL_GAMEENGINE_API FadeInOut : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		FadeInOut(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("FadeInOut"));

		virtual ~FadeInOut() override;

	private:
		DUOLGameEngine::Image* _image;

		FadeInOutMode _currentFadeInOutMode;

		float _duration;

		float _alphaForSeconds;

		bool _isBlinkImage;
	public:
		void StartFadeIn(float duration, std::function<void()> fadeEvent);

		void StartBlinkIn(float duration, std::function<void()> fadeEvent);

		void StartFadeOut(float duration, std::function<void()> fadeEvent);

		void StartBlinkOut(float duration, std::function<void()> fadeEvent);

		void SetUIOption(bool value);


		FadeInOutMode& GetFadeMode() { return _currentFadeInOutMode; }

		std::function<void()> _fadeEvent;

	public:
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;


		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND

	};
}