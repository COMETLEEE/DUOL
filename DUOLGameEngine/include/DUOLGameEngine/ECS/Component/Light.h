/**

    @file      Light.h
    @brief     represents light in game scene.
    @details   ~
    @author    COMETLEE
    @date      30.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"

#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	/**
	 * \brief namespace 를 DUOLGameEngine으로 맞추기 위해서 정의합니다.
	 */
	enum class LightType
	{
		Direction = 1u,
		Point = 2u,
		Spot = 3u,
		Unknown = 0,
	};

	class Light : public DUOLGameEngine::BehaviourBase
	{
	public:
		Light(const std::weak_ptr<DUOLGameEngine::GameObject>& owner,
			const DUOLCommon::tstring& name = TEXT("Light"));

		virtual ~Light() override;

	public:
		const DUOLGraphicsEngine::LightType& GetLightType() const;

		void SetLightType(const DUOLGameEngine::LightType& lightType);

		const DUOLMath::Vector3& GetDirection() const;

		const DUOLMath::Vector3& GetPosition() const;

		float GetRange() const;

		void SetRange(float range);

		const DUOLMath::Vector3& GetColor() const;

		void SetColor(const DUOLMath::Vector3& color);

		float GetIntensity() const;

		void SetIntensity(float intensity);

		float GetAttenuation() const;

		void SetAttenuation(float attenuation);

		float GetAttenuationRadius() const;

		void SetAttenuationRadius(float radius);

	protected:
		/**
		 * \brief per frame constant buffer.
		 */
		DUOLGraphicsEngine::ConstantBufferPerFrame*		_cbPerFrame;

		/**
		 * \brief Light Information structure. 
		 */
		DUOLGraphicsEngine::Light						_lightInfo;

		/**
		 * \brief Copy current light information to constant buffer.
		 */
		void CopyLightInformation();

	public:
		virtual void OnUpdate(float deltaTime) override;
	};
}