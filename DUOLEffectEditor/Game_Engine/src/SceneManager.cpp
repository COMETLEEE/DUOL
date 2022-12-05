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
		// ��� ���� ������ Ŭ���̾�Ʈ ������Ʈ���� ����� �ؾ��Ѵ� !
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
		// ���� ���� û���ϰ� �Ѿ�ô�.
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

		// �� �ѱ� ������ ��� ������ ���ش�.
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