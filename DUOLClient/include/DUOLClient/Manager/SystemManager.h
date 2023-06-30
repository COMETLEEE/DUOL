#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	class GameObject;
}

namespace DUOLClient
{
	class MainCameraController;
}

namespace DUOLClient
{
	/**
	 * \brief system manager : 게임에 각 구역의 기믹을 관리하는 매니저 
	 */
	class SystemManager : public DUOLGameEngine::MonoBehaviourBase
	{
		DELETE_COPY_MOVE(DUOLClient::SystemManager)

	public:
		SystemManager(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("SystemManager"));

		virtual ~SystemManager() override;

	private:
		static DUOLClient::SystemManager* _instance;

		DUOLClient::MainCameraController* _mainCameraController;

	private:
		float _currentTime;
		/**
		* \brief Stage A
		 */



		/**
		* \brief Stage B
		*/
		DUOLGameEngine::GameObject* _rimLightObject;

		DUOLGameEngine::GameObject* _doorObject;

		bool _isBStage;

		bool _isBStageAllMonsterKill;

		bool _isDoorMonsterKill;

		bool _isOpenDoor;

		float _moveDoor;

		float _rimPower;


		/**
		* \brief Stage C
		*/


		bool _isCameraSequenceMode; 

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		void PlayerCameraAction(std::string name,DUOLGameEngine::Transform* playertransform);

		/**
		 * \brief Stage A
		 */

		/**
		 * \brief Stage B
		 */
		void SetBStage(bool value) { _isBStage = value; }

		void SetBStageAllMonsterKill(bool value) { _isBStageAllMonsterKill = value; }

		inline bool GetIsDoor() { return _isDoorMonsterKill; }

		void BSystem(float deltaTime);

		void RimLightFromDeltaTime(float deltaTime);

		void OpenDoor(float deltaTime);

		void DoorCollisionStay();

		/**
		 * \brief Stage C
		 */



		static DUOLClient::SystemManager* GetInstance();

		bool IsGameraSequenceMode() { return _isCameraSequenceMode; }
	private:
		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

			RTTR_REGISTRATION_FRIEND

	};

}