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
		// ��� ���� ������ Ŭ���̾�Ʈ ������Ʈ���� ����� �ؾ��Ѵ� !
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
		// �ε����� ���� ���� ������ �ִ°� ��������..?
			m_pLoadScene = m_SceneMap.at("IntroScene"); // �ε����� ȣ���Ѵ�..
			char* str = new char[sceneName.size() + 1];
			std::copy(sceneName.begin(), sceneName.end(), str);
			str[sceneName.size()] = '\0';

			m_pLoadScene->_inputData = str; // ���� �� ������ �ѱ��.
		}
		else
			m_pLoadScene = m_SceneMap.at(sceneName);
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