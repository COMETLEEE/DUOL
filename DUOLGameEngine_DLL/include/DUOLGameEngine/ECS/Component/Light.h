/**

	@file      Light.h
	@brief     represents light in game scene.
	@details   ~
	@author    COMETLEE
	@date      30.12.2022
	@copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"

#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"


namespace DUOLGameEngine
{
	class Light;
}

namespace DUOLGameEngine
{
	/**
	 * \brief namespace 를 DUOLGameEngine으로 맞추기 위해서 정의합니다.
	 */
	enum class LightType
	{
		Directional = 1u,
		Point = 2u,
		Spot = 3u,
		Area = 4u,
		Unknown = 0,
	};

	enum class LightState
	{
		Static = 1u, //static only
		Mixed = 2u, //mix + static
		Dynamic = 3u, //all dynamic
		Unknown = 0,
	};

	class DUOL_GAMEENGINE_API Light : public DUOLGameEngine::BehaviourBase
	{
	public:
		Light();

		Light(DUOLGameEngine::GameObject* owner,
			const DUOLCommon::tstring& name = TEXT("Light"));

		virtual ~Light() override;

	public:
		DUOLGameEngine::LightType GetLightType() const;

		void SetLightType(DUOLGameEngine::LightType lightType);

		DUOLGameEngine::LightState GetLightState() const;

		void SetLightState(DUOLGameEngine::LightState lightState);

		const DUOLMath::Vector3& GetDirection() const;

		const DUOLMath::Vector3& GetPosition() const;

		float GetAngle() const;

		void SetAngle(float range);

		const DUOLMath::Vector3& GetColor() const;

		void SetColor(const DUOLMath::Vector3& color);

		float GetIntensity() const;

		void SetIntensity(float intensity);

		float GetFallOffExponential() const;

		void SetFallOffExponential(float falloffExponetial);

		float GetAttenuationRadius() const;

		void SetAttenuationRadius(float radius);

		float GetWidth() const;

		void SetWidth(float width);

		float GetHeight() const;

		void SetHeight(float height);

	protected:
		/**
		 * \brief per frame constant buffer.
		 */
		DUOLGraphicsEngine::CurrentSceneInfo* _currentSceneInfo;

		/**
		 * \brief Light Information structure.
		 */
		DUOLGraphicsEngine::Light* _lightInfo;

		//TODO:: 임시 아이디입니다.. 흠..
		int _lightID;

		inline static int id = 0;

		DUOLGameEngine::Transform* _transform;

		/**
		 * \brief Copy current light information to constant buffer.
		 */
		void OnSceneLighting();

		DUOLCommon::EventListenerID _idOfSceneLighting;

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnEnable() override;

		virtual void OnDisable() override;

		RTTR_ENABLE(BehaviourBase)

			RTTR_REGISTRATION_FRIEND
	};
}