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
		/// �̰� �ۿ� ����� ȣ��� ���� .. �ƴϸ� �׳� �Ҹ��ڿ��� �����ұ�
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
			// Awake�� ��쿡�� ��Ȱ��ȭ ������ ���� ������Ʈ�� �����մϴ� ..!
			gameObject->OnAwake();
		}
	}

	void Scene::Start() const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
			{
				// Active���� ���� !
				gameObject->OnActive();

				gameObject->OnStart();
			}
		}
	}

	void Scene::Update(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			// Active Game Object�� ���Ͽ� ���� �ڷ� ������ ���ΰ� �� �������� if �б� �ϴ� ����
			// ���� �� ���� �� ���� ..! (���� ���� �ʼ� !)
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
		// �� �� ���� ������Ʈ �迭���� ���� ���� �ִٸ�, Ȱ��ȭ�Ѵ�.
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
		// �� �� ���� ������Ʈ �迭���� ���� ���� �ִٸ�, ��Ȱ��ȭ�Ѵ�.
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