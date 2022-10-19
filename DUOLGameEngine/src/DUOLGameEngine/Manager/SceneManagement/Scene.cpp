#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLGameEngine
{
	Scene::Scene() :
		_gameObjectsInScene(std::vector<std::shared_ptr<GameObject>>())
		, _gameObjectsForCreate(std::vector<std::shared_ptr<GameObject>>())
		, _gameObjectsForDestroy(std::vector<std::pair<std::shared_ptr<GameObject>, float>>())
		, _gameObjectsForActive(std::vector<std::shared_ptr<GameObject>>())
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::Awake()
	{
		
	}

	void Scene::Start()
	{

	}

	void Scene::Update(float deltaTime)
	{

	}

	void Scene::FixedUpdate(float deltaTime)
	{

	}

	void Scene::LateUpdate(float deltaTime)
	{

	}

	void Scene::CreateGameObjects()
	{

	}

	void Scene::DestroyGameObjects()
	{

	}

	void Scene::ActiveGameObjects()
	{

	}
}