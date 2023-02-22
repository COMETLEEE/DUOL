/**

    @file      SceneManager.h
    @brief     Scene Management at run-time
    @details   ~
    @author    COMETLEE
    @date      19.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <functional>
#include <map>
#include <vector>

#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"


namespace DUOLGameEngine
{
	/**
	 * \brief 씬을 어떻게 로드할지 설정합니다. 디폴트는 Single Mode 입니다.
	 */
	enum class LoadSceneMode
	{
		/**
		 * \brief 현재 로드되어 있는 씬을 모드 종료하고 로드합니다.
		 */
		Single,

		/**
		 * \brief 현재 로드되어 있는 씬에 추가하고 로드합니다.
		 */
		Additive
	};

	/**
	 * \brief 실행 시간에 씬의 로드, 보유, 저장 등의 관리를 담당합니다.
	 */
	class DUOL_GAMEENGINE_API SceneManager : public SingletonBase<SceneManager>
	{
		DECLARE_SINGLETON(SceneManager)

		DELETE_COPY_MOVE(SceneManager)

	public:
		void Initialize();

		void UnInitialize();

		/**
		 * \brief 게임 플레이 로직을 업데이트합니다.
		 * \param deltaTime 프레임 간격
		 */
		void Update(float deltaTime);

		/**
		 * \brief Editor 의 Edit, Pause Mode에서의 로직을 업데이트합니다.
		 * \param deltaTime 프레임 간격
		 */
		void UpdateEditAndPauseMode(float deltaTime);

	private:
		virtual ~SceneManager() override;

	private:
		/**
		 * \brief 해당 게임에서 빌드할 때 등록된 씬들의 집합
		 */
		std::map<DUOLCommon::tstring, std::shared_ptr<Scene>> _scenesInGame;

		/**
		 * \brief 일단 단일 씬의 루프에 대해서만 생각해보자 ..
		 */
		std::shared_ptr<DUOLGameEngine::Scene> _currentScene;

		std::shared_ptr<DUOLGameEngine::Scene> _reservedScene;

	private:
		void ChangeScene();

		bool _isReservedChangeScene;

	public:
		Scene* GetCurrentScene();

		void LoadScene(const DUOLCommon::tstring& sceneName, LoadSceneMode mode = LoadSceneMode::Single);

		void AddGameScene(const std::shared_ptr<DUOLGameEngine::Scene>& scene);

		inline int GetSceneCount() const { return static_cast<int>(_scenesInGame.size()); }

		void SaveCurrentScene();

		DUOLGameEngine::Scene* LoadSceneFile(const DUOLCommon::tstring& sceneName);
#pragma region FRIEND_CLASS
		friend class Engine;
#pragma endregion
	};
}