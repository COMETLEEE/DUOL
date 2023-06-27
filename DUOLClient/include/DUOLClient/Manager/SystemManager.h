#pragma once
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"

namespace DUOLGameEngine
{
	class GameObject;
}

namespace DUOLClient
{
	/**
	 * \brief system manager : 게임에 각 구역의 기믹을 관리하는 매니저 
	 */
	class SystemManager : public DUOLGameEngine::SingletonBase<SystemManager>
	{
		DECLARE_SINGLETON(DUOLClient::SystemManager)

		DELETE_COPY_MOVE(DUOLClient::SystemManager)

	public:
		virtual ~SystemManager() override;

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


		/**
		* \brief Stage C
		*/

	public:
		void OnAwake();

		void OnStart();

		void OnUpdate(float deltaTime);

		void DoorCollisionStay();

		/**
		 * \brief Stage A
		 */

		/**
		 * \brief Stage B
		 */
		void SetBStage(bool value) { _isBStage = value; }

		void SetBStageAllMonsterKill(bool value) { _isBStageAllMonsterKill = value; }

		void BSystem(float deltaTime);

		void RimLightFromDeltaTime(float deltaTime);

		void OpenDoor(float deltaTime);



		/**
		 * \brief Stage C
		 */

	};

}