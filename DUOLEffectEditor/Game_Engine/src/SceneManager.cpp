#include "SceneManager.h"
#include <assert.h>
#include "IScene.h"
#include "CTime.h"
#include "ObjectManager.h"
#include "MuscleEngine.h"

namespace Muscle
{
	SceneManager::~SceneManager()
	{
		for (auto& iter : m_SceneMap)
		{
			iter.second.reset();
		}
		m_SceneMap.clear();

		for (auto& iter : m_DeleteScene)
		{
			iter.reset();
		}
		m_DeleteScene.clear();

		if (m_pCurrentScene != nullptr)
		{
			m_pCurrentScene.reset();
		}
		if (m_pLoadScene != nullptr)
		{
			m_pLoadScene.reset();
		}
	}

	std::shared_ptr<IScene> SceneManager::GetScene()
	{
		return m_pCurrentScene;
	}

	void SceneManager::RegistScene(std::shared_ptr<IScene>& scene)
	{
		// 모든 게임 씬들은 클라이언트 프로젝트에서 등록을 해야한다 !
		if (scene != nullptr)
		{
			m_SceneMap.insert({ scene->GetSceneName(), scene });
		}
	}

	void SceneManager::LoadScene(std::string sceneName)
	{
		_isLoadSceneReserved = true;


		m_pLoadScene = m_SceneMap.at(sceneName);
		assert(m_pLoadScene);
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
		//Muscle::XPad::Get()->StopVibration();

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