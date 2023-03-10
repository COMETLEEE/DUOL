#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"


namespace DUOLClient
{
	/**
	 * \brief ���� ���� �ÿ��� �� ����
	 */
	class DUOL_CLIENT_API MaterialController final : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		MaterialController(DUOLGameEngine::GameObject* owner);

		virtual ~MaterialController() override;

	public:
		DUOLGameEngine::GameObject* obj;

		float _metallic;

		float _roughness;


	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}
