#include "pch.h"
#include "SceneManager.h"

namespace Muscle
{
	SceneManager::~SceneManager()
	{
		if (m_pCurrentScene != nullptr)
		{
			m_pCurrentScene.reset();
		}
	}

	std::shared_ptr<IScene> SceneManager::GetScene()
	{
		return m_pCurrentScene;
	}

	void SceneManager::RegistScene(std::shared_ptr<IScene> scene)
	{
		// 모든 게임 씬들은 클라이언트 프로젝트에서 등록을 해야한다 !
		if (scene != nullptr)
		{
			m_SceneMap.insert({ scene->GetSceneName(), scene });
		}
	}

	void SceneManager::LoadScene(std::string sceneName)
	{
		if (m_SceneMap.find(sceneName) == m_SceneMap.end())
		{
			static_assert(true, "Scene load failed. Check scene's name.");
		}

		_isLoadSceneReserved = true;


		if (GetCurrentSceneName() != "IntroScene")
		{
		// 로딩씬은 게임 엔진 안으로 넣는게 좋을지도..?
			m_pLoadScene = m_SceneMap.at("IntroScene"); // 로딩씬을 호출한다..
			char* str = new char[sceneName.size() + 1];
			std::copy(sceneName.begin(), sceneName.end(), str);
			str[sceneName.size()] = '\0';

			m_pLoadScene->_inputData = str; // 다음 씬 정보를 넘긴다.
		}
		else
			m_pLoadScene = m_SceneMap.at(sceneName);
	}

	void SceneManager::FlipScene()
	{
		// 현재 씬을 청소하고 넘어갑시다.
		if (m_pCurrentScene != nullptr)
		{
			// m_DeleteScene.push_back(m_pCurrentScene);

			m_pCurrentScene->GetObjManager()->DeleteAll();

			m_pCurrentScene->GetObjManager()->SetIsStart(false);
		}

		m_pCurrentScene = m_pLoadScene;

		Muscle::MuscleEngine::GetInstance()->SetObjManager(m_pCurrentScene->GetObjManager());

		CTime::Start();// ->Start();

		m_pCurrentScene->Start();

		m_pCurrentScene->GetObjManager()->Start();

		// 씬 넘길 때에는 모든 진동을 없앤다.
		Muscle::XPad::Get()->StopVibration();

		_isLoadSceneReserved = false;
	}

	std::string SceneManager::GetCurrentSceneName()
	{
		if (m_pCurrentScene != nullptr)
			return m_pCurrentScene->GetSceneName();
		else
			return "";
	}

	void SceneManager::DeleteScene()
	{
		if (!m_DeleteScene.empty())
		{
			m_DeleteScene.back().reset();

			m_DeleteScene.clear();
		}
	}
}