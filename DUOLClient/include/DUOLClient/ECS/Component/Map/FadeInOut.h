#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Image;
}

namespace DUOLClient
{
	enum class FadeInOutMode
	{
		NOT
		, FADE_IN
		, FADE_OUT
		, DONE
	};

	class FadeInOut : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		FadeInOut(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("FadeInOut"));

		virtual ~FadeInOut() override;

	private:
		DUOLGameEngine::Image* _image;

		FadeInOutMode _currentFadeInOutMode;

		float _duration;

		float _alphaForSeconds;

	public:
		void StartFadeIn(float duration, std::function<void()> fadeEvent);

		void StartFadeOut(float duration, std::function<void()> fadeEvent);

		std::function<void()> _fadeEvent;

	public:
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}