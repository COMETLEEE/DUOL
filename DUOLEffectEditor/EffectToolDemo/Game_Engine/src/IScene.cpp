#include "IScene.h"
#include "ObjectManager.h"
namespace Muscle
{
	IScene::IScene(std::string sceneName)
	{
		m_SceneName = sceneName;

		m_ObjManager = std::make_shared<ObjectManager>();
	}

	IScene::~IScene()
	{
		m_SceneName.clear();

		m_ObjManager.reset();

	}

	std::shared_ptr<ObjectManager> IScene::GetObjManager()
	{
		return m_ObjManager;
	}
}
