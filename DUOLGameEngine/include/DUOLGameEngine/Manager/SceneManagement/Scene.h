/**

    @file      Scene.h
    @brief     Run-time data structure for *.duol file.
    @details   ~
    @author    COMETLEE
    @date      19.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <memory>
#include <vector>
#include <list>

#include "DUOLCommon/StringHelper.h"
#include "DUOLGameEngine/API/ISerializable.h"
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	class GameObject;

	/**
	 * \brief DUOLGameEngine에서 사용하는 '.duol' Scene File의 Run-time 객체
	 */
	class Scene : public ISerializable
	{
	public:
		Scene();

		~Scene();

		void UnInitialize();

	private:
		/**
		 * \brief Scene 내에 현재 활성화되어 있는 게임 오브젝트 리스트
		 */
		std::list<std::shared_ptr<GameObject>> _gameObjectsInScene;
		
		/**
		 * \brief 이번 프레임에 생성해야하는 게임 오브젝트 리스트
		 */
		std::vector<std::shared_ptr<GameObject>> _gameObjectsForCreate;

		/**
		 * \brief 파괴 예약된 게임 오브젝트 리스트 (게임 오브젝트 / 파괴까지의 딜레이)
		 */
		std::list<std::pair<std::shared_ptr<GameObject>, float>> _gameObjectsForDestroy;

		/**
		 * \brief 활성화 예약된 게임 오브젝트 리스트
		 */
		std::list<std::shared_ptr<GameObject>> _gameObjectsForActive;

		/**
		 * \brief 비활성화 예약된 게임 오브젝트 리스트
		 */
		std::list<std::shared_ptr<GameObject>> _gameObjectsForInActive;

		/**
		 * \brief 씬의 이름입니다.
		 */
		DUOLCommon::tstring _name;

		/**
		 * \brief 씬의 경로입니다.
		 */
		DUOLCommon::tstring _path;

	private:
		/**
		 * \brief 씬이 시작할 때 Start 보다 이전에 호출됩니다.
		 */
		void Awake() const;

		/**
		 * \brief 씬이 시작할 때 Awake 보다 이후에 호출됩니다.
		 */
		void Start() const;

		/**
		 * \brief 매 프레임마다 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		void Update(float deltaTime) const;

		/**
		 * \brief 매 물리 프레임마다 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		void FixedUpdate(float deltaTime) const;

		/**
		 * \brief Update 이후, 매 프레임마다 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		void LateUpdate(float deltaTime) const;

		/**
		 * \brief 생성 요청된 게임 오브젝트들을 생성합니다.
		 */
		void CreateGameObjects();

		/**
		 * \brief 파괴 요청된 게임 오브젝트들을 파괴합니다.
		 */
		void DestroyGameObjects();

		/**
		 * \brief 활성화 요청된 게임 오브젝트들을 활성화합니다.
		 */
		void ActiveGameObjects();

		/**
		 * \brief 비활성화 요청된 게임 오브젝트들을 활성화합니다.
		 */
		void InActiveGameObjects();

	private:
		// TODO : 아직 Destroy 호출 관련되어 덜 구상됨.
		// inline void RegisterDestroy(std::shared_ptr<GameObject>&& gameObject);

		inline void RegisterActive(std::shared_ptr<GameObject>&& gameObject)
		{
			_gameObjectsForActive.push_back(gameObject);
		}

		inline void RegisterInActive(std::shared_ptr<GameObject>&& gameObject)
		{
			_gameObjectsForInActive.push_back(gameObject);
		}

#pragma region FRIEND_CLASS
		friend class SceneManager;

		friend class GameObject;
#pragma endregion
	};
}