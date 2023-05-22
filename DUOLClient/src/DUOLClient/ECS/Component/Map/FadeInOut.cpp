#include "DUOLClient/ECS/Component/Map/FadeInOut.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Image.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::FadeInOut>("FadeInOut")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLClient
{
	FadeInOut::FadeInOut(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
		, _currentFadeInOutMode(FadeInOutMode::NOT)
		, _duration(0.f)
		, _alphaForSeconds(0.f)
		, _fadeEvent(nullptr)
	{

	}

	FadeInOut::~FadeInOut()
	{

	}

	void FadeInOut::StartFadeIn(float duration, std::function<void()> fadeEvent)
	{
		if (_currentFadeInOutMode != FadeInOutMode::NOT
			&& _currentFadeInOutMode != FadeInOutMode::DONE)
			return;

		_currentFadeInOutMode = FadeInOutMode::FADE_IN;

		GetGameObject()->SetIsActiveSelf(true);

		if(_image==nullptr)
			_image = GetGameObject()->GetComponent<DUOLGameEngine::Image>();

		_image->SetColor(DUOLMath::Vector4(0.f, 0.f, 0.f, 1.f));

		_alphaForSeconds = -1.f / duration;

		_fadeEvent = fadeEvent;
	}

	void FadeInOut::StartFadeOut(float duration, std::function<void()> fadeEvent)
	{
		if (_currentFadeInOutMode != FadeInOutMode::NOT 
			&& _currentFadeInOutMode != FadeInOutMode::DONE)
			return;

		_currentFadeInOutMode = FadeInOutMode::FADE_OUT;

		GetGameObject()->SetIsActiveSelf(true);

		if (_image == nullptr)
			_image = GetGameObject()->GetComponent<DUOLGameEngine::Image>();

		_image->SetColor(DUOLMath::Vector4(0.f, 0.f, 0.f, 0.f));

		_alphaForSeconds = 1.f / duration;

		_fadeEvent = fadeEvent;
	}

	void FadeInOut::OnAwake()
	{
		_image = GetGameObject()->GetComponent<DUOLGameEngine::Image>();

		_image->SetColor(DUOLMath::Vector4(0.f, 0.f, 0.f, 1.f));
	}

	void FadeInOut::OnUpdate(float deltaTime)
	{
		switch (_currentFadeInOutMode)
		{
			case FadeInOutMode::NOT:
			{
				break;
			}

			case FadeInOutMode::FADE_IN:
			{
				auto&& color = std::move(_image->GetColor());

				float currentAlpha = color.w;

				_image->SetColor(color + DUOLMath::Vector4(0.f, 0.f, 0.f, _alphaForSeconds * deltaTime));

				if (currentAlpha + _alphaForSeconds * deltaTime <= 0.f)
				{
					GetGameObject()->SetIsActiveSelf(false);

					_currentFadeInOutMode = FadeInOutMode::DONE;

					if (_fadeEvent != nullptr)
						_fadeEvent();
				}

				break;
			}

			case FadeInOutMode::FADE_OUT:
			{
				auto&& color = std::move(_image->GetColor());

				float currentAlpha = color.w;

				_image->SetColor(color + DUOLMath::Vector4(0.f, 0.f, 0.f, _alphaForSeconds * deltaTime));

				if (currentAlpha + _alphaForSeconds * deltaTime >= 1.1f)
				{
					GetGameObject()->SetIsActiveSelf(false);

					_currentFadeInOutMode = FadeInOutMode::DONE;

					if (_fadeEvent != nullptr)
						_fadeEvent();
				}

				break;
			}

			case FadeInOutMode::DONE:
			{

				break;
			}
		}
	}
}