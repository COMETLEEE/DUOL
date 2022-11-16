#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGameEngine
{
	Scene::Scene(const DUOLCommon::tstring& name) :
		_gameObjectsInScene(std::list<std::shared_ptr<GameObject>>())
		, _gameObjectsForCreate(std::vector<std::shared_ptr<GameObject>>())
		, _gameObjectsForDestroy(std::list<std::pair<std::shared_ptr<GameObject>, float>>())
		, _gameObjectsForActive(std::list<std::shared_ptr<GameObject>>())
		, _gameObjectsForInActive(std::list<std::shared_ptr<GameObject>>())
		, _name(name)
		, _path(DUOLCommon::StringHelper::ToTString("Empty"))
	{
	}

	Scene::~Scene()
	{
		/// <summary>
		/// 이걸 밖에 명시적 호출로 뺄까 .. 아니면 그냥 소멸자에서 정리할까
		/// </summary>
		UnInitialize();
	}

	void Scene::UnInitialize()
	{
		for (auto& gameObject : _gameObjectsForCreate)
			gameObject.reset();

		for (auto& gameObject : _gameObjectsForDestroy)
			gameObject.first.reset();

		for (auto& gameObject : _gameObjectsForActive)
			gameObject.reset();

		for (auto& gameObject : _gameObjectsForInActive)
			gameObject.reset();

		for (auto& gameObject : _gameObjectsInScene)
			gameObject.reset();
	}

	void Scene::Awake()
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			// Awake의 경우에는 비활성화 상태의 게임 오브젝트도 실행합니다 ..!
			gameObject->OnAwake();
		}
	}

	void Scene::Start() const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
			{
				// Active까지 시작 !
				gameObject->OnActive();

				gameObject->OnStart();
			}
		}
	}

	void Scene::Update(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			// Active Game Object에 대하여 따로 자료 구조를 놔두고 매 구문마다 if 분기 하는 것을
			// 줄일 수 있을 것 같다 ..! (추후 개선 필수 !)
			if (gameObject->GetIsActive())
				gameObject->OnUpdate(deltaTime);
		}
	}

	void Scene::InvokeUpdate(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
				gameObject->OnInvokeUpdate(deltaTime);
		}
	}

	void Scene::CoroutineUpdate(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
				gameObject->OnCoroutineUpdate(deltaTime);
		}
	}

	void Scene::FixedUpdate(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
				gameObject->OnFixedUpdate(deltaTime);
		}
	}

	void Scene::LateUpdate(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
				gameObject->OnLateUpdate(deltaTime);
		}
	}

	void Scene::CreateGameObjects()
	{
		// TODO
	}

	void Scene::DestroyGameObjects()
	{
		// TODO
	}

	void Scene::ActiveGameObjects()
	{
		// 씬 내 게임 오브젝트 배열에서 같은 것이 있다면, 활성화한다.
		for (auto iter = _gameObjectsForActive.begin() ; iter != _gameObjectsForActive.end(); )
		{
			for (const auto& gameObject : _gameObjectsInScene)
			{
				if (*iter == gameObject)
				{
					(*iter)->OnActive();

					iter = _gameObjectsForActive.erase(iter);

					break;
				}
				else
					++iter;
			}
		}
	}

	void Scene::InActiveGameObjects()
	{
		// 씬 내 게임 오브젝트 배열에서 같은 것이 있다면, 비활성화한다.
		for (auto iter = _gameObjectsForInActive.begin(); iter != _gameObjectsForInActive.end(); )
		{
			for (const auto& gameObject : _gameObjectsInScene)
			{
				if (*iter == gameObject)
				{
					(*iter)->OnInActive();

					iter = _gameObjectsForInActive.erase(iter);

					break;
				}
				else
					++iter;
			}
		}
	}

	std::shared_ptr<GameObject> Scene::CreateEmpty()
	{
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(TEXT("EmptyObject"));

		gameObject->AddComponent<Transform>();

		gameObject->_scene = this->weak_from_this();

		_gameObjectsInScene.push_back(gameObject);

		return gameObject;
	}
}