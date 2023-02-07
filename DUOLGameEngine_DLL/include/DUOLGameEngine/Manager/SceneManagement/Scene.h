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
#include <rttr/registration_friend>

#include "DUOLCommon/StringHelper.h"
#include "DUOLGameEngine/Util/Defines.h"

#include "DUOLGameEngine/Export_Engine.h"

namespace DUOLGameEngine
{
	class GameObject;

	/**
	 * \brief DUOLGameEngine에서 사용하는 '.duol' Scene File의 Run-time 객체
	 */
	class DUOL_GAMEENGINE_API Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		// Scene();

		Scene(const DUOLCommon::tstring& name = TEXT("EmptyScene"));

		virtual ~Scene();

		void UnInitialize();

	public:
		/**
		 * \brief 씬이 시작할 때 Start 보다 이전에 호출됩니다.
		 * 일단 테스트용으로 상속을 통해 씬을 구성할 수 있도록 구현
		 */
		virtual void Awake();

		/**
		 * \brief 씬이 시작할 때 Awake 보다 이후에 호출됩니다.
		 */
		void Start() const;

		/**
		 * \brief 매 프레임마다 호출됩니다. 원활한 테스트를 위해 virtual로 열어둡니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		virtual void Update(float deltaTime);

		/**
		 * \brief 매 프레임마다 호출됩니다. MonoBehaviour에서 실행된 Invoke와 관련된 업데이트를 진행합니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		void InvokeUpdate(float deltaTime) const;

		/**
		 * \brief 매 프레임마다 호출됩니다. MonoBehaviour에 구현된 코루틴과 관련된 업데이트를 진행합니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		void CoroutineUpdate(float deltaTime) const;

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

#pragma region 
	private:
		/**
		 * \brief Scene 내의 모든 게임 오브젝트
		 */
		std::list<std::shared_ptr<GameObject>> _gameObjectsInScene;

		/**
		 * \brief Scene 내의 Root GameObject
		 */
		std::list<std::shared_ptr<GameObject>> _rootObjectsInScene;

		/**
		 * \brief 이번 프레임에 생성해야하는 게임 오브젝트 리스트
		 */
		std::vector<std::shared_ptr<GameObject>> _gameObjectsForCreate;

		/**
		 * \brief 파괴 예약된 게임 오브젝트 리스트 (게임 오브젝트 / 파괴까지의 딜레이)
		 */
		std::list<std::pair<std::shared_ptr<GameObject>, float>> _gameObjectsForDestroy;

		/**OBJECT_MANAGING
		 * \brief 활성화 예약된 게임 오브젝트 리스트
		 */
		std::list<std::shared_ptr<GameObject>> _gameObjectsForActive;

		/**
		 * \brief 비활성화 예약된 게임 오브젝트 리스트
		 */
		std::list<std::shared_ptr<GameObject>> _gameObjectsForInActive;

	private:
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

		/**
		 * \brief Root Object가 아니게 된 경우에 리스트에서 지웁니다.
		 * \param gameObject 리스트에서 제외할 게임 오브젝트
		 */
		void RemoveInRootObjectsList(DUOLGameEngine::GameObject* gameObject);

		/**
		 * \brief Root Object가 된 경우에 리스트에 추가합니다.
		 * \param gameObject 리스트에 추가할 게임 오브젝트
		 */
		void AddInRootObjectsList(DUOLGameEngine::GameObject* gameObject);

	private:
		// 생성을 등록합니다.
		void RegisterCreateGameObject(GameObject* gameObject);

		// 파괴를 등록합니다.
		void RegisterDestroyGameObject(GameObject* gameObject, float deltaTime);

		// 활성화를 등록합니다.
		void RegisterActiveGameObject(GameObject* gameObject);

		// 비활성화를 등록합니다.
		void RegisterInActiveGameObject(GameObject* gameObject);

	public:
		/**
		 * \brief 해당 씬에 빈 게임 오브젝트를 생성합니다.
		 * \return 생성된 게임 오브젝트
		 */
		DUOLGameEngine::GameObject* CreateEmpty();

		/**
		 * \brief 해당 씬에 .fbx 파일을 참조하여 게임 오브젝트를 생성합니다.
		 * \param fbxFileName .fbx file name to create gameobject.
		 * \return 생성된 게임 오브젝트
		 */
		DUOLGameEngine::GameObject* CreateFromFBXModel(const DUOLCommon::tstring& fbxFileName);

		/**
			@brief 해당 씬에 .dfx 파일을 참조하여 게임 오브젝트를 생성합니다.
			@param  ParticleFileName - .dfx의 주소
			@retval                  -
		**/
		DUOLGameEngine::GameObject* CreateFromParticleData(const DUOLCommon::tstring& ParticleFileName);
#pragma endregion

	protected:
		/**
		 * \brief 씬의 이름입니다.
		 */
		DUOLCommon::tstring _name;

		/**
		 * \brief 씬의 경로입니다.
		 */
		DUOLCommon::tstring _path;

	public:
		const DUOLCommon::tstring& GetName() const;

		const DUOLCommon::tstring& GetPath() const;

		const std::vector<DUOLGameEngine::GameObject*> GetRootObjects() const;

		const std::vector<DUOLGameEngine::GameObject*> GetAllGameObjects() const;

		RTTR_REGISTRATION_FRIEND

#pragma region FRIEND_CLASS
		friend class SceneManager;

		friend class ObjectBase;

		friend class GameObject;

		friend class Transform;
#pragma endregion
	};
}