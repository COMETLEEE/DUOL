#include "pch.h"
#include "IScene.h"

namespace Muscle
{
	IScene::IScene(std::string sceneName)
	{
		m_SceneName = sceneName;

		m_ObjManager = std::make_shared<ObjectManager>();
	}

	std::shared_ptr<ObjectManager> IScene::GetObjManager()
	{
		return m_ObjManager;
	}
}
