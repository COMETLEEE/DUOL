/**

    @file      SceneManager.h
    @brief     Scene Management at run-time
    @details   ~
    @author    COMETLEE
    @date      19.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <map>

#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"

namespace DUOLGameEngine
{
	class Scene;

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
	class SceneManager : public SingletonBase<SceneManager>
	{
		DECLARE_SINGLETON(SceneManager)

		DELETE_COPY_MOVE(SceneManager)

	private:
		virtual ~SceneManager() override;

	private:
		uint32_t _sceneCount;

		std::map<tstring, std::shared_ptr<Scene>> _sceneInEngine;

	public:
		void Initialize();

		void UnInitialize();

		void LoadScene(const tstring& sceneName, LoadSceneMode mode = LoadSceneMode::Single);
	};
}